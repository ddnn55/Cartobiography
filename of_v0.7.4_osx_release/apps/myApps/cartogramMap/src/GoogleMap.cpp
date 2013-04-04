//
//  GoogleMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#include "CBDebug.h"

#include "GoogleMap.h"

#include <string>

void GoogleMap::setLatLngBounds(const Bounds<float> & bounds)
{
    this->latLngBounds = bounds;
    update();
}

void GoogleMap::setScreenBounds(Bounds<float> screenBounds)
{
    //pixelWidth = width;
    //pixelHeight = height;
    this->screenBounds = screenBounds;
    update();
}

Bounds<float> GoogleMap::latLngToGooglePixel(const Bounds<float> & latLngBounds) const
{
    ofVec2f googleWorldPixelNW = latLngToGooglePixel(ofVec2f(latLngBounds.left, latLngBounds.top));
    ofVec2f googleWorldPixelSE = latLngToGooglePixel(ofVec2f(latLngBounds.right, latLngBounds.bottom));
    return Bounds<float>(googleWorldPixelNW.x, googleWorldPixelSE.x, googleWorldPixelNW.y, googleWorldPixelSE.y);
}

unsigned int powui(unsigned int x, unsigned int p) {
    unsigned int v = 1;
    for (unsigned int i = 0; i < p; i++) v *= x;
    return v;
}

ofVec2f GoogleMap::latLngToGooglePixel(ofVec2f latLng) const
{
    // https://developers.google.com/maps/documentation/javascript/maptypes#MapCoordinates
    ofVec2f normalizedWorld = googleMercator(latLng);
    return ofVec2f(256.0 * normalizedWorld.x * powui(2, zoomLevel),
                   256.0 * normalizedWorld.y * powui(2, zoomLevel));
}

ofVec2f GoogleMap::googleMercator(ofVec2f latLng) const
{
    return ofVec2f(
        0.5 + latLng.x / 360.0,
        0.5 + /* or minus? */ MAX(-.5, MIN(+.5, log(tan( (45.0 + latLng.y / 2) * PI / 180.0 )) / (2.0 * PI) ))
    );
}



void GoogleMap::update()
{
    
}

void GoogleMap::load(int zoomLevel, Bounds<float> latLngBounds)
{
    this->zoomLevel = zoomLevel;
    this->latLngBounds = latLngBounds;
    this->map = makeMap();
    this->map.setUseTexture(true);
    this->map.update();
}

void pasteImage(ofImage & from, ofImage & to, int toX, int toY)
{
    for(int y = 0; y < from.height; y++)
    {
        for(int x = 0; x < from.width; x++)
        {
            if((toX + x >= 0) && (toY + y >= 0) && (toX + x < to.width) && (toY + y < to.height))
                to.setColor(toX + x, toY + y, from.getColor(x, y));
        }
    }
}

ofImage GoogleMap::makeMap()
{
    CBLog(latLngBounds);
    
    // calculate pixel bounds, allocate image
    Bounds<float> googlePixelBounds = latLngToGooglePixel(latLngBounds);
    ofImage map;
    map.allocate(ceil(googlePixelBounds.width()), ceil(googlePixelBounds.height()), OF_IMAGE_COLOR);
    
    // get necessary tiles and draw them into image
    for(int tilePixelCenterX = 256; tilePixelCenterX < googlePixelBounds.width() + 257; tilePixelCenterX += 512)
    {
        for(int tilePixelCenterY = 256; tilePixelCenterY < googlePixelBounds.height() + 257; tilePixelCenterY += 512)
        {
            ofVec2f googlePixelTileCenter(googlePixelBounds.left + tilePixelCenterX, googlePixelBounds.bottom + tilePixelCenterY);
            ofVec2f tileCenterLatLng = googlePixelToLatLng(googlePixelTileCenter);
            
            CBLog(tileCenterLatLng);
            
            TileLocator tileLocator;
            tileLocator.center = tileCenterLatLng;
            tileLocator.zoomLevel = zoomLevel;
            
            ofImage tile = loadTileFromGoogleCached(tileLocator);
            
            pasteImage(tile, map, tilePixelCenterX - 256, map.height - tilePixelCenterY - 256);
        }
    }
    
    return map;
}

ofImage GoogleMap::loadTileFromGoogleCached(TileLocator tileLocator)
{
    string cachedTilePath = "tiles/" + tileLocator.filename();
    
    if(ofFile(cachedTilePath).exists())
    {
        CBLog("Loading from cache: " + cachedTilePath);
        return ofImage(cachedTilePath);
    }
    
    stringstream urlStream;
    urlStream << "http://maps.googleapis.com/maps/api/staticmap?";
    urlStream << "center=" << tileLocator.center.y << "," << tileLocator.center.x;
    urlStream << "&zoom=" << tileLocator.zoomLevel;
    urlStream << "&scale=1&size=512x512&sensor=false";
    
    CBLog("Loading from Google: " + urlStream.str());

    ofHttpResponse response = ofLoadURL(urlStream.str());

    
    
    ofImage tile;
    tile.loadImage(response.data);
    tile.saveImage(cachedTilePath);
    
    return tile;
}

