//
//  TextFlock.h
//  emptyExample
//
//  Created by Dimitrov Kamen on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "Flock.h"
#include "ofxTweenLite.h"
#include "ofxFTGLFont.h"


#define NUM_PARTICLES 1000

#define TEXT_RECT_W 600
#define TEXT_RECT_H 400
#define TEXT_LAYOUT_WIDTH 512

#define FADE_SPEED 0.005;


enum TextFlock_Status {
    FLOCK_INCOMING,
    FLOCK_OUTGOING
};

class TextFlock {
public:
    TextFlock(string _str);
       
    void createFbo(ofFbo &fbo, ofxFTGLFont &font);
    void createFlock(ofVec3f startpos);
    void moveFlockOffscreen();
    void update();
    void draw(ofColor boidColor);
    void clear();
    bool isOffscreen();
    
    bool status;
    float opacity;
    
    Flock flock;
private:
    string str;

 
    
    ofFbo textFbo;
    ofPixels fboPixels;
    ofImage fboImage;
    
    
};
