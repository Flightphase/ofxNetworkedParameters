/**
 *  ofxNetworkedParameters
 *	
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * ofxNetworkedParameters provides an easy way to share settings
 * between instances of the same application running on multiple machines
 * synchronized with Most Pixels Ever.
 *
 */

#include "ofxNetworkedParameters.h"


ofxNetworkedParameters::ofxNetworkedParameters()
{

}


//--------------------------------------------------------------

ofxNetworkedParameters::~ofxNetworkedParameters(){
	map<string, NetworkedParameter*>::iterator param = parameterList.begin();

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
	parameterList[_name] = new NetworkedParameter(_p, "int");
}
void ofxNetworkedParameters::addNetworkedParameter(string _name, float * _p){
	parameterList[_name] = new NetworkedParameter(_p, "float");
}
void ofxNetworkedParameters::addNetworkedParameter(string _name, bool * _p){
	parameterList[_name] = new NetworkedParameter(_p, "bool");
}

//--------------------------------------------------------------

void ofxNetworkedParameters::update(ofEventArgs& args){
	map<string, NetworkedParameter*>::iterator param = parameterList.begin();

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

		map<string, NetworkedParameter*>::iterator param = parameterList.find(msg[1]);

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
