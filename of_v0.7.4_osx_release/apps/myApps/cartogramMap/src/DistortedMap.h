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

#include "Bounds.h"
#include "GoogleMap.h"

class DistortedMap {
public:
    DistortedMap() {};
    void load(Bounds<float> bounds, std::string filename);
    
    void draw(float x, float y);
    void drawWireframe(float x, float y);
    void drawDerivative(float x, float y);
    
private:
    ofTexture distortion;
    GoogleMap gMap;
    
    ofShader shader;
    ofMesh mesh;
    
    //ofShader distortionShader;
    //bool shaderLoaded;
};

#endif /* defined(__emptyExample__DistortedMap__) */
