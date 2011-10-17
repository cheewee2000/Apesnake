#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "face.h"

#include "ofxEdsdk.h"

// listen on port 12345
#define PORT 8338
#define NUM_MSG_STRINGS 20

class testApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);		
    
    ofTrueTypeFont		font;
    ofxOscReceiver	receiver;
    
    int				current_msg_string;
    string          msg_strings[NUM_MSG_STRINGS];
    float			timers[NUM_MSG_STRINGS];
    
    
    
    ofSoundPlayer mouthW,mouthH;
    ofxXmlSettings XML;
    
    //FaceTrack
    //check the position of the face and makes calculations
    float checkFace(face f);
    face currentFace;    
    face apesnake;
    
    float score, scoreThres;
    float orientationThres, mouthThres;
    float positionThres, scaleThres;
    float eyeOffsetX,eyeOffsetY;
    float eyePosThres;
    
    //image
    ofImage overlay;
    ofPoint posValue;
    ofPoint scaleValue;
    
    
    
    //camera
    ofxEdsdk::Camera camera;
    bool drawCamera;
    //other
    bool debugMode;
    void drawDebugStuff(bool debugMode);
    void drawFace();
    void setupDebugGUI();
    void loadSounds();
    void loadImages();
    void loadSettings();
    void saveSettings();
    
    ofImage eyeLeft, eyeRight, mouth;
    
};
