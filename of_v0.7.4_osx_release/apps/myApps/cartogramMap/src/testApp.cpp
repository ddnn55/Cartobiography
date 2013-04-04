#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofEnableSmoothing();
    
    
    Bounds<float> latLngBounds;
    latLngBounds.left   = -88.298;
    latLngBounds.right  = 35.498;
    latLngBounds.bottom = 20.627166666666668;
    latLngBounds.top    = 52.312333333333335;
    
    myMap.load(latLngBounds, "../../../../../../data/photos.distortion.128.dat");
    
    gui.addLabel("'g' -- toggle GUI display");
    gui.addToggle("wireframe", &wireframe); wireframe = false;
    gui.addToggle("derivative", &derivative); derivative = false;
    gui.autoSizeToFitWidgets();
    
    
//    gui.setup("panel"); // most of the time you don't need a name
//	gui.add(filled.setup("bFill", true));
//	gui.add(radius.setup( "radius", 140, 10, 300 ));
//	gui.add(r.setup( "red", 100.0f, 0, 255 ));
//	gui.add(g.setup( "green", 100.0f, 0, 255 ));
//	gui.add(b.setup( "blue", 140.0f, 0, 255 ));
//	gui.add(circleResolution.setup("circle res", 5, 3, 90, true));
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    
    if(derivative)
        myMap.drawDerivative(0, 0);
    else if(wireframe)
        myMap.drawWireframe(0, 0);
    else
        myMap.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    static bool drawUI = true;
    switch(key)
    {
        case 'g': case 'G': gui.setAutoDraw(drawUI = !drawUI); break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}