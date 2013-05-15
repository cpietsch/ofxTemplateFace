//
//  ofxTemplateFace.h
//  ofApp
//
//  Created by Christopher Pietsch on 21.04.13.
//  http://www.chrispie.com
//  https://github.com/cpietsch/ofxTemplateFace
//
//
#pragma once

#include "ofxCv.h"

class ofxTemplateFace {
public:
    float scaleFactor;
    string fileName;
    int counter;
    bool cascadeDetect,faceInit;
    
    vector<cv::Rect> objects,all;
    vector<cv::Rect> newObjects;
    vector<cv::Mat> faces,search;
    vector<int> notFound;
    
    
    cv::Rect pos;
    
    ofImage camImg, faceImg;
    
    cv::Mat img, grayCv, faceCv;
    
    cv::CascadeClassifier classifier;
    
    ofRectangle subjectFrame;
    bool subjectIsDefined;
    bool isSelectingTrackingRegion;
    ofPoint subjectLocation;
    
    ofxTemplateFace();
    void setup();
    void drawDebug();
    void setImage(cv::Mat img);
    void update();
    bool retrack(cv::Rect* R, cv::Mat tmpl, cv::Mat big);
    
    void drawCascade();
    void setFace(cv::Rect position);
    cv::Mat getFaceMat(cv::Rect position);
    cv::Mat getSearchMat(cv::Rect position);
    void makePadding(cv::Rect* r,int p);
    vector<cv::Rect> compare(vector<cv::Rect> a, vector<cv::Rect>b);

};

