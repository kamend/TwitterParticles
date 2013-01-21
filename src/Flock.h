//
//  Flock.h
//  4_BlondeBoids
//
//  Created by Kamen Dimitrov on 10/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __BlondeBoids_Flock_h
#define __BlondeBoids_Flock_h

#include "Boid.h"

class Flock {
public:    
    
    bool isAtInitialPosition;
    int minBoidsAtPosition;
    
    Flock() {
        
    }
    
    void init() {
        isAtInitialPosition = false;
        minBoidsAtPosition = 1000;
    }
    
    void run() {
        for(int i=0;i<boids.size();i++) {
       
            boids[i]->run(&boids);
        }
    }
    
    void moveSeekOffscreen() {
        
        for(int i=0;i<boids.size();i++) {
            boids[i]->maxspeed = 1.0;
            boids[i]->seekPosition = ofVec3f(ofRandom(-300,300),ofRandom(-300,300),ofRandom(-600,600));
        }
    }
    
    void moveSeekInitial() {
        for(int i=0;i<boids.size();i++) {
            boids[i]->seekPosition = boids[i]->initialPosition;
        }        
    }
    
    
    bool isBoidOffscreen(ofVec3f pos) {
       // if(pos.x > ofGetWidth() || pos.x < 0 || pos.y > ofGetHeight() || pos.y < 0 || pos.z > 500) 
        if(pos.z > 500) 
            return true;
        
        return false;
    }
    
    bool isOffscreen() {
        
        int numOffscreen = 0;
        int offscreenLimit = 10;
            
        for(int i=0;i<boids.size();i++) {
            if(isBoidOffscreen(boids[i]->loc)) numOffscreen++;
        }
               
        
        if(boids.size() - numOffscreen < 100) {
            return true;
        }
                
        return false;
        
    }
    
    bool seekPosition() {
    
        int atPosition=0;
        
        for(int i=0;i<boids.size();i++) {
            if(boids[i]->loc.distance(boids[i]->seekPosition) < boids[i]->minimumSeekDistance) {
                boids[i]->loc = boids[i]->seekPosition;
                atPosition++;
            } else {
                boids[i]->maxspeed = 2.0;
                boids[i]->seek(boids[i]->seekPosition);
                
                boids[i]->runaway(boids[i]->loc * cos(ofRandomf()*2)*2);
                boids[i]->update();
            }
            
        }
        
        isAtInitialPosition = false;
        if(abs(atPosition - (int)boids.size()) < minBoidsAtPosition) 
        {
            isAtInitialPosition = true;
        }       
    }
    
    void fxFlockRunaway(ofVec3f from, float radius) {
        for(int i=0;i<boids.size();i++) {   
            if(boids[i]->loc.distance(from) < radius) {
                boids[i]->runaway(from);
                boids[i]->update();
            }
        }

    }
    
    
    void clear() {
        
        vector<Boid*>::iterator itr = boids.begin();
        
        while(itr != boids.end()) {
            delete *itr;
            itr = boids.erase(itr);
        }
        
    }
    
    int getNumBoids() {
        return boids.size();
    }
    
    Boid* getBoid(int index) {
        return boids.at(index);
    }
    
    
    void addBoid(Boid *b) {
        boids.push_back(b);
    }
    
    void drawBoidsAsPoints() {
        ofMesh m;
        
        m.setMode(OF_PRIMITIVE_POINTS);
        for(int i=0;i<boids.size(); i++) {
            m.addColor(boids[i]->c);
            m.addVertex(boids[i]->loc);
            
        }
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
    
        
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glDisable(GL_ALPHA_TEST);

        glPointSize(1.0);
        m.drawFaces();
        
    }
    
    void moveOffscreen() {
        for(int i=0;i<boids.size();i++) {
            boids[i]->loc = ofVec3f(ofRandom(-300,300),ofRandom(-300,300),ofRandom(-300,300));
        }
    }
    
    

    void draw() {
        ofPushStyle();
        ofFill();
        ofSetColor(255, 0, 0,50);
        for(int i=0;i<boids.size();i++) {
            ofVec3f l = boids[i]->loc;

            ofCircle(l.x,l.y,5);
        }
        ofPopStyle();
    }

    vector<Boid*> boids;
    
};


#endif
