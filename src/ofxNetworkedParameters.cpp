/*
 *  ofxNetworkedParameters.cpp
 *  ofxNetworkedParametersTest
 *
 *  Created by Timothy Gfrerer on 29/06/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxNetworkedParameters.h"


ofxNetworkedParameters::ofxNetworkedParameters(){

}


//--------------------------------------------------------------

ofxNetworkedParameters::~ofxNetworkedParameters(){
	map<string, sharedParameter*>::iterator param = parameterList.begin();

	while (param != parameterList.end()) {
		delete param->second;
		param++;
	}

}

void ofxNetworkedParameters::attachToNetwork(){
	ofAddListener(ofxMPEEvents.mpeMessage, this, &ofxNetworkedParameters::mpeMessageEvent);
	ofAddListener(ofEvents.update, this, &ofxNetworkedParameters::update);
}

void ofxNetworkedParameters::detachFromNetwork(){
	ofRemoveListener(ofEvents.update, this, &ofxNetworkedParameters::update);
	ofRemoveListener(ofxMPEEvents.mpeMessage, this, &ofxNetworkedParameters::mpeMessageEvent);
}


//--------------------------------------------------------------

void ofxNetworkedParameters::addNetworkedParameter(string _name, int * _p){
	parameterList[_name] = new sharedParameter(_p, "int");
}
void ofxNetworkedParameters::addNetworkedParameter(string _name, float * _p){
	parameterList[_name] = new sharedParameter(_p, "float");
}
void ofxNetworkedParameters::addNetworkedParameter(string _name, bool * _p){
	parameterList[_name] = new sharedParameter(_p, "bool");
}

//--------------------------------------------------------------

void ofxNetworkedParameters::update(ofEventArgs& args){
	map<string, sharedParameter*>::iterator param = parameterList.begin();

	while (param != parameterList.end()) {
		if (param->second->hasChanged()){

			ofLog(OF_LOG_NOTICE, "Parameter" + param->first + " has changed.\nBroadcasting: nP|" + param->first + "|" + param->second->serialize());

			if (client != NULL) 
				// broadcast mpe message here.
				client->broadcast("nP|" + param->first + "|" + param->second->serialize());
			else 
				ofLog(OF_LOG_WARNING, "ofxNetworkedParameters: MPE client not set.");

			// then apply change.
			param->second->update();
			
		}
		param++;
	}

}

void ofxNetworkedParameters::setMPEClient(mpeClientTCP* _client){
	client = _client;
}

//--------------------------------------------------------------
void ofxNetworkedParameters::mpeMessageEvent(ofxMPEEventArgs& eventArgs){
	vector<string> msg = ofSplitString(eventArgs.message, "|", true, true);
	// "nP|integer1|int|-2"
	
	if (msg.size() == 4 && msg[0]=="nP") {

		map<string, sharedParameter*>::iterator param = parameterList.find(msg[1]);

		ofLog(OF_LOG_NOTICE, "ofxNetworkedParameters: Setting parameter " + msg[1] + " to " + ofToString(msg[3]));

		if (param != parameterList.end()) {

			if (msg[2]=="float"){
				param->second->set(ofToFloat(msg[3]));
			} else if (msg[2] == "int") {
				param->second->set(ofToInt(msg[3]));
			} else if (msg[2] == "bool"){
				param->second->set(ofToBool(msg[3]));
			}

			// apply changes locally.
			param->second->update();

		} else {
			ofLog(OF_LOG_ERROR, "ofxNetworkedParameters: Could not find parameter with name: " + msg[1] + " in networked parameters list");
		}


	}

}

//--------------------------------------------------------------
// instantiate a global object upon inclusion of this header file.

ofxNetworkedParameters networkedParameters;
