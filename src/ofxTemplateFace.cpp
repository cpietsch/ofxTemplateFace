//
//  ofxTemplateFace.cpp
//  ofApp
//
//  Created by Christopher Pietsch on 21.04.13.
//  http://www.chrispie.com
//  https://github.com/cpietsch/ofxTemplateFace
//
//

#include "ofxTemplateFace.h"

using namespace ofxCv;
using namespace cv;


ofxTemplateFace::ofxTemplateFace()
:fileName("haarcascade_frontalface_alt2.xml")
,scaleFactor(0.25)
,counter(0)
,cascadeDetect(false)
,faceInit(false)
{}

void ofxTemplateFace::setup(){
    classifier.load(ofToDataPath(fileName));
}

vector<cv::Rect> compare(vector<cv::Rect> a, vector<cv::Rect>b){
    vector<cv::Rect> out;
    if(a.size()>0 && b.size()>0){
        
    } else {
        out = a;
    }
    return out;
}


void ofxTemplateFace::drawDebug(){
    
}

void ofxTemplateFace::update(){
    if(counter%500==0 && cascadeDetect){
        // todo: add new objects not overwrite or delete if confidence is high
        
        //vector<cv::Rect> oldObjects(objects);
        
        classifier.detectMultiScale(
                                    grayCv,
                                    newObjects,
                                    1.2,
                                    2,
                                    0);
        /*
        for(int i = 0; i < newObjects.size(); i++) {
            for(int e = 0; e < objects.size(); e++) {
                if(abs(objects[e].x-newObjects[i].x)>15 && abs(objects[e].y-newObjects[i].y)>15){
                    objects.push_back(newObjects[i]);
                } else {
                    //objects[i] = newObjects[i];
                }
            }
        }
        
        if(objects.size()==0){
            objects = newObjects;
        }
         */
        objects = newObjects;
        
        /*
        oldObjects.insert(oldObjects.end(), objects.begin(), objects.end());
        ofxCv::groupRectangles(oldObjects, 1,100);
        objects = oldObjects;
        */
        
        faces.erase(faces.begin(),faces.end());
        for(int i = 0; i < objects.size(); i++) {
            faces.push_back(getFaceMat(objects[i]));
            notFound.push_back(0);
            // todo: groupRect
            // todo: jedes face hat mehrere templates
        }
       
    } else {
        for(int i = 0; i < objects.size(); i++) {
            
            if(!retrack(&objects[i], faces[i], grayCv)){
                cout << "lost " << i << endl;
                notFound[i]++;
                if(notFound[i]>10){
                    objects.erase(objects.begin()+i);
                    faces.erase(faces.begin()+i);
                    notFound.erase(notFound.begin()+i);
                }
                
                // todo: counter < 4 dann erst dann löschen
            } else {
               faces[i] = getFaceMat(objects[i]); 
            }
            
            
        }
    }
    counter++;
}

void ofxTemplateFace::setImage(Mat image){
    resize(image, img, cv::Size(scaleFactor * image.cols, scaleFactor * image.rows));

    if(grayCv.cols!=img.cols && grayCv.rows != img.rows){
        grayCv.create(img.cols, img.rows, CV_8U);
    }
    convertColor(img, grayCv, CV_RGB2GRAY);
    equalizeHist(grayCv, grayCv);
    
    cascadeDetect = true;
}

void ofxTemplateFace::drawCascade(){
    /*
    for(int i = 0; i < objects.size(); i++) {
        objects[i].x /= scaleFactor;
        objects[i].y /= scaleFactor;
        objects[i].width /= scaleFactor;
        objects[i].height /= scaleFactor;
    }
    */
    
    for(int i = 0; i < objects.size(); i++) {
        ofRectangle r = toOf(objects[i]);
        r.x /= scaleFactor;
        r.y /= scaleFactor;
        r.width /= scaleFactor;
        r.height /= scaleFactor;
        
        ofSetColor(255, 255, 255);
        ofNoFill();
        ofRect(r);
    }
}
cv::Mat ofxTemplateFace::getFaceMat(cv::Rect position){
    cv::Mat tmp;
    //makePadding(&position, -10);
    tmp.create(position.width, position.height, CV_8U);
    tmp = grayCv(position).clone();
    return tmp;
}

void ofxTemplateFace::setFace(cv::Rect position){
    //cout << position.x << " " << position.y << " " << position.width << " " << position.height << endl;
    //makePadding(&position, -10);
    faceCv.create(position.width, position.height, CV_8U);
    faceCv = grayCv(position).clone();
    
    /*
    faceImg.allocate(faceCv.cols, faceCv.rows, OF_IMAGE_GRAYSCALE);
    resize(faceCv,faceImg);
    faceImg.update();
     */
    
    faceInit=true;
}

bool ofxTemplateFace::retrack(cv::Rect* R, cv::Mat tmpl, cv::Mat big){
    
    
    int w = big.rows - tmpl.rows + 1;
    int h = big.cols - tmpl.cols + 1;

    
    Mat res = Mat::zeros(w, h,CV_32F);
    IplImage _big = big;
    IplImage _tmpl = tmpl;
    IplImage _res = res;
    
    // here is where the magic happens, trying to find the templateface in the big image
    cvMatchTemplate(&_big, &_tmpl, &_res, CV_TM_SQDIFF);
    //cvNormalize(&_res, &_res);
    
    double minVal, maxVal;
    CvPoint minLoc, maxLoc;
    cvMinMaxLoc(&_res, &minVal, &maxVal, &minLoc, &maxLoc, 0);
    
   // cout << minVal << endl;
    
    R->x = minLoc.x;
    R->y = minLoc.y;
    
    // todo: dynamic value
    if(minVal<3.42484e+06){
        return true;
    } else {
        return false;
    }
    //cout << minVal << " " << maxVal << " " << maxLoc.x << maxLoc.y << endl;

}

void ofxTemplateFace::makePadding(cv::Rect* r,int p){
    r->width += 2*p;
    r->height += 2*p;
    r->x -= p;
    r->y -= p;
    if(r->x + r->width >= grayCv.cols){
        r->width = grayCv.cols - r->x;
    }
    if(r->y + r->height >= grayCv.rows){
        r->height = grayCv.rows - r->y;
    }
    if(r->x <= 0){
        r->x=1;
    }
    if(r->y <= 0){
        r->y=1;
    }
}


cv::Mat ofxTemplateFace::getSearchMat(cv::Rect position){
    cv::Mat tmp;
    makePadding(&position, 10);
    cout << grayCv.cols << " " << grayCv.rows << endl;
    cout << position.x << " " << position.y << " " << position.width << " " << position.height << endl;
    tmp.create(position.width, position.height, CV_8U);
    tmp = grayCv(position).clone();
    return tmp;
}