#include "ofMain.h"
#include "ofApp.h"
//#include <crtdbg.h>

//========================================================================
int main( ){
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ofGLFWWindowSettings settings; 

	settings.setSize(default_width, default_height); // ofApp.h
	settings.windowMode = OF_WINDOW; 
	settings.resizable = true;
	settings.title = "makeConstellation";

	ofCreateWindow(settings);
	// メインアプリケーション呼出
	ofRunApp(new ofApp());
}
