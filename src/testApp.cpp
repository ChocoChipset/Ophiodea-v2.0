#include "testApp.h"
#include <assert.h>
#include <math.h>
#include <time.h>



//int camindex[] = {1,2,4,5,0,3};
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

void testApp::calculateMovmementForShader()
{
	int position;
	float rSource,gSource,bSource;
    float rPrevious, gPrevious, bPrevious;
	float differenceInPixel;
	float grayPix;
	float grayPPix;
	amountOfMovement = 0;



        for (int i = 0; i < outputImageWidth*outputImageHeight*3; i+=6) {

			position = i;//3 * ((j*outputImageWidth) + i); //anImage.get(i, j);

            rSource = pixelsForOutput[position];
            gSource = pixelsForOutput[position + 1];
            bSource = pixelsForOutput[position + 2];

            rPrevious = previosPixelsForOutput[position];
            gPrevious = previosPixelsForOutput[position + 1];
            bPrevious = previosPixelsForOutput[position + 2];

			grayPix = .333*rSource + .333*gSource + .333*bSource;
			grayPPix = .333*rPrevious + .333*gPrevious + .333*bPrevious;
			differenceInPixel = abs(grayPix - grayPPix);
			if (differenceInPixel > kMOVEMENT_LOW_PASS_FILTER_CONSTANT*.25) amountOfMovement += differenceInPixel;

		}


	previosPixelsForOutput.setFromPixels(pixelsForOutput.getPixels(), outputImageWidth, outputImageHeight, kTHREE_CHANNELS);

	 for (int i = 0; i < outputImageWidth*outputImageHeight*3; i+=3)
	 {

        pixelsForOutput[i] = pixelsForOutput[i] + (amountOfMovement *.01);

	 }

}

void testApp::calculateAmountOfMovement()
{

	int position;

	for (int j = 0; j < outputImageHeight; ++j) {

        for (int i = 0; i < outputImageWidth; ++i) {

                position = 3 * ((j*outputImageWidth) + i);
                pixelsForOutput[position] = pixelsForOutput[position] + (amountOfMovement *.01);
        }
    }


}

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
    amountOfMovement = 0;
    float rSource,gSource,bSource;

    float rPrevious, gPrevious, bPrevious;
    int position, positionTarget;

    float differenceInPixel;

    ofPixels tempPixels;
    tempPixels.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);
    tempPixels.setFromPixels(pixelsForOutput.getPixels(), outputImageWidth, outputImageHeight, kTHREE_CHANNELS);
    pixelsForOutput.clear();
    pixelsForOutput.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

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

            position = 3 * ((j*outputImageWidth) + i); //anImage.get(i, j);

            rSource = tempPixels[position];
            gSource = tempPixels[position + 1];
            bSource = tempPixels[position + 2];

            // Calculate amount of movement here:

            rPrevious = previosPixelsForOutput[position];
            gPrevious = previosPixelsForOutput[position + 1];
            bPrevious = previosPixelsForOutput[position + 2];

            differenceInPixel = distanceBetweenTwoPoints(rSource, gSource, bSource, rPrevious, gPrevious, bPrevious);

            if (differenceInPixel > kMOVEMENT_LOW_PASS_FILTER_CONSTANT)
            {
                amountOfMovement += differenceInPixel;

            }
			/*
			float grayPix = .333*rSource + .333*gSource + .333*bSource;
			float grayPPix = .333*rPrevious + .333*gPrevious + .333*bPrevious;
			float differenceInPixel = abs(grayPix - grayPPix);
			if (differenceInPixel > kMOVEMENT_LOW_PASS_FILTER_CONSTANT) amountOfMovement += differenceInPixel*.5;
			*/
            positionTarget = 3 * ((jt*outputImageWidth) + it);

            pixelsForOutput[positionTarget] = rSource;
            pixelsForOutput[positionTarget + 1 ] = gSource;
            pixelsForOutput[positionTarget + 2 ] = bSource;




        }
    }

    previosPixelsForOutput.setFromPixels(pixelsForOutput.getPixels(), outputImageWidth, outputImageHeight, kTHREE_CHANNELS);
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
#ifdef DEMO_MODE
	videoGrabber[0].grabFrame();
	for( int i = 0; i < kNUMBER_OF_CAMERAS; ++i){
		ofPixels mirrorPix = videoGrabber[0].getPixelsRef();
		mirrorPix.mirror(1,0);

		if(i%2==0)
			pixelsFromCamera[i].setFromPixels(mirrorPix.getPixels(), kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, kTHREE_CHANNELS); // Place capture in ofPixels object
		else
			pixelsFromCamera[i].setFromPixels(videoGrabber[0].getPixels(), kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, kTHREE_CHANNELS); // Place capture in ofPixels object
	}
