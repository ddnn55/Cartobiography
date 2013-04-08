#include "testApp.h"

#include "CBUtil.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofEnableSmoothing();
    
    
    Bounds<float> latLngBounds;
    latLngBounds.left   = -88.298;
    latLngBounds.right  = 35.498;
    latLngBounds.bottom = 20.627166666666668;
    latLngBounds.top    = 52.312333333333335;
    
    myMap.load(latLngBounds, "../../../../../../data/photos-blur2-min0.001.distortion.1024.dat", "../../../../../../www/photos.json");
    
    ofSetWindowPosition(0, 0);
    //ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofSetWindowShape(myMap.gMap.map.width, myMap.gMap.map.height);
    
    gui.addLabel("'g' -- toggle GUI display");
    gui.addSlider("distortion", 0.0, 1.0, &myMap.distortionAmount);
    
    gui.addSpacer();
    gui.addToggle("wireframe", &wireframe); wireframe = false;
    gui.addToggle("derivative", &derivative); derivative = false;
    
    gui.addSpacer();
    //derivativeLabel = gui.addLabel("derivative: ");
    gui.addSlider("image size", 5.0, 100.0, &myMap.imageSize);
    gui.addToggle("render quadtree", &myMap.renderQuadTree);
    
    gui.autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void testApp::update(){
    //myMap.distortionAmount = float(ofGetMouseX()) / float(ofGetWidth());
    myMap.mousePos = ofVec2f(ofGetMouseX(), ofGetMouseY());
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
    string name = "derivative: " + vec2string(myMap.derivativeAtScreenCoord(x, y));
    //cout << name << endl;
    
    //myMap.distortionAmount = float(ofGetMouseX()) / float(ofGetWidth());
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    myMap.clicked = true;
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