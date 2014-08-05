#include "multiPlayer.h"

//--------------------------------------------------------------
void multiPlayer::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetVerticalSync(false);
	
	ofDirectory currentVideoDirectory(ofToDataPath("../../../video", true));
	if (currentVideoDirectory.exists()) 
	{
		currentVideoDirectory.listDir();
		vector<ofFile> files = currentVideoDirectory.getFiles();
		
		
		for (int i=0; i<1; ++i) //files.size(); i++) 
		{
			ofxOMXPlayerSettings settings;
			settings.videoPath = files[0].path();
			settings.useHDMIForAudio = true;	//default true
			settings.enableLooping = true;		//default true
			settings.enableAudio = true;		//default true, save resources by disabling
			settings.enableTexture = false;		//default true
			if (!settings.enableTexture) 
			{
				/*
				 We have the option to pass in a rectangle
				 to be used for a non-textured player to use (as opposed to the default full screen)
				 */
				settings.displayRect.width = 400;
				settings.displayRect.height = 300;
				settings.displayRect.x = 40+(400*i);
				settings.displayRect.y = 200;
			}
			
			ofxOMXPlayer* player = new ofxOMXPlayer();
			player->setup(settings);
			omxPlayers.push_back(player);
      player->setPaused(true);
		}
	}

  mSender.setup( HOST, PORT );
  mReceiver.setup(PORT);
  mTargetFrame = 0;
}

//--------------------------------------------------------------
void multiPlayer::update()
{

  // handle osc input
  while(mReceiver.hasWaitingMessages()) {
    ofxOscMessage lNewMessage;
    mReceiver.getNextMessage(&lNewMessage);

    if(lNewMessage.getAddress() == "/FRAME") {
      mTargetFrame = lNewMessage.getArgAsInt32(0);
    }
  }

}


//--------------------------------------------------------------
void multiPlayer::draw(){
	ofBackgroundGradient(ofColor::red, ofColor::black, OF_GRADIENT_BAR);
	
  ofxOscMessage m;
  m.setAddress("/player/frame");
  for(int i = 0; i<omxPlayers.size(); i++) {
  omxPlayers[i]->setPaused(true);
    //omxPlayers[i]->stepFrameForward();
    if(mTargetFrame == 0 && !mResetFlag) {
      mResetFlag = true;
      omxPlayers[i]->restartMovie();
      continue;
    } else if (mTargetFrame == 0) {
      continue;
    }

    mResetFlag = false;
    int lCurrentFrame = omxPlayers[i]->getCurrentFrame();
    for(int lCurrentFrame = omxPlayers[i]->getCurrentFrame(); lCurrentFrame <= mTargetFrame; ++lCurrentFrame) {
      omxPlayers[i]->stepFrameForward();
    }
    m.addIntArg(lCurrentFrame);
	
  }

  mSender.sendMessage(m);


	for (int i=0; i<omxPlayers.size(); i++) 
	{
		ofxOMXPlayer* player = omxPlayers[i];
		//if (player->isPlaying()) 
		//{
			ofPushMatrix();
				ofTranslate(player->settings.displayRect.x, 0, 0);
				ofDrawBitmapStringHighlight(player->getInfo(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
			ofPopMatrix();
		//}		
	}
	stringstream fpsInfo;
	fpsInfo <<"\n" <<  "APP FPS: "+ ofToString(ofGetFrameRate());
	ofDrawBitmapStringHighlight(fpsInfo.str(), 60, 20, ofColor::black, ofColor::yellow);
}

//--------------------------------------------------------------
void multiPlayer::keyPressed  (int key){
	switch (key) 
	{
		case 'c':
		{
			break;
		}
	}
	
}

void multiPlayer::gotMessage(ofMessage m) {}

