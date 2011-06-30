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

#include "mpeServerTCP.h"
#include "ofxXmlSettings.h"

mpeServerTCP::mpeServerTCP()
{
	allconnected = false;
	framerate = 30;
	numExpectedClients = 0;
	numConnectedClients = 0;
	currentFrame = 0;
	shouldTriggerFrame = false;
	running = false;
	newMessage = false;

	timeOfNextHeartbeat = ofGetElapsedTimef();
	heartBeatInterval = 2.0;

}

mpeServerTCP::~mpeServerTCP()
{
	close();
}

void mpeServerTCP::setup(string settingsFile)
{

	ofxXmlSettings settings;
	if(!settings.loadFile(settingsFile)){
		ofLog(OF_LOG_ERROR, "MPE Server -- Couldn't load settings file: " + settingsFile);
		return;
	}

	setup(settings.getValue("settings:framerate", 30, 0),
		  settings.getValue("settings:port", 9001, 0),
		  settings.getValue("settings:numclients", 2, 0));
}

void mpeServerTCP::setup(int fps, int port, int numClients)
{

	close(); // in case of double set up

	//make sure vsync is OFF
	//make sure framerate is fast as it can go

	if(!server.setup(port, false)){
		ofLog(OF_LOG_ERROR, "MPE Serever :: Setup failed");
	}
	numExpectedClients = numClients;
	framerate = fps;

	for(int i = 0; i < numExpectedClients; i++){
		Connection c;
		c.started = false;
		c.ready = false;
		c.name = "noname";
		connections.push_back(c);
	}

	startThread(true, false);

	shouldTriggerFrame = false;
	allconnected = false;
	lastFrameTriggeredTime = 0;
	currentFrame = 0;

	//ofAddListener(ofEvents.update, this, &mpeServerTCP::update);

	cout << "Setting up server with FPS " << fps << " on port " << port << " with clients " << numClients << endl;
}


void mpeServerTCP::update(ofEventArgs& args)
{
//	if(!server.isConnected()){
//		ofLog(OF_LOG_ERROR, "MPE Server :: Server Disconnected");
//	}
//    if(lock()){

//        if(shouldTriggerFrame){
//            float now = ofGetElapsedTimef();
//            float elapsed = (now - lastFrameTriggeredTime);
//    //		cout << "should trigger frame!" << endl;
//
//            if(elapsed >= 1.0/framerate){
//
//    //			cout << "triggered frame with framerate error of " << fabs( elapsed - 1.0/framerate)  << endl;
//
//                string message = "G,"+ofToString(currentFrame);
//                if (newMessage){
//                    message += currentMessage;
//                    newMessage = false;
//                    currentMessage = "";
//                }
//
//                //TODO append message
//                server.sendToAll(message);
//
//                for(int i = 0; i < connections.size(); i++){
//                    connections[i].ready = false;
//                }
//
//                shouldTriggerFrame = false;
//                lastFrameTriggeredTime = now;
//                currentFrame++;
//            }
//        }

//        unlock();
//    }
}


void mpeServerTCP::reset()
{
	currentFrame = 0;
	shouldTriggerFrame = false;
	server.sendToAll("R");

}

