//
//  DistortionMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#define CARTOGRAM_GRID_SIZE 1024

#include <ofMain.h>

#include "DistortedMap.h"


void DistortedMap::load(Bounds<float> bounds, std::string filename)
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //ofBackground(0,0,0);
    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    glEnable(GL_DEPTH_TEST);
        
    shader.load("shaders/distort.vert", "shaders/distort.frag");
    
    gMap.load(4, bounds);
    
    ofFile file(filename);
    ofBuffer contents = file.readToBuffer();
    std::string line;
    
    float* data = new float[(CARTOGRAM_GRID_SIZE+1) * (CARTOGRAM_GRID_SIZE+1) * 3];
    
    for(int y = CARTOGRAM_GRID_SIZE; y >= 0; y--)
    {
        for(int x = 0; x < CARTOGRAM_GRID_SIZE+1; x++)
        {
            line = contents.getNextLine();
            istringstream stream(line);
            float distortedX, distortedY;
            stream >> distortedX;
            stream >> distortedY;
            
            distortedX /= float(CARTOGRAM_GRID_SIZE);
            distortedY /= float(CARTOGRAM_GRID_SIZE);
            
            data[ 3 * (y * (CARTOGRAM_GRID_SIZE+1) + x)     ] = distortedX;
            data[ 3 * (y * (CARTOGRAM_GRID_SIZE+1) + x) + 1 ] = distortedY;
            
        }
    }
    
    distortion.allocate(CARTOGRAM_GRID_SIZE+1, CARTOGRAM_GRID_SIZE+1, GL_RGB32F);
    distortion.loadData(data, (CARTOGRAM_GRID_SIZE+1), (CARTOGRAM_GRID_SIZE+1), GL_RGB);
    
    delete[] data;
}

void DistortedMap::draw(float x, float y)
{
    ofSetColor(255);
    
    /**************  start working code  **************/
    shader.begin();
    shader.setUniformTexture("colormap", gMap.map, 0);
    shader.setUniformTexture("distortion", distortion, 1);
    //shader.setUniform1i("maxHeight",ofGetMouseX());
    
    ofPoint se = gMap.map.getTextureReference().getCoordFromPercent(1, 1);
    
        glBegin(GL_QUADS);

            glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
            glVertex2f(0.0, 0.0);
            
            glMultiTexCoord2f(GL_TEXTURE0, se.x, 0.0);
            glVertex2f(1024.0, 0.0);
            
            glMultiTexCoord2f(GL_TEXTURE0, se.x, se.y);
            glVertex2f(1024.0, 1024.0);
            
            glMultiTexCoord2f(GL_TEXTURE0, 0.0, se.y);
            glVertex2f(0.0, 1024.0);
    
        glEnd();
    
    shader.end();
}