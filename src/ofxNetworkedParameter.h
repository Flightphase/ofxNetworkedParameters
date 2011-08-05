/*
 *  ofxNetworkedParameter.h
 *  NetworkedSimpleGuiExample
 *
 *  Created by James George on 8/5/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class ofxNetworkedParameter {
  public:
	void * p;
	string isA;
	
	union paramU {
		bool b;
		int  i;
		float f;
	} lastValue;
	
	ofxNetworkedParameter(void * _p, string _isA);
	
	bool hasChanged();
	void update();
	
	string serialize();
	
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