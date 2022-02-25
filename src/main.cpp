#include "ofMain.h"
#include "ofApp.h"
#include <crtdbg.h>

//========================================================================
int main( ){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ofGLFWWindowSettings settings; 
	settings.setSize(600, 450);
	settings.windowMode = OF_WINDOW; 
	settings.resizable = false;
	settings.title = "myConstellation";
	ofCreateWindow(settings);

	//ofSetupOpenGL(600, 450, OF_WINDOW);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
