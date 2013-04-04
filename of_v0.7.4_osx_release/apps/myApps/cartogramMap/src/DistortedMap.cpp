//
//  DistortionMap.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#define CARTOGRAM_GRID_SIZE 128

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
    
    distortion.allocate(CARTOGRAM_GRID_SIZE+1, CARTOGRAM_GRID_SIZE+1, OF_IMAGE_COLOR);
    //distortion.loadData(data, (CARTOGRAM_GRID_SIZE+1), (CARTOGRAM_GRID_SIZE+1), GL_RGB);
    
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
            
            distortion.getPixelsRef().setColor(x, y, ofFloatColor(distortedX, distortedY, 0.0, 1.0));
            
        }
    }
    
    
    //distortion.allocate(CARTOGRAM_GRID_SIZE+1, CARTOGRAM_GRID_SIZE+1, GL_RGB32F);
    //distortion.loadData(data, (CARTOGRAM_GRID_SIZE+1), (CARTOGRAM_GRID_SIZE+1), GL_RGB);
    distortion.update();
    
    delete[] data;
    
    
    ofPoint distortionSE = distortion.getTextureReference().getCoordFromPercent(1, 1);
    
    for(int y = 0; y < CARTOGRAM_GRID_SIZE+1; y++)
    {
        std::vector< ofVec3f > vertices;
        //std::vector< ofVec2f > texCoords;
        for(int x = 0; x < CARTOGRAM_GRID_SIZE+1; x++)
        {
            vertices.push_back(ofVec3f(x, y, 0.0));
            //texCoords.push_back(ofVec2f(distortionSE.x * float(x) / float(CARTOGRAM_GRID_SIZE),
            //                            distortionSE.y * float(y) / float(CARTOGRAM_GRID_SIZE)));
            
            if(y > 0 && x < CARTOGRAM_GRID_SIZE)
            {
                mesh.addTriangle((y-1) * (CARTOGRAM_GRID_SIZE+1) + x,
                                 (y-1) * (CARTOGRAM_GRID_SIZE+1) + x+1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x);
                
                mesh.addTriangle((y-1) * (CARTOGRAM_GRID_SIZE+1) + x+1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x+1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x);
            }
        }
        mesh.addVertices(vertices);
    }
    
    shader.begin();
    shader.setUniform1i("derivative", 0);
    shader.end();
    
    
}

ofVec2f DistortedMap::derivativeAtScreenCoord(int x, int y)
{
    ofVec2f gridCoord ( CARTOGRAM_GRID_SIZE * float(x) / float(ofGetWidth()), CARTOGRAM_GRID_SIZE * float(y) / float(ofGetHeight()));
    //ofVec2f derivative(  );
    return gridCoord;
}

void DistortedMap::drawWireframe(float x, float y)
{
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    draw(x, y);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
}

void DistortedMap::drawDerivative(float x, float y)
{
    shader.begin();
    shader.setUniform1i("derivative", 1);
    shader.end();
    
    draw(x, y);
    
    shader.begin();
    shader.setUniform1i("derivative", 0);
    shader.end();
}

void DistortedMap::draw(float x, float y)
{
    
    ofSetColor(255);
    
    shader.begin();
    shader.setUniformTexture("colormap", gMap.map, 0);
    shader.setUniformTexture("distortion", distortion, 1);
    
    ofPoint mapSE = gMap.map.getTextureReference().getCoordFromPercent(1, 1);
    
    ofPoint distortionSE = distortion.getTextureReference().getCoordFromPercent(1, 1);
    shader.setUniform2f("mapSize", mapSE.x, mapSE.y);
    shader.setUniform2f("distortionSize", distortionSE.x, distortionSE.y);
    shader.setUniform2f("meshSize", (CARTOGRAM_GRID_SIZE+1), (CARTOGRAM_GRID_SIZE+1));
    
    shader.setUniform1f("normalMouseX", float(ofGetMouseX()) / float(ofGetWidth()));
    
    float aspect = gMap.map.width / gMap.map.height;
    float scaleX = float(ofGetWidth()) / float(CARTOGRAM_GRID_SIZE+1);
    float scaleY = float(ofGetHeight()) / float(CARTOGRAM_GRID_SIZE+1);
    
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(scaleX, scaleY, 1);
    
    mesh.draw();
    glPopMatrix();
    
    shader.end();
}