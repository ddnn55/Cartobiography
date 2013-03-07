//
//  GoogleMap.h
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#ifndef __emptyExample__GoogleMap__
#define __emptyExample__GoogleMap__

#include <ofMain.h>

#include "Bounds.h"

class GoogleMap {
    
public:
    void setLatLngBounds(const Bounds & bounds);
    void setPixelSize(float width, float height);
    
    void draw(float x, float y);
    
    Bounds latLngToNormalizedGoogleWorld(const Bounds & latLngBounds) const;
    ofVec2f googleMercator(float lat, float lng) const;
    
private:
    void update();
    
    Bounds latLngBounds;
    float pixelWidth;
    float pixelHeight;
    
    Bounds normalizedWorldBounds;
    ofImage tile;
    bool loaded = false;
    
};

#endif /* defined(__emptyExample__GoogleMap__) */
