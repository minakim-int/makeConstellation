#pragma once
#include "ofMain.h"
#include <vector>

static const float default_width = 600;
static const float default_height = 450;
//--------------------------------------------------------------
class Stars {
public:
	ofPoint coord; // coordination
	float rad; // radius
	float mag; // magnitude
	void drawStar(char mode) const {
		if (mode == 'r') {
			ofDrawCircle(coord, rad);
		}
		else if (mode == 'm') {
			ofDrawCircle(coord, mag);
		}
	}
	void drawTwinkling(ofColor color, float offset) const {
		ofSetColor(color, 100/offset);
		ofDrawCircle(coord, rad+1+offset);
	}
	void setStarsPos() {		
		coord = { ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()) };
		rad = ofRandom(ofGetWidth()/default_width, ofGetWidth()/(default_width/3));
		mag = rad+ofGetWidth()/default_width;
	}
};
//--------------------------------------------------------------
class Button {
public:
	ofPoint coord;
	float width;
	float height;
	string name;

	void setButton(float x, float y, string pname, int pwidth = 50, int pheight = 20) {
		coord = { x, y };
		width = pwidth;
		height = pheight;
		name = pname;
	}	
	void drawButton(bool mode, int mouseX, int mouseY) const {
		ofSetColor(255); // 背景は基本白色のボタン
		if (isIn(mouseX, mouseY)) ofSetColor(200); // マウスがボタンの内部にある場合灰色になる
		if (mode) ofSetColor(255, 50); // modeがtrueの場合透明になる	
		ofDrawRectangle(coord, width, height);
		ofSetColor(0); // 文字は基本黒色
		ofDrawBitmapString(name, coord.x+6, coord.y+15);
	}
	bool isIn(int x, int y) const {
		// xとyがボタンの内部にあるかどうか判断
		if(x > coord.x && x < coord.x + width &&
			y < coord.y + height && y > coord.y) { 
			return true;
		}
		else {
			return false;
		}
	}
};
//--------------------------------------------------------------
class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	//--------------------------------------------------------------
	void newStars();
	
	bool reset; // true：resetボタンを押す　・役割：これまで描いた星座を消す
	bool save; // true：saveボタンを押す ・役割：セーブ中の確認画面を描くようにする
	bool new_sky; // true：newボタンを押す　・役割：星の配置を新しくする
	bool drawing_line; // true：線を引き始める
	bool writing_imgfile; // true：イメージを保存する作業の間

	static const int max_stars_num = 60;

	vector<Stars> stars;
	vector<int> selected_stars_idx; // 選んだ星のインデックス
	vector<ofPolyline*> line; // これまで引いたPolylineを格納
	ofPoint picked; // 直前にクリックした星の座標

	const ofColor color_blueSky = ofColor(0, 51, 77);
	const ofColor color_sunset = ofColor(90, 10, 90);
	const ofColor color_star = ofColor(255, 255, 128);

	Button btn_save;
	Button btn_reset;
	Button btn_new;

	float current_width;
	float current_height;
	
	ofImage img;
	void saveImage();

	//--------------------------------------------------------------
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
