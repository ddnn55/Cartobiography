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

#include "ofxJSONElement.h"


void DistortedMap::load(Bounds<float> bounds, std::string distortionFilename, std::string photosFilename)
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //ofBackground(0,0,0);
    ofDisableArbTex();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    glEnable(GL_DEPTH_TEST);
    
    shader.load("shaders/distort.vert", "shaders/distort.frag");
    
    gMap.load(4, bounds);
    
    ofFile file(distortionFilename);
    ofBuffer contents = file.readToBuffer();
    std::string line;
    
    ofxJSONElement photosJSON;
    photosJSON.open(photosFilename);
    for(int p = 0; p < photosJSON.size(); p++)
    {
        photos.push_back( Photo( photosJSON[p] ) );
    }
    
    distortion.allocate(CARTOGRAM_GRID_SIZE+1, CARTOGRAM_GRID_SIZE+1, OF_IMAGE_COLOR);
    
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
                        
            ofFloatColor color(distortedX, distortedY, 0.0, 1.0);
            distortion.getPixelsRef().setColor(x, y, color);
            
        }
    }
    
    
    distortion.update();
       
    
    ofPoint distortionSE = distortion.getTextureReference().getCoordFromPercent(1, 1);
    
    for(int y = 0; y < CARTOGRAM_GRID_SIZE+1; y++)
    {
        std::vector< ofVec3f > vertices;
        std::vector< ofFloatColor > colors;
        for(int x = 0; x < CARTOGRAM_GRID_SIZE+1; x++)
        {
            vertices.push_back(ofVec3f(x, y, 0.0));
            //colors.push_back(ofFloatColor(distortion.getPixelsRef().getColor(x, y),                                          ));
            
            //texCoords.push_back(ofVec2f(distortionSE.x * float(x) / float(CARTOGRAM_GRID_SIZE),
            //                            distortionSE.y * float(y) / float(CARTOGRAM_GRID_SIZE)));
            
            if(y > 0 && x > 0)
            {
                mesh.addTriangle((y-1) * (CARTOGRAM_GRID_SIZE+1) + x,
                                 (y-1) * (CARTOGRAM_GRID_SIZE+1) + x-1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x);
                
                mesh.addTriangle((y-1) * (CARTOGRAM_GRID_SIZE+1) + x-1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x-1,
                                 (y  ) * (CARTOGRAM_GRID_SIZE+1) + x);
            }
        }
        mesh.addVertices(vertices);
        mesh.addColors(colors);
    }
    
    shader.begin();
    shader.setUniform1i("derivative", 0);
    shader.end();
    
    
}

ofVec2f DistortedMap::derivativeAtScreenCoord(int x, int y)
{
    ofVec2f gridCoord ( CARTOGRAM_GRID_SIZE * float(x) / float(ofGetWidth()), CARTOGRAM_GRID_SIZE * float(y) / float(ofGetHeight()));
    //ofVec2f derivative( distortion.getColor(x, y) - distortion.getColor(x-1, y), distortion.getColor(x, y) - distortion.getColor(x-1, y) );
    return gridCoord;
}

Bounds<float> DistortedMap::screenBounds()
{
    return Bounds<float>(0, ofGetWidth(), 0, ofGetHeight());
}

Bounds<float> DistortedMap::gridBounds()
{
    return Bounds<float>(0, CARTOGRAM_GRID_SIZE+1, 0, CARTOGRAM_GRID_SIZE+1);
}

ofVec2f DistortedMap::lngLatToScreen(ofVec2f lngLat)
{
    ofVec2f undistortedNormalPos = DNS::Geometry::Normalize(lngLat, gMap.getLatLngBounds());
    
    
    ofVec2f distortionGridPos = DNS::Geometry::Map(lngLat, gMap.getLatLngBounds(), gridBounds());
    
    ofFloatColor gridDistortedPosSample = DNS::Image::Sample(distortion, distortionGridPos);
    ofVec2f normalDistortedPos(gridDistortedPosSample.r, gridDistortedPosSample.g);
    
    //ofVec2f normalDistortedPos = DNS::Geometry::Normalize(gridDistortedPos, gridBounds());
    
    ofVec2f normalInterpolatedPos = DNS::Geometry::InterpolateLinear(normalDistortedPos, undistortedNormalPos, distortionAmount);
    
     
    return DNS::Geometry::Map(normalInterpolatedPos, Bounds<float>::normalBounds(), screenBounds());
    //return DNS::Geometry::Map(lngLat, gMap.getLatLngBounds(), screenBounds());
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
    
    shader.setUniform1f("normalMouseX", distortionAmount);
    
    float aspect = gMap.map.width / gMap.map.height;
    float scaleX = float(ofGetWidth()) / float(CARTOGRAM_GRID_SIZE+1);
    float scaleY = float(ofGetHeight()) / float(CARTOGRAM_GRID_SIZE+1);
    
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glScalef(scaleX, scaleY, 1);
    
    mesh.draw();
    glPopMatrix();
    
    shader.end();
    
    drawPhotos();
}

void DistortedMap::drawPhotos()
{
    int numPhotos = 20;
    
    for(int p = 0; p < photos.size(); p += 1 /* photos.size() / numPhotos*/)
    {
        glPushMatrix();
            ofVec2f pos = lngLatToScreen( photos[p].lngLat() );
            ofTranslate(pos.x, pos.y);
            photos[p].draw();
        glPopMatrix();
    }
}