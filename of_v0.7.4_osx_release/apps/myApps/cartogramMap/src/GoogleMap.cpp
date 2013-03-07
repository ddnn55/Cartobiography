//
//  GoogleMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#include "GoogleMap.h"

void GoogleMap::setLatLngBounds(const Bounds & bounds)
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

Bounds GoogleMap::latLngToNormalizedGoogleWorld(const Bounds & latLngBounds) const
{
    // https://developers.google.com/maps/documentation/javascript/maptypes#MapCoordinates
    ofVec2f worldNW = googleMercator(latLngBounds.left, latLngBounds.top);
    ofVec2f worldSE = googleMercator(latLngBounds.right, latLngBounds.bottom);
    return Bounds(worldNW.x, worldSE.x, worldNW.y, worldSE.y);
}

ofVec2f GoogleMap::googleMercator(float lng, float lat) const
{
    return ofVec2f(
        0.5 + lng / 360.0,
        0.5 + /* or minus? */ MAX(-.5, MIN(+.5, log(tan( (45.0 + lat / 2) * PI / 180.0 )) / (2.0 * PI) ))
    );
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
                        //100, 100, 256, 256);
    //tile.draw(x, y);
}