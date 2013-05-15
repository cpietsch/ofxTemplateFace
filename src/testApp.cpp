#include "testApp.h"

using namespace ofxCv;
using namespace cv;

static ofPoint fullHD = ofPoint(1920,1080);
static ofPoint halfHD = ofPoint(1280,720);

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetWindowShape(1280, 720);
    
    testVideo=false;
        
    if(testVideo){
        player.close();
        player.loadMovie("test.mp4");
        player.setVolume(0);
        player.play();
    } else {
        cam.close();
        qtCam = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
        cam.setGrabber(qtCam);
        qtCam->setVideoDeviceID(0);
        qtCam->setUseAudio(false);
        cam.initGrabber(halfHD.x, halfHD.y);
    }
    
    templateFace.setup();
    
}

//--------------------------------------------------------------
void testApp::update()
{
    newFrame = false;
    
    if(testVideo){
        player.update();
        newFrame = player.isFrameNew();
    } else {
        cam.update();
        newFrame = qtCam->hasPreview();
    }
    
    if(newFrame) {
        if(testVideo){
            camMat = toCv(player);
        } else {
            camMat = toCv(cam);
        }
    
        cv::flip(camMat, mirrorMat, 1);
        toOf(mirrorMat,camImg);
        camImg.update();
    
        templateFace.setImage(mirrorMat);
         
	}
    templateFace.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(100,100,100);

	ofSetHexColor(0xffffff);
    camImg.draw(0,0);
	
    templateFace.drawCascade();

    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()),20,20);
    
  //  resultImg.draw(0, 0);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
		case ' ':
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{ 
}
