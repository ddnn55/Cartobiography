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
        std::string imagePath = json.get("path", Json::nullValue).asString();
        image.loadImage(imagePath);
        loadedImage = true;
    }
        
    float scale;
    if(image.width < image.height)
        scale = 2.0 * size / float(image.width);
    else
        scale = 2.0 * size / float(image.height);
    
    ofSetColor(255, 255, 255, 255);
    ofPushMatrix();
        ofScale(scale, scale);
        image.draw(-image.width/2, -image.height/2, image.width, image.height);
    ofPopMatrix();
    
    ofSetColor(255, 255, 255, 180);
    //ofCircle(0, 0, size);
}