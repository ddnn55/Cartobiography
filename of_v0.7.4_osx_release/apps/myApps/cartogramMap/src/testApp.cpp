#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    
    float left = -88.298,
         right = 35.498,
        bottom = 20.627166666666668,
           top = 52.312333333333335;
    
    gMap.setLatLngBounds(Bounds<float>(left, right, top, bottom));
    gMap.setPixelSize(1024, 1024);
    
    distortion.load("/Users/dstolarsky3/Desktop/Cartobiography/photos_cart.dat");
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    distortion.map.draw(400, 0);
    gMap.draw(0, 0);
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