#else
	for( int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        videoGrabber[i].grabFrame();

       // if (videoGrabber[i].isFrameNew())
        {
            pixelsFromCamera[i].setFromPixels(videoGrabber[i].getPixels(), kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, kTHREE_CHANNELS); // Place capture in ofPixels object
        }



    }
#endif

}

void testApp::scheduleMotorStop()
{
    time_t now;

    time(&now);

    if (difftime(now, timeSinceLasqtActivation) > kMOTOR_ACTIVATION_INTERVAL && isMotorRunning)
    {
    printf("Stop Motor . Last Activation: %f. Last Pause: %f", timeSinceLastActivation, timeSinceLastPause);
        stopTheMotor();
    }
}

void testApp::stopTheMotor()
{
    serialManager.writeByte('X');
    serialManager.writeByte(1);
    serialManager.writeByte(0);
    time(&timeSinceLastPause);
    isMotorRunning = false;
}


void testApp::startTheMotor()       // Release the Kraken!!!
{
    time_t now;
    time(&now);

    if (difftime(now, timeSinceLastPause) > kMOTOR_ACTIVATION_PAUSE_INTERVAL && !isMotorRunning)
    {
        serialManager.writeByte('X');
        serialManager.writeByte(1);
        serialManager.writeByte(255);
        time(&timeSinceLastActivation);
        printf("StartMotor . Last Activation: %f. Last Pause: %f", timeSinceLastActivation, timeSinceLastPause);
        isMotorRunning = true;
    }

}

void testApp::renderWithShader(){

	ofImage outputImage;
	outputImage.setFromPixels(pixelsForOutput);

	// render output image to fbo
	fbo.begin();
	ofClear(0);
	outputImage.draw(0,0,outputImage.getWidth(),outputImage.getHeight());
	fbo.end();


	// render fbo with shader
	circleShader.begin();
		circleShader.setUniformTexture("src_tex",fbo.getTextureReference(),2);
		circleShader.setUniform1f("halfOutputImageWidth",halfOutputImageWidth);
		circleShader.setUniform1f("halfOutputImageHeight",halfOutputImageHeight);

		ofSetRectMode(OF_RECTMODE_CENTER);
		fbo.draw(ofGetWidth()*.5, ofGetHeight()*.5,ofGetHeight(),ofGetHeight());
		ofSetRectMode(OF_RECTMODE_CORNER);

	circleShader.end();

	// draw mask on top
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofEnableAlphaBlending();
	if(bUseMask) maskImg.draw(ofGetWidth()*.5, ofGetHeight()*.5);
	ofDisableAlphaBlending();
	ofSetRectMode(OF_RECTMODE_CORNER);


	ofDrawBitmapString( ofToString(ofGetFrameRate()), 10,10);
	ofDrawBitmapString( ofToString(amountOfMovement), 10,30);

}


void testApp::startNextCamera(){

	if(camsStarted < kNUMBER_OF_CAMERAS)
	{
		videoGrabber[camsStarted].setDeviceID(kNUMBER_OF_CAMERAS);
		videoGrabber[camsStarted].initGrabber(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT);
        camsStarted++;
	}

}

//--------------------------------------------------------------

// OF Methods:
//--------------------------------------------------------------

void testApp::exit()
{
    for( int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        videoGrabber[i].close();
    }

}

