#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	client = new mpeClientTCP();
	client->setup(ofToDataPath("mpe_client_settings.xml", false), true);

	ofxNetworkedParameters.setMPEClient(client);
		
	client->start();

	for (int i = 0; i<9; i++) {
		isLarge[i] = false;
		ofxNetworkedParameters.addNetworkedParameter("bool_" + ofToString(i), &isLarge[i]);
	}
	
	ofxNetworkedParameters.attachToNetwork();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	
	ofFill();
	for (int i = 0; i<9; i++) {

		ofSetColor(255, (isLarge[i]==true ? 255 : 0), 0);
		ofCircle(100 + (i%3)*200, 100 + (i/3)*200, 20 + (isLarge[i]==true ? 50 : 0));

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
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	int closest = 0; 
	float closestDistance = 1000000;
	
	for (int i = 0; i<9; i++){
		
		float distance = ofVec2f(100 + (i%3)*200, 100 + (i/3)*200).distance(ofVec2f(x,y));
		
		if ( distance < closestDistance){
			closestDistance = distance;
			closest	 = i;
		}
	}
	
	isLarge[closest] ^= true;
	
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