void mpeServerTCP::threadedFunction()
{
	while(isThreadRunning()){

		if(shouldTriggerFrame){
			float now = ofGetElapsedTimef();
			float elapsed = (now - lastFrameTriggeredTime);

			//cout << "should trigger frame!" << endl;

			if(elapsed >= 1.0/framerate){

				//cout << "triggered frame with framerate error of " << fabs( elapsed - 1.0/framerate)  << endl;

				string message = "G,"+ofToString(currentFrame);
				if (newMessage){
					message += currentMessage;
					newMessage = false;
					currentMessage = "";
				}

				server.sendToAll(message);

				for(int i = 0; i < connections.size(); i++){
					connections[i].ready = false;
				}

				shouldTriggerFrame = false;
				lastFrameTriggeredTime = now;
				currentFrame++;
			}
		}
		else {

			//check for dead clients
			bool lostConnection = false;
			for(int c = 0; c < numExpectedClients; c++){
				if(connections[c].started && !server.isClientConnected(connections[c].tcpServerIndex)){
					connections[c].started = false;
					lostConnection = true;
				}
			}

			if(allconnected && lostConnection){

				ofLog(OF_LOG_NOTICE, "MPE :: Client Disconnected -- RESET");

				//oops someone left
				printClientStatus();
				currentFrame = 0;
				shouldTriggerFrame = false;
				allconnected = false;

				server.sendToAll("R");
			}

			//cout << "All clients are connected! " << endl;

			for(int i = 0; i < server.getNumClients(); i++){
				string response = server.receive(i);

				if(response == ""){
					continue;
				}

	//			cout << "received a response " << response << endl;

				char first = response.at(0);
				if(first == 'L'){
					//Listener connected
					listeners.push_back(i);
				}
				else if(first == 'S'){
					//that's the start!
					int clientID = ofToInt(response.substr(1,1));
					if(clientID < numExpectedClients){
						vector<string> info = ofSplitString(response, ",", true, true);
						if(connections[clientID].started && currentFrame != 0){
                            //client already started, must have reset...
                            allconnected = false;
                            currentFrame = 0;
                            shouldTriggerFrame = false;
                            server.sendToAll("R");
						}

						connections[clientID].tcpServerIndex = i;
						connections[clientID].started = true;
						connections[clientID].name = info[1];
						cout << "Client ID " << clientID << " with response " << response << endl;
						//TODO: parse name
						printClientStatus();
					}
					else{
						ofLog(OF_LOG_ERROR, "Received Client ID " + ofToString(clientID)  + " out of range");
					}
				}
				else if(first == 'D'){

					if(!allconnected){
						continue;
					}

					vector<string> info = ofSplitString(response, ",", true, true);
					if(info.size() >= 3){
						int clientID = ofToInt(info[1]);
						int fc = ofToInt(info[2]);
						if(fc == currentFrame){
							//todo validate client id
							connections[clientID].ready = true;
							//cout << " client " << clientID << " is ready " << endl;
						}
						if(info.size() > 3){
							newMessage = true;
							for(int i = 3; i < info.size(); i++){
								currentMessage += ":" + info[i];
							}
							//cout << "NEW FORMAT :: MESSSAGE IS " << currentMessage << endl;
						}
					}
					else {
						ofLog(OF_LOG_ERROR, "MPE Server :: Response String " + response + " Invalid size");
					}
				}
			}

			if(!allconnected){
				allconnected = true;
				for(int c = 0; c < connections.size(); c++){
					if(!connections[c].started){
						allconnected = false;
						break;
					}
				}
				if(allconnected){
					shouldTriggerFrame = true;
					cout << "All clients connected!" << endl;
				}
			}
			//All connected and going
			else {
				bool allready = true;
				for(int c = 0; c < connections.size(); c++){
					if(!connections[c].ready){
						allready = false;
						break;
					}
				}
				if(allready){
					shouldTriggerFrame = true;
				}
			}
		}
        ofSleepMillis(5);
	}//end while
}

void mpeServerTCP::printClientStatus()
{
    ofLog(OF_LOG_NOTICE, "MPE Client Status:");
    ofLog(OF_LOG_NOTICE, "  Expecting " + ofToString(numExpectedClients) + " Clients");
    for(int i = 0; i < connections.size(); i++){
        ofLog(OF_LOG_NOTICE, "  Client (" + ofToString(i) + ") " + connections[i].name + " connected? " + (connections[i].started ? "yes" : "no") );
    }
}

void mpeServerTCP::close()
{
	if(!running) return;

	ofRemoveListener(ofEvents.update, this, &mpeServerTCP::update);

	cout << " closing MPE Server " << endl;

	if(server.isConnected()){
		server.close();
	}

	connections.clear();

	stopThread();

	running = false;
}
