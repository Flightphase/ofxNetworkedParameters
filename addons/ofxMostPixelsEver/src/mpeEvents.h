/**
 *  mpeEvents.h
 *  openFrameworks version of the popular synchronization system Most Pixels Ever by Dan Shiffman
 *  Modifications+Additions by James George
 *
 *  original repo: https://github.com/shiffman/Most-Pixels-Ever
 *  our fork: https://github.com/FlightPhase/Most-Pixels-Ever
 *
 *  I affectionately refer to as "Most Pickles Ever" since it's gotten me out of the most pickles. ever!
 * 
 *  Standing on the shoulders of the original creators:
 *  Dan Shiffman with Jeremy Rotsztain, Elie Zananiri, Chris Kairalla.
 *  Extended by James George on 5/17/11 @ Flightphase for the National Maritime Museum
 *
 *  Still need to convert the original examples to the new format
 * 
 *  There is a drawback that this is not compatible with the Java MPE jar, the connections must go OF client to OF Server
 *
 */


#pragma once

#include "ofMain.h"
#include "ofEvents.h"

class ofxMPEEventArgs : public ofEventArgs
{
  public:
    int frame;
    string message;
	//TODO:
//	vector<char> bytes;
//	vector<int> ints;
//	vector<float> floats;
};

class ofxMPECoreEvents
{
  public:
    ofEvent<ofxMPEEventArgs> mpeFrame;	
  	ofEvent<ofxMPEEventArgs> mpeMessage;
	ofEvent<ofxMPEEventArgs> mpeReset; //reset everything, used when the client or server goes down
	//TODO:
//	ofEvent<ofxMPEEventArgs> mpeInts;
//	ofEvent<ofxMPEEventArgs> mpeBytes;
//	ofEvent<ofxMPEEventArgs> mpeFloats;
};

extern ofxMPECoreEvents ofxMPEEvents;

template<class ListenerClass>
void ofxMPERegisterEvents(ListenerClass * listener){
    ofAddListener(ofxMPEEvents.mpeFrame, listener, &ListenerClass::mpeFrameEvent);
    ofAddListener(ofxMPEEvents.mpeMessage, listener, &ListenerClass::mpeMessageEvent);
    ofAddListener(ofxMPEEvents.mpeReset, listener, &ListenerClass::mpeResetEvent);
	
	//TODO:	
//	ofAddListener(ofxMPEEvents.mpeBytes, listener, &ListenerClass::mpeByteEvent);
//	ofAddListener(ofxMPEEvents.mpeInts, listener, &ListenerClass::mpeIntEvent);
//	ofAddListener(ofxMPEEvents.mpeFloats, listener, &ListenerClass::mpeIntEvent);
}


