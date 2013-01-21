//
//  Boid.cpp
//  4_BlondeBoids
//
//  Created by Kamen Dimitrov on 10/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Boid.h"

const int width = 640;
const int height = 480;


Boid::Boid(ofVec3f l,float ms,float mf,float minDistance=0.05) {
    
    wrapped = false;
    
    drawLine = false;
         
    acc = ofVec3f(0,0,0);
    
    minimumSeekDistance = minDistance;
    
    seekPosition = ofVec3f(0,0,0);
    
    vel = ofVec3f(-abs(ofRandomf()),abs(ofRandomf()),0);
    
    initialPosition = l;
   initialPosition.z = ofRandom(-5,5);
    loc = l;
    lastLoc = l;
    
    r = 50.0;
    
    maxforce = mf;
    maxspeed = ms;
        
    myAlpha = int(ofRandom(155));
    
    c = ofColor(0,ofRandom(255),0);
        
    desc();
    
}

//----------------------------------

void Boid::desc() {
 //   ofLog() << "Location: " << loc << " Max Force: " << maxforce << "Max Speed: " << maxspeed << "Acceleration: " << acc << "Velocity:" << vel << endl;
}

//----------------------------------

void Boid::run(vector<Boid*> *boids) {
    flock(boids);
    update();
    borders();
    render();
}

//----------------------------------
// We accumulate a new acceleration each time based on three rules
void Boid::flock(vector<Boid*> *boids) {
   ofVec3f sep = separate(boids); // 1) separation
   ofVec3f ali = align(boids); // 2) allignment
   ofVec3f coh = cohesion(boids); // 3) cohesion
    
   sep *= 2.0;
   ali *= 1.0;
   coh *= 1.0;
        
//    seek(initialPosition);
    
}
//----------------------------------
void Boid::update(){
    
    vel += acc;
    vel.limit(maxspeed);
    loc += vel;
  
    acc.set(0,0,0);
}
//----------------------------------
void Boid::seek(ofVec3f target){
    
  //  if((target-seekPosition).length() < minimumSeekDistance) return;
    
    acc += steer(target,false);
}

void Boid::runaway(ofVec3f target) {
    acc -= steer(target,false);
}
//----------------------------------
void Boid::arrive(ofVec3f target) {
    acc += steer(target,true);
}
//----------------------------------
// A method that calculates a steering vector towards a target
// Takes a second argument, if true, it slows down as it approaches the target
ofVec3f Boid::steer(ofVec3f target, bool slowdown){
    ofVec3f steer;
    
    ofVec3f desired = target - loc;
    
    float d = desired.length();
    
    if(d > 0) {
        
        desired.normalize();
        
        if((slowdown) && (d < 100.0)) desired *= maxspeed * (d/100.0);
        else desired *= maxspeed;
         
        // Steering = Desired minus Velocity
        steer = desired - vel;
        steer.limit(maxforce);
        
    } else {
        steer = ofVec3f(0,0,0);
    }
    
    return steer;
}
//----------------------------------
void Boid::render() {
    return;
    //ofSetColor(myCol,myCol,myCol,myAlpha);
    
    ofSetColor(c,myAlpha);
    
    
    if(!wrapped)  {
      //   ofLine(loc.x,loc.y,lastLoc.x,lastLoc.y);
        ofNoFill();
        ofRect(loc.x,loc.y,3,3);
    }
    
    lastLoc = loc;
    
}
//----------------------------------

// Wraparound
// if the boid reach the end of the scree, send through the other side

void Boid::borders() {
    
    wrapped = false;
    
    if(loc.x < -r){
        loc.x = ofGetWidth()  + r;
        wrapped = true;
    }
    
    if(loc.y < -r) {
        loc.y = ofGetHeight() + r;
        wrapped = true;
    }
    
    if(loc.x > ofGetWidth() + r) {
        loc.x = -r;
        wrapped = true;
    }
    
    if(loc.y > ofGetHeight() + r) {
        loc.y = -r;
        wrapped = true;
    }
    
}
//----------------------------------
// Separation
// Method checks for nearby boids and steers away

ofVec3f Boid::separate(vector<Boid*> *boids) {
    float desiredSeparation = 25.0;
    
    ofVec3f sum = ofVec3f(0,0,0);
    
    int count = 0;
    for(int i=0;i<boids->size();i++) {
        Boid *other = boids->at(i);
        
        float d = loc.distance(other->loc);
        
        // if the boids are close
        if((d > 0) && (d < desiredSeparation)) {
            ofVec3f diff = loc - other->loc;
            diff.normalize();
            diff /= d;
            sum += diff;
            count++;
        }
       
    }
    
    if(count > 0){
        sum /= count;
    }
    
    return sum;
}
//----------------------------------
// Alignment
// For every nearby boid in the system, calculate the average velocity

ofVec3f Boid::align(vector<Boid*> *boids) {
    float neighborDist = 50.0;
    ofVec3f sum;
    sum.set(0.0,0.0,0.0);
    
    int count = 0;
    
    for(int i = 0;i < boids->size();i++) {
        Boid *other = boids->at(i);
        
        float d = loc.distance(other->loc);
        
        if(d > 0 && d < neighborDist) {
            sum += other->vel;
            count++;
        }
    }
    
    if(count > 0) {
        sum /= count;
        sum.limit(maxforce);
    }
    
    return sum;
}
//----------------------------------
// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location

ofVec3f Boid::cohesion(vector<Boid*> *boids) {
    float neighborDist = 50.0;
    ofVec3f sum = ofVec3f(0,0,0);
    
    int count = 0;
    
    for(int i=0;i<boids->size();i++) {
        Boid *other = boids->at(i);
        
        float d = loc.distance(other->loc);
        
        if(d > 0 && d < neighborDist) {
            sum += other->loc;
            count++;
        }
    }
    
    if(count > 0) {
        sum /= count;
        return steer(sum,false); // steer towards that location
    }
    return sum;
}
//----------------------------------