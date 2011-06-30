/**
 *  openFrameworks version of the popular synchronization system Most Pixels Ever by Dan Shiffman
 *  original repo: https://github.com/shiffman/Most-Pixels-Ever
 *  our fork: https://github.com/FlightPhase/Most-Pixels-Ever
 *
 *  I affectionately refer to as "Most Pickles Ever" since it's gotten me out of the most pickles. ever!
 *
 *  Standing on the shoulders of the original creators:
 *  Dan Shiffman with Jeremy Rotsztain, Elie Zananiri, Chris Kairalla.
 *  Extended by James George on 5/17/11 @ Flightphase for the National Maritime Museum
 *
 *  Still need to convert the original examples to the new format
 *
 *  There is a drawback that this is not compatible with the Java MPE jar, the connections must go OF client to OF Server
 *
 */

#include "mpeClientTCP.h"
#include "mpeEvents.h"

//--------------------------------------------------------------
mpeClientTCP::mpeClientTCP() {
    setDefaults();
}

//--------------------------------------------------------------
void mpeClientTCP::setup(string _fileString, bool updateOnMainThread) {

	useMainThread = updateOnMainThread;

	fps = 0;
    loadIniFile(_fileString);
	frameCount = 0;
	shouldReset = false;
	heartbeatInterval = 0;
	timeOfNextHeartbeat = ofGetElapsedTimef();
}

