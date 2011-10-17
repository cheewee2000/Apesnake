#include "face.h"
#include "ofxXmlSettings.h"

void face::load(string file){
    
    xml.loadFile(file);
    
    mouthWidth	= xml.getValue("SETINGS:mouthWidth", -1);
    mouthHeight	= xml.getValue("SETINGS:mouthHeight", -1);
    
    positionX	= xml.getValue("SETINGS:positionX", -1);
    positionY	= xml.getValue("SETINGS:positionY", -1);
    orientationX	= xml.getValue("SETINGS:orientationX", -1);
    orientationY	= xml.getValue("SETINGS:orientationY", -1);
    orientationZ	= xml.getValue("SETINGS:orientationZ", -1);
    scale	= xml.getValue("SETINGS:scale", -1);
    eyebrowLeft	= xml.getValue("SETINGS:eyebrowLeft", -1);
    eyebrowRight	= xml.getValue("SETINGS:eyebrowRight", -1);
    
    eyeLeft	= xml.getValue("SETINGS:eyeLeft", -1);
    eyeRight	= xml.getValue("SETINGS:eyeRight", -1);
    jaw	= xml.getValue("SETINGS:jaw", -1);
    nostrils	= xml.getValue("SETINGS:nostrils", -1);
    
    eyePosRightX	= xml.getValue("SETINGS:eyePosRightX", -1);
    eyePosRightY	= xml.getValue("SETINGS:eyePosRightY", -1);
    
    eyePosLeftX	= xml.getValue("SETINGS:eyePosLeftX", -1);
    eyePosLeftY	= xml.getValue("SETINGS:eyePosLeftY", -1);
    
    cout<< "Settings loaded!";

}

// default value always is the last one to be specified
void face::save(face f, string file){
    
    xml.setValue("SETINGS:mouthWidth", f.mouthWidth);
    xml.setValue("SETINGS:mouthHeight", f.mouthHeight);
    xml.setValue("SETINGS:positionX", f.positionX);
    xml.setValue("SETINGS:positionY", f.positionY);
    xml.setValue("SETINGS:orientationX", f.orientationX);
    xml.setValue("SETINGS:orientationY", f.orientationY);
    xml.setValue("SETINGS:orientationZ", f.orientationZ);
    xml.setValue("SETINGS:scale", f.scale);
    xml.setValue("SETINGS:eyebrowLeft", f.eyebrowLeft);
    xml.setValue("SETINGS:eyebrowRight", f.eyebrowRight);
    xml.setValue("SETINGS:eyeLeft", f.eyeLeft);
    xml.setValue("SETINGS:eyeRight", f.eyeRight);
    xml.setValue("SETINGS:jaw", f.jaw);
    xml.setValue("SETINGS:nostrils", f.nostrils);   
    
    xml.setValue("SETINGS:eyePosLeftX", f.eyePosLeftX);
    xml.setValue("SETINGS:eyePosLeftY", f.eyePosLeftY);
    xml.setValue("SETINGS:eyePosRightX", f.eyePosRightX);
    xml.setValue("SETINGS:eyePosRightY", f.eyePosRightY);
    
    
    xml.saveFile(file);
    
    cout<< "Saved settings";

}

