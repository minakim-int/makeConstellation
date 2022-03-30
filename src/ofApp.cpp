#include "ofApp.h"
#include  <ctime>

//--------------------------------------------------------------
void ofApp::setup(){
	reset = false;
	newSky = false;
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
	resetButton.setButton(ofGetWidth()-137, ofGetHeight()-35, "reset");
	saveButton.setButton(ofGetWidth()-70, ofGetHeight()-35, "save");
	newButton.setButton(ofGetWidth()-190, ofGetHeight()-35, "new", 35, 20);
}
//--------------------------------------------------------------
void ofApp::update(){
	if (reset) {
		for (int i = 0; i < line.size(); i++) {
			line[i]->clear();
		}
		while (selectedStarsIdx.size() != 0) {
			selectedStarsIdx.pop_back();
		}
		reset = false;
		//resetStars();
	}
	if (newSky) {
		resetStars();
		newSky = false;
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
	}
	if (writing_file) {
		time_t timer = time(NULL);
		struct tm* t = localtime(&timer);

		string filename = "Constellation-";
		img.grabScreen(0, 0, 600, 450);
		filename = filename + to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon+1) + "-"
			+ to_string(t->tm_mday) + "-" + to_string(t->tm_hour) + to_string(t->tm_min) + to_string(t->tm_sec);
		img.save(filename.append(".jpg"), OF_IMAGE_QUALITY_BEST);
		writing_file = false; 
	}

	//-------------------Draw Buttons--------------------------//
	ofSetColor(255);
	if (resetButton.isIn(mouseX, mouseY)) ofSetColor(200);
	if (save) ofSetColor(255, 50);
	resetButton.drawButton();
	
	if (saveButton.isIn(mouseX, mouseY)) ofSetColor(200);
	if (save) ofSetColor(255, 50);
	saveButton.drawButton();

	if (newButton.isIn(mouseX, mouseY)) ofSetColor(200);
	if (save)ofSetColor(255, 50);
	newButton.drawButton();
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
					listofStars[selectedStarsIdx.back()].y, x, y))<=5) {
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
	if (saveButton.isIn(x, y)) { //Save Button Pressed
		save = true; writing_file = true;
	}
	else save = false;
	reset = resetButton.isIn(x, y); //Reset Button Pressed
	newSky = newButton.isIn(x, y); //New Button Pressed
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
	ofDrawBitmapString(b_name, b_coord.x+6, b_coord.y+15);
	ofSetColor(255);
}
bool Button::isIn(int x, int y) const {
	if(x > b_coord.x && x < b_coord.x+b_width && y < b_coord.y+b_height && y > b_coord.y) { 
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

