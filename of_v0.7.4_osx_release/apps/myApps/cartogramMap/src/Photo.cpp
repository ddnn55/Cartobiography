//
//  Photo.cpp
//  emptyExample
//
//  Created by David Stolarsky on 4/5/13.
//
//

#include "Photo.h"

Photo::Photo(ofxJSONElement photoJSON)
{
    this->json = photoJSON;
    loadedImage = false;
}

ofVec2f Photo::lngLat()
{
    Json::Value lng = json.get("lon", Json::nullValue);
    Json::Value lat = json.get("lat", Json::nullValue);
    
    return ofVec2f( lng.asDouble(), lat.asDouble() );
}

void Photo::draw(float size)
{
    if(!loadedImage)
    {
        image.loadImage(json.get("path", Json::nullValue).asString());
        loadedImage = true;
    }
    
    ofCircle(0, 0, size);
}