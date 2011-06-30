/**
 *  openFrameworks version of the popular synchronization system Most Pixels Ever by Dan Shiffman
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
#include "ofxNetwork.h"

typedef struct
{
	bool started;
	bool ready;
	string name;
	int tcpServerIndex;
} Connection;

class mpeServerTCP : public ofThread
{
  public:
	mpeServerTCP();
	~mpeServerTCP();

	void setup(string setupFile);
	void setup(int framerate, int port, int numClients);
	void reset(); //sends a reset signal to all clients and reset frame count
	void close();
    void printClientStatus();

  protected:
	ofxTCPServer server;
	void update(ofEventArgs& args);
	void threadedFunction();

	float lastFrameTriggeredTime;
	bool allconnected;
	bool running;
	int framerate;
	int numExpectedClients;
	int numConnectedClients;
	int currentFrame;
	bool shouldTriggerFrame;
	vector<Connection> connections;
	vector<int> listeners;
	bool newMessage;
	string currentMessage;
	float timeOfNextHeartbeat;
	float heartBeatInterval;

};
