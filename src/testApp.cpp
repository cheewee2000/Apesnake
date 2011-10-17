#include "testApp.h"
#include "ofxSimpleGuiToo.h"
///TODO: set the mouth in a poligon?

//--------------------------------------------------------------
void testApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup( PORT );
    
	current_msg_string = 0;
	ofBackground( 0 );
    
    orientationThres= XML.getValue("SETINGS:orientationThres", 0);
    mouthThres= XML.getValue("SETINGS:mouthThres", 0);
    
    ofSetCircleResolution(64);
    ofEnableSmoothing();
    
    //loadXMLs
    apesnake.load();
    loadSettings();
    
    //camera
    camera.setup();
    
    //setup controll panel
    setupDebugGUI();
    debugMode=false;
    
    //load images and sounds
    loadSounds();
    loadImages();
    
    eyeOffsetX=15;
    eyeOffsetY=-40;
    drawCamera=false;
}

//--------------------------------------------------------------
void testApp::update(){
    //camera
    
    camera.update();
	if(camera.isFrameNew()) {
		// process the live view with camera.getLivePixels()
	}
	if(camera.isPhotoNew()) {
		// process the photo with camera.getPhotoPixels()
		// or just save the photo to disk (jpg only):
		camera.savePhoto("apesnakephotos/apesnake03-"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds())+".jpg");
	}
    
    
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}
    
    
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        
		// check for message
        if ( m.getAddress() == "/found") currentFace.found= m.getArgAsInt32(0);
        else if ( m.getAddress() == "/pose/position" ){
            currentFace.positionX= m.getArgAsFloat(0);
            currentFace.positionY= m.getArgAsFloat(1);
        }
        else if ( m.getAddress() == "/pose/orientation" ){
            currentFace.orientationX= m.getArgAsFloat(0);
            currentFace.orientationY= m.getArgAsFloat(1);
            currentFace.orientationZ= m.getArgAsFloat(2);
            
        }
        else if ( m.getAddress() == "/pose/scale") currentFace.scale= m.getArgAsFloat(0);
        
		else if ( m.getAddress() == "/gesture/mouth/width" ) currentFace.mouthWidth= m.getArgAsFloat(0);
		else if ( m.getAddress() == "/gesture/mouth/height" ) currentFace.mouthHeight= m.getArgAsFloat(0);
        
        else if ( m.getAddress() == "/gesture/eyebrow/left") currentFace.eyebrowLeft= m.getArgAsFloat(0);
        else if ( m.getAddress() == "/gesture/eyebrow/right") currentFace.eyebrowRight= m.getArgAsFloat(0);
        else if ( m.getAddress() == "/gesture/eye/left") currentFace.eyeLeft= m.getArgAsFloat(0);
        else if ( m.getAddress() == "/gesture/eye/right") currentFace.eyeRight= m.getArgAsFloat(0);
        else if ( m.getAddress() == "/gesture/jaw") currentFace.jaw= m.getArgAsFloat(0);
        else if ( m.getAddress() == "/gesture/nostrils") currentFace.nostrils= m.getArgAsFloat(0);
        
        
        
		else
		{
            ///*
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for ( int i=0; i<m.getNumArgs(); i++ )
			{
				// get the argument type
				msg_string += m.getArgTypeName( i );
				msg_string += ":";
				// display the argument - make sure we get the right type
				if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
					msg_string += ofToString( m.getArgAsInt32( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
					msg_string += ofToString( m.getArgAsFloat( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
					msg_string += m.getArgAsString( i );
				else
					msg_string += "unknown";
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
            //*/
            
		}
        
	}
    
    
    currentFace.eyePosRightX=eyeOffsetX*currentFace.eyeRight;
    currentFace.eyePosRightY=eyeOffsetY*currentFace.eyeRight;
    
    currentFace.eyePosLeftX=-eyeOffsetX*currentFace.eyeLeft;
    currentFace.eyePosLeftY=eyeOffsetY*currentFace.eyeLeft;
    
    
    //do stuff!
    if(currentFace.found>0){        
        score=checkFace(apesnake);
        
        if(!mouthW.getIsPlaying() ){
            if(score<=scoreThres){
                mouthW.play();
                camera.takePhoto();
            }
        }
        
    }
    
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
    //draw APESNAKE! hell yeah!
    float imageR=ofGetWidth()/overlay.getWidth();
    
    overlay.draw(ofGetWidth()*posValue.x,
                 ofGetHeight()*posValue.y, 
                 (overlay.getWidth()*imageR)*scaleValue.x, 
                 (overlay.getHeight()*imageR)*scaleValue.y);
    
    if(currentFace.found>0) drawFace();        
    drawDebugStuff(debugMode);
    
    if(drawCamera){
    	camera.draw(250, 380, 420, 278);
        // camera.drawPhoto(0, 0, 432, 288);
        
        if(camera.isLiveReady()) {
            stringstream status;
			status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
			(int) ofGetFrameRate() << " app-fps " << " / " <<
			(int) camera.getFrameRate() << " cam-fps";
            ofDrawBitmapString(status.str(), 10, 20);
        }
    }
}

//--------------------------------------------------------------
float testApp::checkFace  (face f){
    float score=0;
    
    if(f.mouthHeight!=-1)score+=abs(currentFace.mouthHeight-f.mouthHeight)*mouthThres;
    if(f.mouthWidth!=-1)score+=abs(currentFace.mouthWidth-f.mouthWidth)*mouthThres;
    
    //if(f.orientationX!=-1)score+=abs(currentFace.orientationX-f.orientationX)*orientationThres;
    //if(f.orientationY!=-1)score+=abs(currentFace.orientationY-f.orientationY)*orientationThres;
    //if(f.orientationZ!=-1)score+=abs(currentFace.orientationZ-f.orientationZ)*orientationThres;
    
    //if(f.positionX!=-1)score+=abs(currentFace.positionX-f.positionX)*positionThres;
    //if(f.positionY!=-1)score+=abs(currentFace.positionY-f.positionY)*positionThres;
    
    //if(f.scale!=-1)score+=abs(currentFace.scale-f.scale)*scaleThres;
    
    
    //check eyes
    if(f.eyePosRightX!=-1)score+=abs(currentFace.eyePosRightX-f.eyePosRightX)*eyePosThres;
    if(f.eyePosRightY!=-1)score+=abs(currentFace.eyePosRightY-f.eyePosRightY)*eyePosThres;
    
    if(f.eyePosLeftX!=-1)score+=abs(currentFace.eyePosLeftX-f.eyePosLeftX)*eyePosThres;
    if(f.eyePosLeftY!=-1)score+=abs(currentFace.eyePosLeftY-f.eyePosLeftY)*eyePosThres;
    
    
    //chech mouthposition
    
    
    
    return score;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    switch (key) {
            //save case: no data gets saved unless you hit the s key
        case 's':
            apesnake.save(currentFace);
            saveSettings();
            break;
            //save case: no data gets loaded after first load unless you hit the l key
        case 'l':
            apesnake.load();
            loadSettings();
            break;
        case 'c':
            drawCamera=!drawCamera;
            break;
        case 'd':
            debugMode=!debugMode;
            drawCamera=!drawCamera;
            gui.setPage(key - '0');
            gui.show();
            break;
        case ' ':
            break;
        case ']':
            orientationThres+=.1;
            break;
        case '[':
            orientationThres-=.1;
            break;
        case ',':
            mouthThres+=.1;
            break;
        case '.':
            mouthThres-=.1;
            break;
            
        default:
            break;
    }
    
    //    XML.setValue("SETINGS:orientationThres", orientationThres);
    //    XML.setValue("SETINGS:mouthThres", mouthThres);
    
    //    XML.saveFile("settings.xml");
}

//--------------------------------------------------------------
void testApp::drawFace(){
    ofNoFill();
    ofSetColor(255);
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()-currentFace.positionX * ofGetWidth()/320.0, currentFace.positionY* ofGetHeight()/240);
    //ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
    
    
    ofScale(currentFace.scale, currentFace.scale);
    
    
    ofRotate(ofRadToDeg(currentFace.orientationX), -1, 0, 0);
    ofRotate(ofRadToDeg(currentFace.orientationY), 0, -1, 0);
    ofRotate(ofRadToDeg(currentFace.orientationZ), 0, 0, -1);
    
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255,100);
    //mouth
    //ofSetLineWidth(5);
    //ofEllipse(0,0, currentFace.mouthWidth*10, currentFace.mouthHeight*10);
    mouth.draw(0,0,currentFace.mouthWidth*15, currentFace.mouthHeight*28);
    
    
    //eyes
    
    //left eye
    //ofEllipse(-15*currentFace.eyeLeft,-40*currentFace.eyeLeft,10,10);
    eyeLeft.draw(-15*currentFace.eyeLeft,-40*currentFace.eyeLeft);
    //right eye
    //ofEllipse(15*currentFace.eyeRight,-40*currentFace.eyeRight,10,10);
    eyeRight.draw(15*currentFace.eyeRight,-40*currentFace.eyeRight);
    
    //eyebrows
    ofLine(-4*currentFace.eyebrowLeft,-20*currentFace.eyebrowLeft, -10*currentFace.eyebrowLeft,-20*currentFace.eyebrowLeft);
    
    ofPopMatrix();
    
}
//--------------------------------------------------------------
void testApp::drawDebugStuff(bool debugMode){
    if(debugMode){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(260, 130);
        
        ofFill();
        ofSetColor(0,0,0,80);
        ofRect(-10, -15, 420, 250);
        
        ofNoFill();
        ofSetColor(255,255,255);
        string buf;
        buf = "listening for osc messages on port" + ofToString( PORT );
        ofDrawBitmapString( buf, 0, 0);
        
        // draw mouse state
        
        buf = "\nposition: " + ofToString( currentFace.positionX, 4) +  " " + ofToString( currentFace.positionY, 4 );
        buf += "\norientation: " + ofToString( currentFace.orientationX, 4) +  " " + ofToString( currentFace.orientationY, 4 )+  " " + ofToString( currentFace.orientationZ, 4 );
        buf += "\nscale: " + ofToString( currentFace.scale, 4);
        
        buf += "\nmouth: " + ofToString( currentFace.mouthWidth, 4) +  " " + ofToString( currentFace.mouthHeight, 4 );
        buf += "\neyes: " + ofToString( currentFace.eyeLeft, 4) +  " " + ofToString( currentFace.eyeRight, 4 );
        buf += "\neyebrow: " + ofToString( currentFace.eyebrowLeft, 4) +  " " + ofToString( currentFace.eyebrowRight, 4 );
        buf += "\njaw: " + ofToString( currentFace.jaw, 4);
        buf += "\nnostrils: " + ofToString( currentFace.nostrils, 4);
        
        
        ofDrawBitmapString( buf, 0, 40 );
        
        
        buf = "score: " + ofToString( score, 4);
        buf += "\norientationThres[]: " + ofToString( orientationThres, 4);
        buf += "\nmouthhres<>: " + ofToString( mouthThres, 4);
        
        ofDrawBitmapString( buf, 0, 180 );
        
        for ( int i=0; i<NUM_MSG_STRINGS; i++ )
            ofDrawBitmapString( msg_strings[i], 0, 200+15*i );
        
        ofNoFill();
        ofSetColor(255);
        ofPopMatrix();
        
        
        gui.draw();
        ofPopStyle();
    }
}
//--------------------------------------------------------------
void testApp::setupDebugGUI(){
	gui.addTitle("Position and Scale");		// use '[' ']' to cycle through pages, or keys 1-9
    gui.addSlider2d("Scale",scaleValue, -1, 2, -1, 2);
    gui.addSlider2d("Pos",posValue, 0, 1, 0, 1);
    
    gui.loadFromXML();
}

//--------------------------------------------------------------
void testApp::loadSounds(){
    mouthW.loadSound("sounds/OralMoses_Tongue Chatter Short2.wav");
    mouthH.loadSound("sounds/OralMoses_Tongue Chatter Short3.wav");
    
}

//--------------------------------------------------------------
void testApp::loadImages(){
    eyeLeft.loadImage("imgs/eyeL.png");
    eyeRight.loadImage("imgs/eyeR.png");
    mouth.loadImage("imgs/mouth.png");
    overlay.loadImage("imgs/manwolfs.png");
}

//--------------------------------------------------------------
void testApp::loadSettings(){
    
    XML.loadFile("settings.xml");
    orientationThres= XML.getValue("SETINGS:orientationThres", 0.0);
    mouthThres= XML.getValue("SETINGS:mouthThres", 0.0);
    scoreThres= XML.getValue("SETINGS:scoreThres", 0.0);
    positionThres= XML.getValue("SETINGS:positionThres", 0.0);
    scaleThres= XML.getValue("SETINGS:scaleThres", 0.0);
    
    eyePosThres= XML.getValue("SETINGS:eyePosThres", 0.0);
    
}

//--------------------------------------------------------------
void testApp::saveSettings(){
    XML.setValue("SETINGS:orientationThres", orientationThres);
    XML.setValue("SETINGS:mouthThres", mouthThres);
    XML.setValue("SETINGS:scoreThres", scoreThres);
    XML.setValue("SETINGS:positionThres", positionThres);
    XML.setValue("SETINGS:scaleThres", scaleThres);
    XML.setValue("SETINGS:eyePosThres", eyePosThres);
    
    XML.saveFile("settings.xml");
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
