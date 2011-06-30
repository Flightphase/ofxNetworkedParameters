#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(0);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	client = new mpeClientTCP();
	client->setup("mpe_client_settings.xml", true);
	client->start();
	
	backgroundWhite = false;
	networkedParameters.setMPEClient(client);
	networkedParameters.addNetworkedParameter("background", &backgroundWhite);
	networkedParameters.attachToNetwork();
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