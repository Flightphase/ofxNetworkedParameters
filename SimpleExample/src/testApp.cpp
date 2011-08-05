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
 * This example shows the simplest use case, just sharing a single bool across
 * to apps to change the background color in sync
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	backgroundWhite = false;
	
	client = new mpeClientTCP();
	client->setup("mpe_client_settings.xml", true);	
	ofxNetworkedParameters.setMPEClient(client);
	
	ofxNetworkedParameters.addNetworkedParameter("background", &backgroundWhite);
	
	client->start();

	ofxNetworkedParameters.attachToNetwork();

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	if(backgroundWhite){
		ofBackground(255, 200, 10);
	}
	else{
		ofBackground(0, 250, 100);
	}
		
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	backgroundWhite = !backgroundWhite;
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}