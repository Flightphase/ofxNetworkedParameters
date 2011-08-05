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

#include "ofxNetworkedParameter.h"

ofxNetworkedParameter::ofxNetworkedParameter(void * _p, string _isA){
	p = _p;
	isA = _isA;

	update();
}

bool ofxNetworkedParameter::hasChanged(){
	if (isA=="bool") {
		return (lastValue.b != *(bool *)p);
	} else if (isA=="int"){
		return (lastValue.i != *(int *)p);
	} else if (isA=="float"){
		return (lastValue.f != *(float *)p);
	}
}

void ofxNetworkedParameter::update(){
	if (isA=="bool")
		lastValue.b = *(bool *)p;
	else if (isA=="int")
		lastValue.i = *(int *)p;
	else if (isA=="float")
		lastValue.f = *(float *)p;
}

string ofxNetworkedParameter::serialize(){
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
