#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "ofxOsc.h"

#define HOST "192.168.1.51"
#define PORT 12345

class multiPlayer : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

    void gotMessage(ofMessage msg);

	private:
    int mTargetFrame;
    bool mResetFlag;
	  vector<ofxOMXPlayer*> omxPlayers; 
    ofxOscSender mSender;
    ofxOscReceiver mReceiver;


};

