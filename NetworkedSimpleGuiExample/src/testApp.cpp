#include "testApp.h"
#include "ofxNPSimpleGuiBridge.h"
//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	
	bool1 = bool2 = false;
	integer1 = float1 = 0;
	
	client = new mpeClientTCP();
	client->setup(ofToDataPath("mpe_client_settings.xml", false), true);
	ofxNetworkedParameters.setMPEClient(client);
	
	
	ofxSimpleGuiPage& testPage = gui.addPage("testPage");
	testPage.addToggle("Bool1", bool1);
	testPage.addToggle("Bool2", bool2);
	testPage.addSlider("Float1", float1, -42.0, 42.0);
	testPage.addSlider("Integer1", integer1, 0, 42);
	NetworkSimpleGuiPage(gui.page("testPage"));

	ofxNetworkedParameters.attachToNetwork();
						 
	// client->useSimulationMode(2);
	client->start();

	gui.toggleDraw();

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0);

	ofFill();
	ofSetColor(0, 0, 128);
	ofRect(0,0,(ofGetWidth()/42)*integer1,ofGetHeight());
	
	gui.draw(); 
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