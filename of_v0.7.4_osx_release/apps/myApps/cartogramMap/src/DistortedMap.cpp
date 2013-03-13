//
//  DistortionMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#define CARTOGRAM_GRID_SIZE 1024

#include <ofMain.h>

//#include "CBDebug.h"

#include "DistortedMap.h"

DistortedMap::DistortedMap(Bounds<float> bounds, std::string filename)
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    gMap = GoogleMap(5, bounds);
    
    /*ofFile file(filename);
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
    
    map.allocate(CARTOGRAM_GRID_SIZE+1, CARTOGRAM_GRID_SIZE+1, GL_RGB32F);
    map.loadData(data, (CARTOGRAM_GRID_SIZE+1), (CARTOGRAM_GRID_SIZE+1), GL_RGB);
    
    delete[] data;*/
    
    shaderLoaded = false;
}

void DistortedMap::draw(float x, float y)
{    
    if(!shaderLoaded)
    {
        distortionShader.load("Distortion");
        shaderLoaded = true;
    }
    
    distortionShader.begin();
        glBegin(GL_QUADS);
    
            glTexCoord2f(0.0, 0.0);
            glVertex2f(0.0, 0.0);
    
            glTexCoord2f(1.0, 0.0);
            glVertex2f(1280.0, 0.0);
    
            glTexCoord2f(1.0, 1.0);
            glVertex2f(1280.0, 720.0);
    
            glTexCoord2f(0.0, 1.0);
            glVertex2f(0.0, 720.0);
    
        glEnd();
    distortionShader.end();
}