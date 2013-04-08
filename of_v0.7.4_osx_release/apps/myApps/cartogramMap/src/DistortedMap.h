//
//  DistortionMap.h
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#ifndef __emptyExample__DistortedMap__
#define __emptyExample__DistortedMap__

#include <iostream>
#include <vector>

#include "DNS/Geometry.h"
#include "GoogleMap.h"
#include "Photo.h"

class DistortedMap {
public:
    DistortedMap() { distortionAmount = 0.0; imageSize = 5.0; renderQuadTree = false; clicked = false; };
    void load(Bounds<float> bounds, std::string filename, std::string photosFilename);
    
    Bounds<float> screenBounds();
    
    ofVec2f lngLatToScreen(ofVec2f lngLat);
    
    void draw(float x, float y);
    void drawWireframe(float x, float y);
    void drawDerivative(float x, float y);
    
    ofVec2f derivativeAtScreenCoord(int x, int y);
    
    
    GoogleMap gMap;
    
    float distortionAmount;
    float imageSize;
    bool renderQuadTree;
    
    ofVec2f mousePos;
    bool clicked;
    
private:
    
    Bounds<float> gridBounds();
    
    ofShader imageShader;
    void drawPhotos();
    
    vector< Photo > photos;
    ofFloatImage distortion;
    //ofTexture distortion;
    
    ofShader shader;
    ofMesh mesh;
    
    //ofShader distortionShader;
    //bool shaderLoaded;
};

#endif /* defined(__emptyExample__DistortedMap__) */
