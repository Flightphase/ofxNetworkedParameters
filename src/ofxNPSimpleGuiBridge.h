/**
 *  ofxNetworkedParameters
 *	
 *   Copyright (c) 2011, Tim Gfrerer, James George
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
////////////
//IMPORANT//
////////////
//You only need to include this if you are working with netowrking ofxSimpleGuiToo
//otherwise you dont' need it
//a compatible version of ofxSimpleGuiToo can always be found here:
//https://github.com/Flightphase/ofxSimpleGuiToo

#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxNetworkedParameters.h"

static void NetworkSimpleGuiPage(ofxSimpleGuiPage& guiPage){
	for (int i = 0; i < guiPage.getControls().size(); i++){
		string controlType = (guiPage.getControls())[i]->controlType;
		string pageName = guiPage.key + "_";
		
		if (controlType == "Toggle") {
			ofLog(OF_LOG_NOTICE, "Sharing Toggle parameter: " + (guiPage.getControls())[i]->key);
			ofxNetworkedParameters.addNetworkedParameter(pageName + (guiPage.getControls())[i]->key,
								  ((ofxSimpleGuiToggle *)((guiPage.getControls())[i]))->value);
		} else if (controlType == "SliderInt") {
			ofLog(OF_LOG_NOTICE, "Sharing SliderInt parameter: " + (guiPage.getControls())[i]->key);
			ofxNetworkedParameters.addNetworkedParameter(pageName + (guiPage.getControls())[i]->key,
								  ((ofxSimpleGuiSliderInt *)((guiPage.getControls())[i]))->value);
		} else if (controlType == "SliderFloat") {
			ofLog(OF_LOG_NOTICE, "Sharing SliderFloat parameter: " + (guiPage.getControls())[i]->key);
			ofxNetworkedParameters.addNetworkedParameter(pageName + (guiPage.getControls())[i]->key,
								  ((ofxSimpleGuiSliderFloat *)((guiPage.getControls())[i]))->value);
		}
	}
}