#pragma once

#include "ofMain.h"
#include <vector>

#define MAX_STARS 60

class Stars {
public:
	ofPoint s_coord; //x, y
	float s_rad; //radius
	float s_mag; //magnitude
	void drawStar(char mode) const;
	void setStars() {
		s_coord = { ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()) };
		s_rad = ofRandom(1, 3);
		s_mag = s_rad+2;
	}
};

class Button {
private:
	ofPoint b_coord; //x, y
	float b_width;
	float b_height;
	string b_name;

public:
	void drawButton() const;
	bool isIn(int x, int y) const;
	void setButton(float x, float y, string name) {
		b_coord = { x,y };
		b_width = 50;
		b_height = 20;
		b_name = name;
	}
	void setButton(float x, float y, string name, int W, int H) {
		b_coord = { x, y };;
		b_width = W;
		b_height = H;
		b_name = name;
	}
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void resetStars();
		
		bool reset; //is reset button pressed?
		bool save; //is save button pressed?
		bool newSky; //is new button pressed?
		bool drawingLine; //show line between pickedXY and current mouseXY
		int pickedX, pickedY; //latest selected star

		vector<Stars> stars;
		vector<ofPoint> listofStars; //store current Stars' coordinates
		vector<int> selectedStarsIdx; //store selected Stars' index
		vector<ofPolyline*> line; //store Polylines until reset

		Button saveButton;
		Button resetButton;
		Button newButton;
		
		ofImage img; //for save image

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
