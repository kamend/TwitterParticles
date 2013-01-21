#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "TwitterDelegate.h"
#include "TextFlock.h"
#include "ofxFX.h"
#include "ofxTweenLite.h"

class testApp : public ofBaseApp, TwitterDelegate {
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
    
    // Twitter
    
    ofxTwitter twitter;
    void  searchResult(vector<Tweet> results, int queryIdentifier);
    vector<Tweet> tweets;
    vector<TextFlock*> flocks;
    TextFlock *randomText;
    
    ofFbo textFbo;
    ofxFTGLFont font;
    
    bool newTweet;
    
    ofVec3f enemyPos;
    
    ofxGlow  glow;
    ofxBlur blur;
    
    ofFbo bloomTex;
    
    int screenWidth;
    int screenHeight;

    float randomTextOpacity;
     

 
};
