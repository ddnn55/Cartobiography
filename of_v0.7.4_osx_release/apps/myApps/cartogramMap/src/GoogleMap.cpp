//
//  GoogleMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#include "GoogleMap.h"

#include <string>

void GoogleMap::setLatLngBounds(const Bounds<float> & bounds)
{
    this->latLngBounds = bounds;
    update();
}

void GoogleMap::setPixelSize(float width, float height)
{
    pixelWidth = width;
    pixelHeight = height;
    update();
}

Bounds<float> GoogleMap::latLngToNormalizedGoogleWorld(const Bounds<float> & latLngBounds) const
{
    // https://developers.google.com/maps/documentation/javascript/maptypes#MapCoordinates
    ofVec2f worldNW = googleMercator(latLngBounds.left, latLngBounds.top);
    ofVec2f worldSE = googleMercator(latLngBounds.right, latLngBounds.bottom);
    return Bounds<float>(worldNW.x, worldSE.x, worldNW.y, worldSE.y);
}

ofVec2f GoogleMap::googleMercator(float lng, float lat) const
{
    return ofVec2f(
        0.5 + lng / 360.0,
        0.5 + /* or minus? */ MAX(-.5, MIN(+.5, log(tan( (45.0 + lat / 2) * PI / 180.0 )) / (2.0 * PI) ))
    );
}

int powi(int x, int p) {
    int i = 1;
    for (int j = 1; j < p; j++)  i *= x;
    return i;
}

void GoogleMap::update()
{
    if(!loaded)
    {
        ofHttpResponse response = ofLoadURL("http://maps.googleapis.com/maps/api/staticmap?center=0,0&zoom=1&scale=2&size=512x512&sensor=false");
        tile.loadImage(response.data);
        loaded = true;
    }
    
    normalizedWorldBounds = latLngToNormalizedGoogleWorld(latLngBounds);
    
    float googleWorldWidth = normalizedWorldBounds.width() * 256.0;
    zoom = log(pixelWidth / googleWorldWidth) / log(2.0);
    
    Bounds<int> tileBounds = getTileBounds();
    // which tiles do we need?

    
    ensureVisibleTiles();
    
    
}

Bounds<int> GoogleMap::getTileBounds()
{
    int zoomLevel = floor(zoom);
    int tileSize = powi(2, zoomLevel);
    int leftTile   = floor(normalizedWorldBounds.left   * tileSize),
        rightTile  = floor(normalizedWorldBounds.right  * tileSize),
        topTile    = floor(normalizedWorldBounds.top    * tileSize),
        bottomTile = floor(normalizedWorldBounds.bottom * tileSize);
    
    cout << "tiles(zoomLevel = " << zoomLevel << "): (" << leftTile << ", " << topTile << ") -- (" << rightTile << ", " << bottomTile << ")" << endl;
    
    return Bounds<int>(leftTile, rightTile, topTile, bottomTile);
}

void GoogleMap::ensureVisibleTiles()
{
    int zoomLevel = floor(zoom);
    Bounds<int> bounds = getTileBounds();
    
    if(zoomLevel < 0 || zoomLevel > 30) // 30 is generous
        return;
    
    for(int x = bounds.left; x <= bounds.right; x++)
    {
        for(int y = bounds.top; y <= bounds.bottom; y++)
        {
            ensureTile(zoomLevel, x, y);
        }
    }
}

void GoogleMap::ensureTile(int zoomLevel, int x, int y)
{
    if(tiles.find(tileKey(zoomLevel, x, y)) == tiles.end())
    {
        stringstream urlStream;
        urlStream << "http://maps.googleapis.com/maps/api/staticmap?";
        urlStream << "center=" << tileLatLngCenterStr(zoomLevel, x, y);
        urlStream << "&zoom=" << zoomLevel;
        urlStream << "&scale=2&size=512x512&sensor=false";
        
        ofHttpResponse response = ofLoadURL(urlStream.str());
        ofImage tile;
        tile.loadImage(response.data);
        tiles[tileKey(zoomLevel, x, y)] = tile;
        
        cout << "downloaded " << urlStream.str() << endl;
    }
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
    int tileSize = powi(2, zoomLevel);
    
    float lng = -180.0 + 360.0 * float(x) / float(tileSize);
    float lat = 2.0 * ( (180.0 / PI) * atan( pow( 10.0, /*2.0 * */ PI * ( y / float(tileSize) - 0.5 ) ) ) - 45.0 );
    
    return ofVec2f(lng, lat);
}

void GoogleMap::draw(float x, float y)
{
    
    tile.drawSubsection(
        x, y, pixelWidth, pixelHeight,
        tile.width  * normalizedWorldBounds.left,
        tile.height * (1.0 - normalizedWorldBounds.top),
        tile.width  * normalizedWorldBounds.width(),
        tile.height * normalizedWorldBounds.height()
    );
}