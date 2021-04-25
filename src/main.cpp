#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
//========================================================================
int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
	ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
    settings.setSize(1024, 768);
    settings.numSamples =16;
    settings.alphaBits =4;
    ofCreateWindow(settings);
	ofRunApp(new ofApp());

}

