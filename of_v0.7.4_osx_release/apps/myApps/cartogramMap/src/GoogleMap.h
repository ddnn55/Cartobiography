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

#include <set>

class GoogleMap {
    
public:
    void setLatLngBounds(const Bounds & bounds);
    void setPixelSize(float width, float height);
    
    void draw(float x, float y);
    
    Bounds latLngToNormalizedGoogleWorld(const Bounds & latLngBounds) const;
    ofVec2f googleMercator(float lat, float lng) const;
    
private:
    void update();
    void ensureTilesInRange(int zoomLevel, int leftTile, int rightTile, int topTile, int bottomTile);
    void ensureTile(int zoomLevel, int x, int y);
    string tileKey(int zoomLevel, int x, int y);
    string tileLatLngCenterStr(int zoomLevel, int x, int y);
    ofVec2f tile2LatLng(int zoomLevel, float x, float y);
    
    Bounds latLngBounds;
    float pixelWidth;
    float pixelHeight;
    
    Bounds normalizedWorldBounds;
    float zoom;
    ofImage tile;
    map< string, ofImage > tiles;
    bool loaded = false;
    
};

#endif /* defined(__emptyExample__GoogleMap__) */
