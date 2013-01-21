#include "testApp.h"

FTSimpleLayout simpleLayout;
FTLayout *layouts[] = { &simpleLayout, NULL };


static int downloadTweetsOnEvery = 700; // 3 sec



//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(30);
    twitter.setup();
    twitter.setSearchDelegate(this);
    
    newTweet = false;
    
    
    
    font.loadFont("Arial.ttf",32, false);  
    
    font.setUseLayout(true);
    font.setLayoutLength(512.0);
    font.setLayoutAlignment(FTGL::ALIGN_CENTER);  
    
    enemyPos = ofVec3f(0,400,0);
    
     
    glow.setRadius(15.0);
    blur.setRadius(4.0);
    
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
    
    textFbo.allocate(screenWidth, screenHeight);
    
    ofLog() << "Screen w/h: " << screenWidth << "," << screenHeight;
    
    glow.allocate(screenWidth, screenHeight);
    blur.allocate(screenWidth, screenHeight);
    bloomTex.allocate(screenWidth, screenHeight);
    
    randomText = new TextFlock("test test sfsa asdasd sdad asd asd as das das d asd as d as das das d as");
    randomText->createFbo(textFbo, font);
    randomText->createFlock(ofVec3f(-TEXT_RECT_W/2,-TEXT_RECT_H/2,0));
    randomText->flock.moveSeekOffscreen();
    
    randomTextOpacity = 0;

}



void testApp::searchResult(vector<Tweet> results, int queryIdentifier) {

    for(int i=0;i < results.size(); i++) {
        Tweet tw = results[i]; // go through all tweets and see if the ones are added
        
        bool found = false;
        
        for(int t=0;t<tweets.size();t++) {
            if(tweets[t].id == tw.id) { found = true; break; }
        }
        
        if(!found) {
            tweets.push_back(tw);
            newTweet = true;
        }
    }
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // check for new tweets
    
    if(newTweet) {
        
        Tweet tw = tweets[tweets.size()-1];
        
        TextFlock *tFlock = new TextFlock(tw.title);
        tFlock->createFbo(textFbo, font);
        tFlock->createFlock(ofVec3f(-TEXT_RECT_W/2,-TEXT_RECT_H/2,0));
        tFlock->flock.moveSeekInitial();
        
        flocks.push_back(tFlock);
        
        // check all other flocks to outgoing

        if(flocks.size() > 1) {
            for(int i=0;i < flocks.size()-1; i++) {
                flocks[i]->status = FLOCK_OUTGOING;
                flocks[i]->flock.moveSeekOffscreen();
            }
        }
        
        newTweet = false;
    
    }
    
    
    vector<TextFlock*>::iterator itr = flocks.begin();
    
    while(itr != flocks.end()) {
    
        if(((*itr)->status == FLOCK_OUTGOING) && (ofGetFrameNum() % 200 == 0)) {
            ofLog() << "Check if flock is offscreen";
            if((*itr)->isOffscreen()) {
                ofLog() << "Flock is offscreen, deleting..";
                (*itr)->clear();
                delete *itr;
                itr = flocks.erase(itr);
            } else {
                ofLog() << "Flock is still inscreen";
                 (*itr)->update();
                itr++;
            }
        } else {
          
            (*itr)->update();
            itr++;
        }
    }
    
    for(int i=0;i<flocks.size();i++) {
        
       ofVec3f realPos = ofVec3f(enemyPos.x - (ofGetWidth()-500.0)/2.0, enemyPos.y-200,ofRandom(-50,50));
        
        flocks[i]->flock.fxFlockRunaway(realPos, 100);
        
        
    }
    
    // twitter query
    if(ofGetFrameNum() % downloadTweetsOnEvery == 0) {
         twitter.startTwitterQuery("apple");
    }
    
    bloomTex.begin();
    ofPushMatrix();
    ofTranslate(screenWidth/2,screenHeight/2);
    ofRotateY(ofGetFrameNum());
    ofBackground(10, 10, 20,255);
    ofEnableAlphaBlending();
    randomText->opacity = 1.0;
    randomText->draw(ofColor(250,150,150,ofxTweenLite::tween(100, 150, sin(ofDegToRad(randomTextOpacity)), OF_EASE_LINEAR_IN)));
  
    ofSetColor(160,160,160, 100);
       
    for(int i=0;i<flocks.size();i++) {
        flocks[i]->draw(ofColor(150,255,150,240));
    }
    ofPopMatrix();
    
    
    
    bloomTex.end();
 
    blur.setTexture(bloomTex.getTextureReference());
    
    blur >> glow;
    
   
    blur.update();
    glow.update();
      
    randomTextOpacity += 4;
    
    if(randomTextOpacity >=180) {
        randomTextOpacity = 0;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDisableAlphaBlending();
    ofBackground(20, 20, 40,255);
    ofPushMatrix();
    glow.draw();
    ofPopMatrix();
   
   
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 't') {
        twitter.startTwitterQuery("apple");
    }

    
    if(key == 'f') {
        
        ofToggleFullscreen();
        screenWidth = ofGetWidth();
        screenHeight = ofGetHeight();

        
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    enemyPos.x = x;
    enemyPos.y = y;
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