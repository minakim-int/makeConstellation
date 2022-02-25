#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	reset = false;
	//-----Set vector size-----//
	stars.resize(MAX_STARS);
	listofStars.resize(MAX_STARS);
	//-----Initialize Stars-----// 
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].setStars();
	}
	//-----Listing Coordination of Stars-----//
	for (int i = 0; i < MAX_STARS; i++) {
		listofStars[i] = stars[i].s_coord;
	}
	//-----first line-----//
	line.push_back(new ofPolyline);
	resetButton.setButton(ofGetWidth()-140, ofGetHeight()-35, "reset");
	saveButton.setButton(ofGetWidth()-70, ofGetHeight()-35, "save");
}
//--------------------------------------------------------------
void ofApp::update(){
	if (reset) {
		resetStars();
	}
}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 51, 77);
	//-------------------Draw Stars----------------------//
	ofSetColor(255, 255, 128);
	if (save) ofSetColor(255, 255, 128, 98); //if Save Button Pressed, non-selected Stars opacity decrease
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].drawStar('r');
	}
	//-------------------Draw Current Line-----------------------//
	ofSetColor(255, 255, 128);
	if (drawingLine) {
		ofDrawLine(pickedX, pickedY, mouseX, mouseY);
	}
	//-----------------Nearest Star Shines----------------------//
	for (int i = 0; i < MAX_STARS; i++) {
		if (abs(ofDist(listofStars[i].x, listofStars[i].y, mouseX, mouseY)) <= stars[i].s_mag) {
			stars[i].drawStar('m');
		}
	}
	//-------------------Draw Existing Line---------------------//
	for (int i = 0; i < line.size(); i++) {
		line[i]->draw();
	}
	//----------------Show Saved Constellation-------------------//
	if (save) {
		for (int i = 0; i < selectedStarsIdx.size(); i++) {
			ofSetColor(255, 255, 128);
			stars[selectedStarsIdx[i]].drawStar('m');
		}
		img.grabScreen(0, 0, 600, 450);
		img.save("Constellation.jpg", OF_IMAGE_QUALITY_BEST);
	}
	ofSetColor(255);
	//-------------------Draw Buttons--------------------------//
	if (resetButton.isIn(mouseX, mouseY, 'r')) ofSetColor(200);
	if (save) ofSetColor(255, 50);
	resetButton.drawButton();
	
	if (saveButton.isIn(mouseX, mouseY, 's')) ofSetColor(200);
	if (save) ofSetColor(255, 50);
	saveButton.drawButton();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//-----Start Drawing Line-----//
	//-----Pick Nearest Star from Pressed location-----//
	for (int i = 0; i < MAX_STARS; i++) {
		int dis = abs(ofDist(listofStars[i].x, listofStars[i].y, x, y));
		if (dis <= stars[i].s_mag) {
			drawingLine = true;
			pickedX = listofStars[i].x;
			pickedY = listofStars[i].y;
			//-----if same star selected again-----//
			if (!selectedStarsIdx.empty() && 
				abs(ofDist(listofStars[selectedStarsIdx.back()].x, 
					listofStars[selectedStarsIdx.back()].y, 
						x, y))<=5) {
				drawingLine = false; 
				line[line.size() - 1]->end(); 
				//-------Add New Line for Next Selection-------//
				line.push_back(new ofPolyline); 
				break;
			}
			selectedStarsIdx.push_back(i); //Store index of Selected Stars
			line[line.size()-1]->addVertex(pickedX, pickedY);
		}
	}	
	save = saveButton.isIn(x, y, 's'); //Save Button Pressed
	reset = resetButton.isIn(x, y, 'r'); //Reset Button Pressed
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
void Stars::drawStar(char mode) const{
	if (mode == 'r')
		ofDrawCircle(s_coord, s_rad);
	else if (mode == 'm')
		ofDrawCircle(s_coord, s_mag);
}
void Button::drawButton() const{
	ofDrawRectangle(b_coord, b_width, b_height);
	ofSetColor(0);
	ofDrawBitmapString(b_name, b_coord.x+5, b_coord.y+15);
	ofSetColor(255);
}
bool Button::isIn(int x, int y, char mode) const {
	int xpos;
	if(mode == 'r')
		xpos = ofGetWidth() - 140;
	if (mode == 's')
		xpos = ofGetWidth() - 70;
	int ypos = ofGetHeight() - 35;
	if(x > xpos && x < xpos+50 && y < ypos+20 && y > ypos) { 
		return true;
	}
	else {
		return false;
	}
}
void ofApp::resetStars() {
	for (int i = 0; i < line.size(); i++) {
		line[i]->clear(); //reset PolyLine
	}
	drawingLine = false; //line being invisible
	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].setStars(); //reset Stars
	}
	for (int i = 0; i < MAX_STARS; i++) {
		listofStars[i] = stars[i].s_coord; //overwrite Stars' coordinate lists
	}
	selectedStarsIdx.clear();
	reset = false;
}

