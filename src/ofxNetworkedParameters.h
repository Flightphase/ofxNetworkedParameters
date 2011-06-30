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

#ifndef guard_ofxNetworkedParameters
#define guard_ofxNetworkedParameters

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
