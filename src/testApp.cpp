#include "testApp.h"

// Our own functions
//--------------------------------------------------------------
void testApp::updateALLtheCaptures()  // "X all the Y" pun intended
{
    for( int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        videoGrabber[i].grabFrame();
        pixelsFromCamera[i].setFromPixels(videoGrabber[i].getPixels(), kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, kTHREE_CHANNELS); // Place capture in ofPixels object
    }
}
//--------------------------------------------------------------

// OF Methods:
//--------------------------------------------------------------
void testApp::setup()
{
    // Allocate Object to hold captured images
    for(int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        pixelsFromCamera[i].allocate(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, OF_IMAGE_COLOR);
    }

    // Initialize Video Grabbers
    for(int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        videoGrabber[i].setDeviceID(i);
        videoGrabber[i].initGrabber(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT);
    }

    ofBackground(0, 0, 0);  // Black Background


}

//--------------------------------------------------------------
void testApp::update()
{
    updateALLtheCaptures();

}

//--------------------------------------------------------------
void testApp::draw(){

}

// Interruption events
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