Bounds<int> GoogleMap::getTileBounds()
{
    int zoomLevel = floor(zoom);
    int tileSize = powui(2, zoomLevel);
    int leftTile   = floor(normalizedWorldBounds.left   * tileSize),
        rightTile  = floor(normalizedWorldBounds.right  * tileSize),
        topTile    = floor(normalizedWorldBounds.top    * tileSize),
        bottomTile = floor(normalizedWorldBounds.bottom * tileSize);
    
    return Bounds<int>(leftTile, rightTile, topTile, bottomTile);
}

string GoogleMap::tileKey(int zoomLevel, int x, int y)
{
    stringstream keyStream;
    keyStream << zoomLevel << "-" << x << "-" << y;
    return keyStream.str();
}

string GoogleMap::tileLatLngCenterStr(int zoomLevel, int x, int y)
{
    ofVec2f centerLatLng = tile2LatLng(zoomLevel, float(x) + 0.5, float(y) + 0.5);
    
    stringstream centerStream;
    centerStream << centerLatLng.y << "," << centerLatLng.x;
    return centerStream.str();
}

ofVec2f GoogleMap::tile2LatLng(int zoomLevel, float x, float y)
{
    int tileSize = powui(2, zoomLevel);
    
    float lng = -180.0 + 360.0 * float(x) / float(tileSize);
    float lat = 2.0 * ( (180.0 / PI) * atan( pow( 10.0, /*2.0 * */ PI * ( y / float(tileSize) - 0.5 ) ) ) - 45.0 );
    
    return ofVec2f(lng, lat);
}

ofVec2f GoogleMap::googlePixelToLatLng(ofVec2f googlePixel) const
{
    ofVec2f normalizedGoogleWorld(googlePixelToNormalizedGoogleWorld(googlePixel));
    
    float y = 0.5 - normalizedGoogleWorld.y;
    
    float lat = -1.0 * (180.0 / PI) * (2 * atan(exp(2.0 * PI * y)) - PI / 2);
    
    float lng = -180.0 + 360.0 * normalizedGoogleWorld.x;
    //float lat = 2.0 * ( (180.0 / PI) * atan( pow( 10.0, 2.0 * PI * ( normalizedGoogleWorld.y - 0.5 ) ) ) - 45.0 );
    
    return ofVec2f(lng, lat);
}

ofVec2f GoogleMap::googlePixelToNormalizedGoogleWorld(ofVec2f googlePixel) const
{
    return ofVec2f((googlePixel.x / powui(2, zoomLevel)) / 256.0,
                   (googlePixel.y / powui(2, zoomLevel)) / 256.0);
}

void GoogleMap::draw(float x, float y)
{
    /*tile.drawSubsection(
        x, y, screenBounds.width(), screenBounds.height(),
        tile.width  * normalizedWorldBounds.left,
        tile.height * (1.0 - normalizedWorldBounds.top),
        tile.width  * normalizedWorldBounds.width(),
        tile.height * normalizedWorldBounds.height()
    );*/
    
//    Bounds<int> tileBounds = getTileBounds();
//    for(int googleTileCol = tileBounds.left; googleTileCol <= tileBounds.right; googleTileCol++)
//    {
//        for(int googleTileRow = tileBounds.top; googleTileRow <= tileBounds.bottom; googleTileRow++)
//        {
//            int localTileCol = googleTileCol - tileBounds.left,
//                localTileRow = googleTileRow - tileBounds.top;
//            
//            ofImage* tile = & tiles[tileKey(zoomLevel(), googleTileCol, googleTileRow)];
//            
//            Bounds<float> crop = Bounds<float>(*tile);
//            if(googleTileCol < normalizedWorldBounds.left * tileSize())
//            {
//                crop.left = (normalizedWorldBounds.left * tileSize() - float(googleTileCol)) /* * float(tile->width) */;
//            }
//            
//            Bounds<float> viewport;
//            viewport.left   = screenBounds.left + screenBounds.width() * float(localTileCol) / float(tileBounds.width() + 1);
//            viewport.right  = viewport.left + (localTileCol + 1) * screenBounds.width() / float(tileBounds.width() + 1);
//            viewport.top    = screenBounds.top - screenBounds.height() * float(localTileRow) / float(tileBounds.height() + 1);
//            viewport.bottom = viewport.top - (localTileRow + 1) * screenBounds.height() / float(tileBounds.height() + 1);
//            
//            //if(tileCol < normalizedWorldBounds.left * tileSize())
//            //    asdfasdfasdfsa
//            
//            tile->drawSubsection(
//                viewport.left, viewport.bottom, viewport.width(), viewport.height(),
//                crop.left, crop.bottom, crop.width(), crop.height()
//            );
//            
//            CBLog(viewport.toString());
//        }
//    }
//    CBLog("--------------");
    
    map.draw(32, 32, ofGetWidth() - 64, (ofGetWidth() - 64) * map.height / map.width);
    
    ofSetLineWidth(3.0);
    ofLine(screenBounds.left, screenBounds.top, screenBounds.right, screenBounds.top);
    ofLine(screenBounds.left, screenBounds.bottom, screenBounds.right, screenBounds.bottom);
    ofLine(screenBounds.left, screenBounds.top, screenBounds.left, screenBounds.bottom);
    ofLine(screenBounds.right, screenBounds.top, screenBounds.right, screenBounds.bottom);
}