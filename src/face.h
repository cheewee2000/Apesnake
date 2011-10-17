
#import "ofMain.h"

#include "ofxXmlSettings.h"

#define DEFAULT_XML_FILE  "apesnake.xml"

class face{
//apesnake!!!
public:
    float mouthWidth, mouthHeight;
    int found;
    float positionX, positionY;
    float orientationX, orientationY, orientationZ;
    float scale;
    float eyebrowLeft, eyebrowRight;
    float eyeLeft, eyeRight;
    float jaw, nostrils;    
    
    float eyePosRightX, eyePosRightY;
    float eyePosLeftX, eyePosLeftY;
    
    ofxXmlSettings xml;
    
    
    void load(string file = DEFAULT_XML_FILE);
    void save(face f, string file = DEFAULT_XML_FILE);
    
};


