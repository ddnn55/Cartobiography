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

std::string Photo::filePath()
{
    return json.get("path", Json::nullValue).asString();
}

void Photo::launch() {
    std::string thumbnailPath = filePath();
    size_t index = thumbnailPath.find("iPhone");
    std::string orginalPath = thumbnailPath.substr(0, index) + "iPhoneOriginals" + thumbnailPath.substr(index + 6);
    //cout << orginalPath << endl;
    ofLaunchBrowser( orginalPath );
}
    
void Photo::draw(float size, ofShader & shader)
{
    if(!loadedImage)
    {
        std::string imagePath = filePath();
        image.loadImage(imagePath);
        aspect = float(image.width) / float(image.height);
        loadedImage = true;
    }
        
    ofVec2f normalImageSize;
    if(image.width < image.height)
        //scale = 2.0 * size / float(image.width);
        normalImageSize = ofVec2f( 1.0, 1.0 / aspect );
    else
        //scale = 2.0 * size / float(image.height);
        normalImageSize = ofVec2f( aspect, 1.0 );
    
    ofSetColor(255, 255, 255, 255);
    ofPushMatrix();
        //ofScale(scale, scale);
        //image.draw(-image.width/2, -image.height/2, image.width, image.height);
    
        shader.setUniformTexture("image", image, 0);
        shader.setUniform2f("normalImageSize", normalImageSize.x, normalImageSize.y);
        ofCircle(0.0, 0.0, size);
    ofPopMatrix();
    
    //ofSetColor(255, 255, 255, 180);
    //ofCircle(0, 0, size);
}