void testApp::setup()
{
    serialManager.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serialManager.getDeviceList();
	serialManager.setup("/dev/ttyACM0", 9600); //open the first device
    time(&timeSinceLastPause);
    timeSinceLastPause = timeSinceLastPause - kMOTOR_ACTIVATION_PAUSE_INTERVAL;
    //std::exit(1);
    //ofSetFrameRate(60);

    assert(kNUMBER_OF_CAMERAS % 2 == 0);    // We really need cameras in even numbers. Exit if not.

	camsStarted = kNUMBER_OF_CAMERAS;
    amountOfMovement = 0;

    halfTheNumberOfCameras = kNUMBER_OF_CAMERAS / 2;

    // Allocate Object to hold captured images
    for(int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        pixelsFromCamera[i].allocate(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT, OF_IMAGE_COLOR);
    }

#ifdef DEMO_MODE
	videoGrabber[0].initGrabber(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT);
#else
	for(int i = 0; i < kNUMBER_OF_CAMERAS; ++i)
    {
        videoGrabber[i].setDeviceID( i );
        videoGrabber[i].initGrabber(kCAPTURED_IMAGE_WIDTH, kCAPTURED_IMAGE_HEIGHT);
    }
#endif

    // Calculate Composite Image Output

    // Width of each image - overlaped space
    outputImageWidth = (kCAPTURED_IMAGE_WIDTH * halfTheNumberOfCameras) - (kMERGING_AREA_WIDTH * (kNUMBER_OF_CAMERAS-1));

    // Composite is composed by "two floors": Twice the height minus - merging area
    outputImageHeight= (kCAPTURED_IMAGE_HEIGHT * 2) - kMERGING_AREA_HEIGHT;

    imageWidthWithoutBlendingArea = kCAPTURED_IMAGE_WIDTH - kMERGING_AREA_WIDTH;
    imageHeightWithoutBlendingArea = kCAPTURED_IMAGE_HEIGHT- kMERGING_AREA_HEIGHT;

    pixelsForOutput.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    pixelsForBottomImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);
    pixelsForTopImage.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    previosPixelsForOutput.allocate(outputImageWidth, outputImageHeight, OF_IMAGE_COLOR);

    halfOutputImageHeight = outputImageHeight / 2;
    halfOutputImageWidth = outputImageWidth / 2;

   	bUseMask = true;
	maskImg.loadImage("mask.png");

	bUseShaderRender = true;
	fbo.allocate(outputImageWidth,outputImageHeight,GL_RGB);
	circleShader.load("circle.vert","circle.frag");

	ofBackground(0, 0, 0);  // Black Background



}

//--------------------------------------------------------------
void testApp::update()
{



		updateALLtheCaptures();

		placeCapturedImagesOnScreen();
		drawVerticalAlphaComposites();
		drawHorizontalAlphaComposites();

		if(!bUseShaderRender){
			applySphereTransformation();
			calculateAmountOfMovement();
		}else{
			calculateMovmementForShader();
		}

    //if (amountOfMovement > kAMOUNT_OF_MOVEMENT_THRESHOLD)
    {
        //startTheMotor();

    }
    //scheduleMotorStop();

}

//--------------------------------------------------------------
void testApp::draw()
{
   if(!bUseShaderRender)
	{

		ofImage outputImage;
		outputImage.setFromPixels(pixelsForOutput);


		ofSetRectMode(OF_RECTMODE_CENTER);

		ofEnableAlphaBlending();

		outputImage.draw(ofGetWidth()*.5, ofGetHeight()*.5, ofGetHeight(),ofGetHeight());

		if(bUseMask) maskImg.draw(ofGetWidth()*.5, ofGetHeight()*.5);
		ofDisableAlphaBlending();

		ofSetRectMode(OF_RECTMODE_CORNER);


		ofDrawBitmapString( ofToString(ofGetFrameRate()), 10,10);
		ofDrawBitmapString( ofToString(amountOfMovement), 10,30);

	}else{
		renderWithShader();
	}

}

double testApp::distanceBetweenTwoPoints(float x1, float y1, float z1, float x2, float y2, float z2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2) + pow((z2 - z1), 2));
	//return (pow((x2 - x1), 2) + pow((y2 - y1), 2) + pow((z2 - z1), 2));
}

// Interruption events
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 's') bUseShaderRender = !bUseShaderRender;
	else if(key == 'm') bUseMask = !bUseMask;

	else if (key == 'q') {
            startTheMotor();
	}
	else if (key == 'w') {
            scheduleMotorStop();
	}


}

//---------------------------------------------f-----------------
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
