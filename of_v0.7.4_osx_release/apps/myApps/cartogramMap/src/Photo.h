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
    
    void draw();
    
private:
    ofxJSONElement json;
};

#endif /* defined(__emptyExample__Photo__) */
