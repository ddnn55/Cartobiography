#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    
    
    Bounds<float> latLngBounds;
    latLngBounds.left   = -88.298;
    latLngBounds.right  = 35.498;
    latLngBounds.bottom = 20.627166666666668;
    latLngBounds.top    = 52.312333333333335;
    
    myMap.load(latLngBounds, "photos_cart.dat");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    myMap.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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