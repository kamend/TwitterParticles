//
//  Boid.h
//  4_BlondeBoids
//
//  Created by Kamen Dimitrov on 10/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __BlondeBoids_Boid_h
#define __BlondeBoids_Boid_h

#include "ofMain.h"

class Boid {
    
public:
    
    ofVec3f loc, lastLoc;
    ofVec3f vel;
    ofVec3f acc;
    ofVec3f initialPosition;
    ofVec3f seekPosition;
    
    bool drawLine;
    
    
    float angleOfMovement;
    
    float r;
    float maxforce; // Maximum steering force
    float maxspeed; // Maximum speed
    float minimumSeekDistance; // Minimum distance between current location and the seeking location
    bool affected;
    int affectedBy;
    
    // Alpha & Color
    int myAlpha;
    int myCol; 
    
    ofColor c;
    
    bool wrapped;
    
    Boid(ofVec3f l,float ms,float mf, float minDistance);
    void run(vector<Boid*> *boids);
    void flock(vector<Boid*> *boids);
    void update();
    void seek(ofVec3f target);
    void runaway(ofVec3f target);
    void arrive(ofVec3f target);
    ofVec3f steer(ofVec3f target, bool slowdown);
    void render();
    void borders();
    ofVec3f separate(vector<Boid*> *boids);
    ofVec3f align(vector<Boid*> *boids);
    ofVec3f cohesion(vector<Boid*> *boids);
    void desc();
    
};


#endif
