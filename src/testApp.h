#pragma once

#include "ofMain.h"
#include "OphiodeaConstants.h"


class testApp : public ofBaseApp{
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



        // Our own variables

        ofVideoGrabber videoGrabber[kNUMBER_OF_CAMERAS];
        ofPixels pixelsFromCamera[kNUMBER_OF_CAMERAS];
        ofPixels pixelsForOutput;
        ofPixels pixelsForTopImage;
        ofPixels pixelsForBottomImage;


        int halfTheNumberOfCameras;

        int outputImageWidth;
        int outputImageHeight;
        int imageWidthWithoutBlendingArea;
        int imageHeightWithoutBlendingArea;
		// Our own functions

        void updateALLtheCaptures();
        void placeCapturedImagesOnScreen();
        void drawVerticalAlphaComposites();
        void drawHorizontalAlphaComposites();
        void applySphereTransformation();
};
