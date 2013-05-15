#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxTemplateFace.h"

class testApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

    ofVideoGrabber cam;
    ofPtr<ofQTKitGrabber>	qtCam;
    ofVideoPlayer player;
    
    cv::Mat camMat, mirrorMat;
    ofImage camImg;
    
    ofxTemplateFace templateFace;
    bool newFrame;
    bool testVideo;
};

