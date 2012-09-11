#include "testApp.h"
#include <assert.h>

// Our own functions
//--------------------------------------------------------------
/*
void testApp::makeLookUpTable()
{
	float x,y;
	float xt,yt;
	int it,jt;

	int h = imgout.getHeight();
	int w = imgout.getWidth();

	for (int j=0;j<h;j++) {
		for (int i=0;i<w;i++) {

			x = (i-ic)/rw;
			y = (j-jc)/rh;

			xt = x*sqrt(1-0.5*y*y);
			yt = y*sqrt(1-0.5*x*x);

			it = int(rw*xt + ic);
			jt = int(rh*yt + jc);

			//c = imgout.pixels[i+j*imgout.width];//get(i,j);
			//imgcircle.pixels[it+jt*imgcircle.width] = c;//set(it,jt,c);
			lookUpCircleX[i][j] = it;
			lookUpCircleY[i][j] = jt;
		}

	}
*/
void testApp::drawHorizontalAlphaComposites()
{
    ofColor ca,cb,ct;
	float ra,ga,ba,rb,gb,bb,rt,gt,bt;
    float alphaChannel, alphaChannelCompliment;

    int startPositionY = (kCAPTURED_IMAGE_HEIGHT - kMERGING_AREA_HEIGHT);

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

void testApp::drawVerticalAlphaComposites()
{
    ofColor ca,cb,ct;
	float ra,ga,ba,rb,gb,bb,rt,gt,bt;
    float alphaChannel, alphaChannelCompliment;

    for (int k = 1; k < halfTheNumberOfCameras; ++k)
    {
        for (int i = 0; i < kMERGING_AREA_WIDTH; ++i)
        {
            int startPositionX = (kCAPTURED_IMAGE_WIDTH - kMERGING_AREA_WIDTH) * k;

            alphaChannel =  i / float(kMERGING_AREA_WIDTH-1);
            alphaChannelCompliment = 1 - alphaChannel;

            for (int j=0; j < kCAPTURED_IMAGE_HEIGHT; ++j)
            {
                // For Top Image
				int p = ((kCAPTURED_IMAGE_WIDTH-kMERGING_AREA_WIDTH+i)+(j*kCAPTURED_IMAGE_WIDTH)) * 3;
				ra = pixelsFromCamera[k-1][p];
				ga = pixelsFromCamera[k-1][p+1];
				ba = pixelsFromCamera[k-1][p+2];

				int pb = (i + (j * kCAPTURED_IMAGE_WIDTH)) * 3;
				rb = pixelsFromCamera[k][pb];
				gb = pixelsFromCamera[k][pb+1];
				bb = pixelsFromCamera[k][pb+2];

				rt = alphaChannelCompliment * ra + alphaChannel * rb;
				gt = alphaChannelCompliment * ga + alphaChannel * gb;
				bt = alphaChannelCompliment * ba + alphaChannel * bb;

				int pf = 3 * ( (startPositionX+i)+(outputImageWidth*j) );
				pixelsForTopImage[pf] = rt;
				pixelsForTopImage[pf+1] = gt;
				pixelsForTopImage[pf+2] = bt;

                // For Bottom Image
				p = ((kCAPTURED_IMAGE_WIDTH-kMERGING_AREA_WIDTH+i)+(j*kCAPTURED_IMAGE_WIDTH)) * 3;
				ra = pixelsFromCamera[k-1 + halfTheNumberOfCameras][p];
				ga = pixelsFromCamera[k-1 + halfTheNumberOfCameras][p+1];
				ba = pixelsFromCamera[k-1 + halfTheNumberOfCameras][p+2];

				pb = (i + (j * kCAPTURED_IMAGE_WIDTH)) * 3;
				rb = pixelsFromCamera[k + halfTheNumberOfCameras][pb];
				gb = pixelsFromCamera[k + halfTheNumberOfCameras][pb+1];
				bb = pixelsFromCamera[k + halfTheNumberOfCameras][pb+2];

				rt = alphaChannelCompliment * ra + alphaChannel * rb;
				gt = alphaChannelCompliment * ga + alphaChannel * gb;
				bt = alphaChannelCompliment * ba + alphaChannel * bb;

				pf = 3 * ( (startPositionX+i)+(outputImageWidth*j) );
				pixelsForBottomImage[pf] = rt;
				pixelsForBottomImage[pf+1] = gt;
				pixelsForBottomImage[pf+2] = bt;



            }

        }



    }

    pixelsForTopImage.pasteInto(pixelsForOutput, 0, 0);
    pixelsForBottomImage.pasteInto(pixelsForOutput, 0, imageHeightWithoutBlendingArea);

}


void testApp::applySphereTransformation()
{
    float rSource,gSource,bSource;
    float rTarget,gTarget,bTarget;
    int position, positionTarget;

    ofPixels tempPixels;
    tempPixels.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);
    tempPixels.setFromPixels(pixelsForOutput.getPixels(), outputImageWidth, outputImageHeight, kTHREE_CHANNELS);


    float x, y;
    float xt, yt;
    int it, jt;

    for (int j = 0; j < outputImageHeight; ++j) {

        for (int i = 0; i < outputImageWidth; ++i) {

            x = (i - halfOutputImageWidth) / halfOutputImageWidth;
            y = (j - halfOutputImageHeight) / halfOutputImageHeight;

            xt = x * sqrt(1 - 0.5 * y * y);
            yt = y * sqrt(1 - 0.5 * x * x);

            it = int(halfOutputImageWidth * xt + halfOutputImageWidth);
            jt = int(halfOutputImageHeight * yt + halfOutputImageHeight);

            position= 3 * (j*outputImageWidth) + i; //anImage.get(i, j);

            rSource = tempPixels[position];
            gSource = tempPixels[position + 1];
            bSource = tempPixels[position + 2];

            positionTarget = (jt*outputImageWidth) + it;

            pixelsForOutput[positionTarget] = rSource;
            pixelsForOutput[positionTarget + 1 ] = gSource;
            pixelsForOutput[positionTarget + 2 ] = bSource;

    }
  }
}


void testApp::placeCapturedImagesOnScreen()
{
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

    imageWidthWithoutBlendingArea = kCAPTURED_IMAGE_WIDTH - kMERGING_AREA_WIDTH;
    imageHeightWithoutBlendingArea = kCAPTURED_IMAGE_HEIGHT- kMERGING_AREA_HEIGHT;

    pixelsForOutput.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    pixelsForBottomImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);
    pixelsForTopImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    halfOutputImageHeight = outputImageHeight / 2;
    halfOutputImageWidth = outputImageWidth / 2;

    ofBackground(0, 0, 0);  // Black Background


}

//--------------------------------------------------------------
void testApp::update()
{
    updateALLtheCaptures();

    placeCapturedImagesOnScreen();
    drawVerticalAlphaComposites();
    drawHorizontalAlphaComposites();
    applySphereTransformation();

}

//--------------------------------------------------------------
void testApp::draw()
{
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
