#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"


//========================================================================
int main( ){
	
	ofAppBaseWindow* pWindow = new ofAppGlutWindow();
	ofSetupOpenGL(pWindow, 600,320, OF_FULLSCREEN);			// <-------- setup the GL context
	
	ofSetLogLevel(OF_LOG_NOTICE);
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	ofRunApp(new testApp());
	
}
