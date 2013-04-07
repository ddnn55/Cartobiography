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
}

ofVec2f Photo::lngLat()
{
    Json::Value lng = json.get("lon", Json::nullValue);
    Json::Value lat = json.get("lat", Json::nullValue);
    
    return ofVec2f( lng.asDouble(), lat.asDouble() );
}

void Photo::draw()
{
    //ofRect(-10, -10, 0, 20, 20);
    ofCircle(0, 0, 5.0);
}