#pragma once

#include "ofMain.h"

// #define USE_NETWORKEDSIMPLEGUITOO
#include "ofxNetworkedParameters.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	mpeClientTCP* client;
	void mpeFrameEvent(ofxMPEEventArgs& event);
	void mpeResetEvent(ofxMPEEventArgs& event);

	bool bool1, bool2;
	int integer1;
	float float1;
	
	bool isLarge[9];
	
	
};
