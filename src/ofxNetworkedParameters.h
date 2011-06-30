#ifndef guard_ofxNetworkedParameters
#define guard_ofxNetworkedParameters

/*
 *  ofxNetworkedParameters.h
 *  ofxNetworkedParametersTest
 *
 *  Created by Timothy Gfrerer on 29/06/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 */
#include "ofMain.h"
#include "ofxMostPixelsEver.h"


#ifdef USE_NETWORKEDSIMPLEGUITOO
#include "ofxSimpleGuiToo.h"
#endif

class NetworkedParameter {
    public:
	void * p;
	string isA;
	
	union paramU {
		bool b;
		int  i;
		float f;
	} lastValue;

	NetworkedParameter(void * _p, string _isA){
		p = _p;
		isA = _isA;

		update();
	}

		
	bool hasChanged(){
		if (isA=="bool") {
			return (lastValue.b != *(bool *)p);
		} else if (isA=="int"){
			return (lastValue.i != *(int *)p);
		} else if (isA=="float"){
			return (lastValue.f != *(float *)p);
		}
	};

	void update(){
		if (isA=="bool")
			lastValue.b = *(bool *)p;
		else if (isA=="int")
			lastValue.i = *(int *)p;
		else if (isA=="float")
			lastValue.f = *(float *)p;
	};

	string serialize(){
		string s;
		s = isA + "|";

		if (isA=="bool")
			s += ofToString(*(bool *)p);
		else if (isA=="int")
			s += ofToString(*(int *)p);
		else if (isA=="float")
			s += ofToString(*(float *)p);

		return s;
	}

	template<class T>
	void set(T _val){
		if (typeOf(_val) == isA)
			*((T * )p) = _val;
		else
			ofLog(OF_LOG_ERROR, "ofxNetworkedParameters: Wrong type. Expected: " + isA + " received: " + typeOf(_val));
	}

private:
	const string typeOf(int&) { return "int"; }
	const string typeOf(float&) { return "float"; }
	const string typeOf(bool&) { return "bool"; }

};

class ofxNetworkedParameters {
public:
	ofxNetworkedParameters();
	~ofxNetworkedParameters();

	void addNetworkedParameter(string _name, int * _p);
	void addNetworkedParameter(string _name, float * _p);
	void addNetworkedParameter(string _name, bool * _p);

    void attachToNetwork();
	void detachFromNetwork();

	void update(ofEventArgs& args);
	void setMPEClient(mpeClientTCP* client);

#ifdef USE_NETWORKEDSIMPLEGUITOO

	void shareOfxSimpleGuiTooPage(ofxSimpleGuiPage& guiPage){
		for (int i = 0; i < guiPage.getControlsP()->size(); i++){
			string controlType = (*guiPage.getControlsP())[i]->controlType;
			string pageName = guiPage.key + "_";


			// wohoo! here we go...
			if (controlType == "Toggle") {
				ofLog(OF_LOG_NOTICE, "Sharing Toggle parameter: " + (*guiPage.getControlsP())[i]->key);
				addNetworkedParameter(
														  pageName + (*guiPage.getControlsP())[i]->key,
														  ((ofxSimpleGuiToggle *)((*guiPage.getControlsP())[i]))->value);
			} else if (controlType == "SliderInt") {
				ofLog(OF_LOG_NOTICE, "Sharing SliderInt parameter: " + (*guiPage.getControlsP())[i]->key);
				addNetworkedParameter(
														  pageName + (*guiPage.getControlsP())[i]->key,
														  ((ofxSimpleGuiSliderInt *)((*guiPage.getControlsP())[i]))->value);
			} else if (controlType == "SliderFloat") {
				ofLog(OF_LOG_NOTICE, "Sharing SliderFloat parameter: " + (*guiPage.getControlsP())[i]->key);
				addNetworkedParameter(
														  pageName + (*guiPage.getControlsP())[i]->key,
														  ((ofxSimpleGuiSliderFloat *)((*guiPage.getControlsP())[i]))->value);
			}
		}
	}
#endif


private:
	map<string, NetworkedParameter *> parameterList;
	void mpeMessageEvent(ofxMPEEventArgs& eventArgs);
	mpeClientTCP* client;

};

// spawn off global networkedParameters object, similar to ofxSimpleGuiToo

#endif

extern ofxNetworkedParameters networkedParameters;
