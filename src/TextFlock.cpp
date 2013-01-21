//
//  File.cpp
//  emptyExample
//
//  Created by Dimitrov Kamen on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "TextFlock.h"

TextFlock::TextFlock(string _str) {
    
    str = _str;

    opacity = 0;
        
    flock = Flock();

    textFbo.allocate(TEXT_RECT_W, TEXT_RECT_H);
}

void TextFlock::createFbo(ofFbo &fbo,ofxFTGLFont &font) {

    // create Fbo
    
    fbo.begin();
    ofBackground(255, 30, 30, 0);
    ofSetColor(255,255,255,255);
    font.drawString(str, (TEXT_RECT_W - TEXT_LAYOUT_WIDTH) / 2.0 ,80);
    fbo.end();
    fbo.readToPixels(fboPixels);
    fboImage.setFromPixels(fboPixels);
    
    fboPixels.clear();    
}



void TextFlock::createFlock(ofVec3f startpos) {
    
    status = FLOCK_INCOMING;
    
    flock.init();
    
    for(int x=0;x<fboImage.width;x++) {
        for(int y=0;y<fboImage.height;y++) {
            
            ofColor c = fboImage.getColor(x, y);
            
            if(c.a > 150 ) {
                
                
                ofVec3f pos = ofVec3f(x+startpos.x,y+startpos.y,0.0+startpos.z);
                            
                Boid *b = new Boid(pos,4.0 + ofRandomf()*4.0,4.0,2.0);
                b->c = c;
                
                float r = ofRandom(0,10);
                
                if(r < 2) {
                    b->drawLine = true;
                }
                
                flock.addBoid(b);    
            }
            
            
        }
    }
    
    fboImage.clear();
    
    flock.moveOffscreen();
}

void TextFlock::moveFlockOffscreen() {
    flock.moveSeekOffscreen();
}

void TextFlock::clear() {
    flock.clear();
}

bool TextFlock::isOffscreen() {
   
    if(opacity < 0.01 && status == FLOCK_OUTGOING) return true;
    
 //   if(flock.isOffscreen()) return true;
   
    return false;
}

void TextFlock::update() {
    
    if(status == FLOCK_INCOMING) {
        if(opacity < 1) {
            opacity += FADE_SPEED;
        } else {
            opacity = 1.0;
        }
    } else if(status == FLOCK_OUTGOING) {
       
        if(opacity > 0.01) {
            opacity -= FADE_SPEED;
        } else {
            opacity = 0;
        }
    }
    
   
    
    flock.seekPosition();
}

void TextFlock::draw(ofColor boidColor) {
   
    ofColor c = ofColor(boidColor);
    
    float pointOpacity = ofxTweenLite::tween(0,boidColor.a,opacity,OF_EASE_LINEAR_IN);
    float lineOpacity = ofxTweenLite::tween(0,60,opacity, OF_EASE_LINEAR_IN);
    
    ofPushStyle();
    ofMesh m;
   
    ofMesh lmesh;
    ofEnableSmoothing();
    ofSetLineWidth(0.5);
    
    m.setMode(OF_PRIMITIVE_POINTS);
    lmesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    
    for(int i=0;i<flock.getNumBoids(); i++) {
       
        ofVec3f bp = flock.getBoid(i)->loc;
               
        c.a = pointOpacity;
        m.addVertex(bp);
        m.addColor(c);
        
        if(flock.getBoid(i)->drawLine) {
            lmesh.addVertex(bp);
            c.a = lineOpacity;
            lmesh.addColor(c);
        }

        
    }
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
    
    
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(3.0);
     
    lmesh.drawFaces();
    m.drawFaces();
    ofPopStyle();
    
}



