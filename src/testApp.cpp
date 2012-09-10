#include "testApp.h"
#include <assert.h>

// Our own functions
//--------------------------------------------------------------

void testApp::drawHorizontalAlphaComposites()
{
    ofColor ca,cb,ct;
	float ra,ga,ba,rb,gb,bb,rt,gt,bt;

    int startPositionY = (kCAPTURED_IMAGE_HEIGHT - kMERGING_AREA_HEIGHT);

    float alphaChannel, alphaChannelCompliment;
    int currentImageIndex;

    for (int i=0; i< kMERGING_AREA_HEIGHT; ++i)
    {
        alphaChannel =  i / float(kMERGING_AREA_HEIGHT-1);
        alphaChannelCompliment = 1 - alphaChannel;

        for (int j=0; j < outputImageWidth; ++j)
        {

            int p = 3 * (((startPositionY + i) * outputImageWidth) + j);  //(i + (kCAPTURED_IMAGE_HEIGHT-kMERGING_AREA_HEIGHT+j) * outputImageWidth);
			ra = pixelsForTopImage[p];
			ga = pixelsForTopImage[p+1];
			ba = pixelsForTopImage[p+2];

			int pa = 3 * (j + (i * outputImageWidth));
			rb = pixelsForBottomImage[pa];
			gb = pixelsForBottomImage[pa+1];
			bb = pixelsForBottomImage[pa+2];

			rt = alphaChannelCompliment*ra + alphaChannel*rb;
			gt = alphaChannelCompliment*ga + alphaChannel*gb;
			bt = alphaChannelCompliment*ba + alphaChannel*bb;

			int pf = 3 * (j + ((startPositionY+i) * outputImageWidth));

			pixelsForOutput[pf] = rt;
			pixelsForOutput[pf+1] = gt;

			pixelsForOutput[pf+2] = bt;






        }

    }


}

void testApp::placeCapturedImagesOnScreen()
{
    int imageWidthWithoutBlendingArea = kCAPTURED_IMAGE_WIDTH - kMERGING_AREA_WIDTH;
    int imageHeightWithoutBlendingArea = kCAPTURED_IMAGE_HEIGHT- kMERGING_AREA_HEIGHT;

    for (int j = 0; j < halfTheNumberOfCameras; ++j)
    {
        pixelsFromCamera[j].pasteInto(pixelsForTopImage, j * imageWidthWithoutBlendingArea, 0);
        pixelsFromCamera[j+halfTheNumberOfCameras].pasteInto(pixelsForBottomImage, j * imageWidthWithoutBlendingArea, 0);
    }

    pixelsForTopImage.pasteInto(pixelsForOutput, 0, 0);
    pixelsForBottomImage.pasteInto(pixelsForOutput, 0, imageHeightWithoutBlendingArea);
}

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
    assert(kNUMBER_OF_CAMERAS % 2 == 0);    // We really need cameras in even numbers. Exit if not.

    halfTheNumberOfCameras = kNUMBER_OF_CAMERAS / 2;

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

    // Calculate Composite Image Output

    // Width of each image - overlaped space
    outputImageWidth = (kCAPTURED_IMAGE_WIDTH * halfTheNumberOfCameras) - (kMERGING_AREA_WIDTH * (kNUMBER_OF_CAMERAS-1));

    // Composite is composed by "two floors": Twice the height minus - merging area
    outputImageHeight= (kCAPTURED_IMAGE_HEIGHT * 2) -kMERGING_AREA_HEIGHT;

    pixelsForOutput.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    pixelsForBottomImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);
    pixelsForTopImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    ofBackground(0, 0, 0);  // Black Background


}

//--------------------------------------------------------------
void testApp::update()
{
    updateALLtheCaptures();



}

//--------------------------------------------------------------
void testApp::draw()
{
    placeCapturedImagesOnScreen();
    //drawHorizontalAlphaComposites();

    ofImage outputImage;
	outputImage.setFromPixels(pixelsForOutput);
    //ofSetRectMode(OF_RECTMODE_CENTER);
    outputImage.draw(0, 0,ofGetHeight(),ofGetHeight());

    ofDrawBitmapString( ofToString(ofGetFrameRate()), 10,10);
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