//will work offline
void  mpeClientTCP::useSimulationMode(int framesPerSecond)
{
	simulatedFPS = framesPerSecond;
	simulationMode = true;
	lastFrameTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void mpeClientTCP::start() {

	tcpClient.setVerbose(DEBUG);

	//if(useMainThread){
    ofAddListener(ofEvents.draw, this, &mpeClientTCP::draw);
	//}

    if (!simulationMode && !tcpClient.setup(hostName, serverPort)) {
        err("TCP failed to connect to port " + ofToString(serverPort));
		lastConnectionAttempt = ofGetElapsedTimef();
		ofAddListener(ofEvents.update, this, &mpeClientTCP::retryConnectionLoop);
    }
	else{
		startThread(true, false);  // blocking, verbose
		out("TCP connection bound on port " + ofToString(serverPort));
	}
}

void mpeClientTCP::retryConnectionLoop(ofEventArgs& e)
{
	float now = ofGetElapsedTimef();
	if(now - lastConnectionAttempt > 1.0){ //retry every second
		if(tcpClient.setup(hostName, serverPort)) {
			//cout << "retry succeeded, removing listener!" << endl;
			ofRemoveListener(ofEvents.update, this, &mpeClientTCP::retryConnectionLoop);
			startThread(true, false);  // blocking, verbose
		}
		lastConnectionAttempt = now;
	}
}

void mpeClientTCP::draw(ofEventArgs& e)
{
    //no blocking
    if(useMainThread && lock()) {

		for(int i = 0; i < dataMessage.size(); i++){
			ofxMPEEventArgs e;
			e.message = dataMessage[i];
			e.frame = getFrameCount();
			//cout << "sending message in update " << e.frame << " message " << e.message << endl;

			ofNotifyEvent(ofxMPEEvents.mpeMessage, e);
		}
		dataMessage.clear();

		//TODO: ints, floats, bytes,

		if(shouldReset){
			reset();
		}

		if(triggerFrame){
			//ofLog(OF_LOG_VERBOSE, "Trigger Event :: ! with frame count " + frameCount);

			triggerFrame = false;

//            cout << "triggering frame" << endl;

			ofxMPEEventArgs e;
			e.message = "";
			e.frame = frameCount;
			ofNotifyEvent(ofxMPEEvents.mpeFrame, e);

			if(!simulationMode){
				done();
			}
		}

		unlock();
    }

    //cout << ofGetWidth() << " :: " << lWidth << "  " << ofGetHeight() << " :: " << lHeight << endl;
    if(ofGetWindowPositionX() != xOffset || ofGetWindowPositionY() != yOffset || ofGetWidth() != lWidth || ofGetHeight() != lHeight){
        setupViewport();
    }
}

//--------------------------------------------------------------
// Loads the settings from the Client XML file.
//--------------------------------------------------------------
void mpeClientTCP::loadIniFile(string _fileString) {
	out("Loading settings from file " + _fileString);

	ofxXmlSettings xmlReader;
    if (!xmlReader.loadFile(_fileString)){
        err("ERROR loading XML file!");
		return;
	}

    // parse INI file
    hostName   = xmlReader.getValue("settings:server:ip", "127.0.0.1", 0);
    serverPort = xmlReader.getValue("settings:server:port", 7887, 0);
    //turn this off if you don't want this client to sync frames but can still
    //receive messages. default is ON as that's the normal behavior
    frameLock = xmlReader.getValue("settings:framelock", true);
    if(frameLock){
        id         = xmlReader.getValue("settings:client_id", -1, 0);
        clientName = xmlReader.getValue("settings:client_name", "noname", 0);
    }
    else{
        cout << "opting out of frame lock" << endl;
    }
	cout << "***MPE:: HOST IS " << hostName << " Server Port is " << serverPort << endl;

	setLocalDimensions(xmlReader.getValue("settings:local_dimensions:width",  640, 0),
					   xmlReader.getValue("settings:local_dimensions:height", 480, 0));

    setOffsets(xmlReader.getValue("settings:local_location:x", 0, 0),
               xmlReader.getValue("settings:local_location:y", 0, 0));

	setMasterDimensions(xmlReader.getValue("settings:master_dimensions:width",  640, 0),
						xmlReader.getValue("settings:master_dimensions:height", 480, 0));

    goFullScreen = xmlReader.getValue("settings:go_fullscreen", "false", 0).compare("true") == 0;
    offsetWindow = xmlReader.getValue("settings:offset_window", "false", 0).compare("true") == 0;

    setupViewport();

	if (xmlReader.getValue("settings:debug", 0, 0) == 1){
        DEBUG = true;
	}

    if(xmlReader.getValue("settings:simulation:on", 0, 0) == 1){
        useSimulationMode(xmlReader.getValue("settings:simulation:fps", 30));
        cout << "using simulation mode" << endl;
    }

    out("Settings: server = " + hostName + ":" + ofToString(serverPort) + ",  id = " + ofToString(id)
        + ", local dimensions = " + ofToString(lWidth) + ", " + ofToString(lHeight)
        + ", location = " + ofToString(xOffset) + ", " + ofToString(yOffset));
}

void mpeClientTCP::setupViewport()
{
	if (goFullScreen){
		ofSetFullscreen(true);
    }
	if(offsetWindow){
		ofSetWindowPosition(xOffset, yOffset);
        ofSetWindowShape(lWidth, lHeight);
	}
}

bool mpeClientTCP::isConnected()
{
    return tcpClient.isConnected();
}

//--------------------------------------------------------------
// Sets the dimensions for the local display.
//--------------------------------------------------------------
void mpeClientTCP::setLocalDimensions(int _lWidth, int _lHeight) {
    if (_lWidth > -1 && _lHeight > -1) {
        lWidth = _lWidth;
        lHeight = _lHeight;
    }
}

//--------------------------------------------------------------
// Sets the offsets for the local display.
//--------------------------------------------------------------
void mpeClientTCP::setOffsets(int _xOffset, int _yOffset) {
    if (_xOffset > -1 && _yOffset > -1) {
        xOffset = _xOffset;
        yOffset = _yOffset;
    }
}

//--------------------------------------------------------------
// Sets the dimensions for the local display.
// The offsets are used to determine what part of the Master Dimensions to render.
// For example, if you have two screens, each 100x100, and the master dimensions are 200x100
// then you would set
//  client 0: setLocalDimensions(0, 0, 100, 100);
//  client 1: setLocalDimensions(100, 0, 100, 100);
// for a 10 pixel overlap you would do:
//  client 0: setLocalDimensions(0, 0, 110, 100);
//  client 1: setLocalDimensions(90, 0, 110, 100);
//--------------------------------------------------------------
void mpeClientTCP::setLocalDimensions(int _xOffset, int _yOffset, int _lWidth, int _lHeight) {
    setOffsets(_xOffset, _yOffset);
    setLocalDimensions(_lWidth, _lHeight);
}

//--------------------------------------------------------------
// Sets the master dimensions for the Video Wall.
// This is used to calculate what is rendered.
//--------------------------------------------------------------
void mpeClientTCP::setMasterDimensions(int _mWidth, int _mHeight) {
    if (_mWidth > -1 && _mHeight > -1) {
        mWidth = _mWidth;
        mHeight = _mHeight;
    }
}

//--------------------------------------------------------------
// Sets the field of view of the camera when rendering in 3D.
// Note that this has no effect when rendering in 2D.
//--------------------------------------------------------------
void mpeClientTCP::setFieldOfView(float val) {
    fieldOfView = val;
    cameraZ = (ofGetHeight() / 2.f) / tanf(M_PI * fieldOfView/360.f);
}

//--------------------------------------------------------------
// Places the viewing area for this screen. This must be called at the
// beginning of the render loop.  If you are using Processing, you would
// typically place it at the beginning of your draw() function.
//--------------------------------------------------------------
void mpeClientTCP::placeScreen() {
    if (bEnable3D) {
        placeScreen3D();
    } else {
        placeScreen2D();
    }
}

//--------------------------------------------------------------
// If you want to enable or disable 3D manually in automode
//--------------------------------------------------------------
void mpeClientTCP::enable3D(bool _b) {
    bEnable3D = _b;
}

//--------------------------------------------------------------
// Places the viewing area for this screen when rendering in 2D.
//--------------------------------------------------------------
void mpeClientTCP::placeScreen2D() {
    glTranslatef(xOffset * -1, yOffset * -1, 0);
}


//--------------------------------------------------------------
// Restores the viewing area for this screen when rendering in 3D.
//--------------------------------------------------------------
void mpeClientTCP::restoreCamera() {
    gluLookAt(ofGetWidth()/2.f, ofGetHeight()/2.f, cameraZ,
              ofGetWidth()/2.f, ofGetHeight()/2.f, 0,
              0, 1, 0);

    float mod = .1f;
    glFrustum(-(ofGetWidth()/2.f)*mod, (ofGetWidth()/2.f)*mod,
              -(ofGetHeight()/2.f)*mod, (ofGetHeight()/2.f)*mod,
              cameraZ*mod, 10000);
}


//--------------------------------------------------------------
// Places the viewing area for this screen when rendering in 3D.
//--------------------------------------------------------------
void mpeClientTCP::placeScreen3D() {
    gluLookAt(mWidth/2.f, mHeight/2.f, cameraZ,
              mWidth/2.f, mHeight/2.f, 0,
              0, 1, 0);


    // The frustum defines the 3D clipping plane for each Client window!
    float mod = .1f;
    float left   = (xOffset - mWidth/2)*mod;
    float right  = (xOffset + lWidth - mWidth/2)*mod;
    float top    = (yOffset - mHeight/2)*mod;
    float bottom = (yOffset + lHeight-mHeight/2)*mod;

    //float far    = 10000;
    float a = 10;
    float Far = 10000;
    float Near   = cameraZ*mod;
    glFrustum(left, right,
              top, bottom,
              Near, Far);
}


//--------------------------------------------------------------
// Checks whether the given point is on screen.
//--------------------------------------------------------------
bool mpeClientTCP::isOnScreen(float _x, float _y) {
    return (_x > xOffset &&
            _x < (xOffset + lWidth) &&
            _y > yOffset &&
            _y < (yOffset + lHeight));
}

//--------------------------------------------------------------
// Checks whether the given rectangle is on screen.
//--------------------------------------------------------------
bool mpeClientTCP::isOnScreen(float _x, float _y, float _w, float _h) {
    return (isOnScreen(_x, _y) ||
            isOnScreen(_x + _w, _y) ||
            isOnScreen(_x + _w, _y + _h) ||
            isOnScreen(_x, _y + _h));
}

//--------------------------------------------------------------
// Outputs a message to the console.
//--------------------------------------------------------------
void mpeClientTCP::out(string _str) {
    //print(_str);
	//cout << _str << endl;
}

//--------------------------------------------------------------
// Outputs a message to the console.
//--------------------------------------------------------------
void mpeClientTCP::print(string _str) {
    if (DEBUG)
        cout << "mpeClient: " << _str << endl;
}

//--------------------------------------------------------------
// Outputs an error message to the console.
//--------------------------------------------------------------
void mpeClientTCP::err(string _str) {
    //cerr << "mpeClient: " << _str << endl;
	ofLog(OF_LOG_ERROR, "MPE Client Error :: " + _str);
}

//--------------------------------------------------------------
void mpeClientTCP::threadedFunction() {
    out("Running!");
	
	lastHeartbeatTime = ofGetElapsedTimef();
	
    if(frameLock){
        // let the server know that this client is ready to start
        send("S" + ofToString(id) + "," + clientName);
    }
	else{
		//start a listener
		send("L");
	}

    while(isThreadRunning()) {

		if(frameLock && simulationMode){
			
			float now = ofGetElapsedTimef();
			if(now - lastFrameTime > 1./simulatedFPS){
				if(!useMainThread){
					ofxMPEEventArgs e;
					e.message = "";
					e.frame = frameCount;
					ofNotifyEvent(ofxMPEEvents.mpeFrame, e);
				}
				else {
					triggerFrame = true;
				}

				lastFrameTime = now;
				frameCount++;
			}

            ofSleepMillis(5);
			continue;
		}

		
		if (allConnected && ofGetElapsedTimef() - lastHeartbeatTime > 2.0) {
			//we lost connection... manually disconnect and join reset cycle
			if(tcpClient.close()){
				ofLog(OF_LOG_ERROR, "mpeClientTCP -- server connection timed out. Closing and entering reconnect loop.");
			}
			else{
				ofLog(OF_LOG_ERROR, "mpeClientTCP -- Error when closing TCP connection after timeout.");			
			}
		}
		
		if(!tcpClient.isConnected()){
			//we lost connection, start the retry loop and kill the thread
			lastConnectionAttempt = ofGetElapsedTimef();
			ofAddListener(ofEvents.update, this, &mpeClientTCP::retryConnectionLoop);
			stopThread(true);
			if(useMainThread){
				shouldReset = true;
			}
			else{
				reset();
			}

			if(DEBUG){
                cout << "lost connection to server " << endl;
			}
			//break the loop because we'll need to restart
			return;
		}

        if (!useMainThread || (useMainThread && lock()) ) {
            string msg = tcpClient.receive();
            if (msg.length() > 0 && lastmsg != msg) {
                read(msg);
                lastmsg = msg;
            }

			if(useMainThread){
				unlock();
			}
        }
        ofSleepMillis(5);
    }
}

//--------------------------------------------------------------
// Reads and parses a message from the server.
//--------------------------------------------------------------
void mpeClientTCP::read(string _serverInput) {
    out("Receiving: " + _serverInput);

    char c = _serverInput.at(0);
	if(c == 'R'){
		if(frameCount != 0){
			//we received a reset signal
			if(useMainThread){
				shouldReset = true;
			}
			else{
				reset();
			}
			cout << "Received frame reset" << endl;
		}
	}
    else if (c == 'G' || c == 'B' || c == 'I') {
        if (!allConnected) {
            if (DEBUG) out("all connected!");
            allConnected = true;
        }
		
		lastHeartbeatTime = ofGetElapsedTimef();
		
        // split into frame message and data message
        vector<string> info = ofSplitString(_serverInput, ":");
        vector<string> frameMessage = ofSplitString(info[0], ",");
        int fc = ofToInt(frameMessage[1]);


        if (frameLock && fc == frameCount) {
            frameCount++;
			
            // calculate new framerate
			float nowms = ofGetElapsedTimeMillis();
            float ms = nowms - lastMs;
            fps += ((1000.f / ms) - fps)*.2;
            lastMs = nowms;

			if(!useMainThread){

                //cout << "trigger frame " << frameCount << endl;

				ofxMPEEventArgs e;
				e.message = "";
				e.frame = frameCount;
				ofNotifyEvent(ofxMPEEvents.mpeFrame, e);

				done();
            }
			else {
                //cout << "trigger frame " << frameCount << endl;
				triggerFrame = true;
			}
        }

         //JG switched to after done event
        if (info.size() > 1) {
            // there is a message here with the frame event
            info.erase(info.begin());

            //TODO: Track Byte/Int/Floats messages too
            for(int i = 0; i < info.size(); i++){
				if(useMainThread){
					dataMessage.push_back( info[i] );
				}
				else{
					ofxMPEEventArgs e;
					e.message = info[i];
					e.frame = getFrameCount();

					//cout << "sending message in update " << e.frame << " message " << e.message << endl;

					ofNotifyEvent(ofxMPEEvents.mpeMessage, e);

				}
				//cout << "MPE frame " << getFrameCount() << " receiving data message " << dataMessage[i] << endl;
            }
		}
    }
}

void mpeClientTCP::reset()
{
	ofxMPEEventArgs e;
	e.message = "reset";
	e.frame = frameCount;
	ofNotifyEvent(ofxMPEEvents.mpeReset, e);
	allConnected = false;
	
	shouldReset = false;
	frameCount = 0;
}

//void mpeClientTCP::sendPauseEvent()
//{
//	ofxMPEEventArgs e;
//	e.message = "pause";
//	e.frame = frameCount;
//
//	ofNotifyEvent(ofxMPEEvents.mpePause, e);
//}

//--------------------------------------------------------------
// Send a message to the server.
//--------------------------------------------------------------
void mpeClientTCP::send(string _msg) {

    //_msg += "\n";
	if(!simulationMode && frameLock){
		out("Sending: " + _msg);
		//tcpClient.sendRaw(_msg);
		tcpClient.send(_msg);
	}
}

//--------------------------------------------------------------
// Format a broadcast message and send it.
// Do not use a colon ':' in your message!!!
//--------------------------------------------------------------
void mpeClientTCP::broadcast(string _msg) {
	if(!simulationMode){
		outgoingMessage += "," + _msg;
		//cout << "outgoing message is now " << outgoingMessage << endl;
	}
}

//--------------------------------------------------------------
// Sends a "Done" command to the server. This must be called at
// the end of the draw loop.
//--------------------------------------------------------------
//TODO: if done has already been called, dont call it again
void mpeClientTCP::done() {
//    rendering = false;
    string msg = "D," + ofToString(id) + "," + ofToString(frameCount);
	if(outgoingMessage != ""){
		msg += outgoingMessage;
		outgoingMessage = "";
		//cout << "MPE DEBUG :: outgoing message is " << msg << endl;
	}
    send(msg);
}

//--------------------------------------------------------------
// Stops the client thread.  You don't really need to do this ever.
//--------------------------------------------------------------
void mpeClientTCP::stop() {
    out("Quitting.");
    stopThread();
	if(useMainThread){
		ofRemoveListener(ofEvents.draw, this, &mpeClientTCP::draw);
	}
}


