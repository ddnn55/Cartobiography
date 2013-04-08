//
//  Photo.h
//  emptyExample
//
//  Created by David Stolarsky on 4/5/13.
//
//

#ifndef __emptyExample__Photo__
#define __emptyExample__Photo__

#include "ofxJSONElement.h"

class Photo
{
public:
    Photo(ofxJSONElement photoJSON);
    
    ofVec2f lngLat();
    std::string filePath();
    
    void draw(float size, ofShader & shader);
    
    void launch();
    
private:
    ofxJSONElement json;
    ofImage image;
    float aspect;
    bool loadedImage;
};

#endif /* defined(__emptyExample__Photo__) */
