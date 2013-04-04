#pragma once

#include "ofMain.h"

#define OFX_UI_NO_XML
#include "ofxUI.h"

#include "DistortedMap.h"
#include "GoogleMap.h"

class testApp : public ofBaseApp{
    
//    ofSlider radius;
//    ofSlider r, g, b;
//    ofSlider circleResolution;
//    ofToggle filled;
//    ofPanel gui;
    
    DistortedMap myMap;
    
    bool wireframe, derivative;
    
    ofxUICanvas gui;
    ofxUIRadio* displayMode;
    
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
