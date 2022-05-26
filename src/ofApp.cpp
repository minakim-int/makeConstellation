#include "ofApp.h"
#include  <ctime> // イメージファイル名指定の際に現在時刻を呼出するため

/* 
 * 製作：KIM MINA（崇実大学校）
 * 以下はopenFrameWorksの構造の簡単な説明です。
 * 
 * setup()が最初に呼び出された後、終了までupdate()とdraw()が交互に呼び出される。
 * setup()：変数などを初期化
 * update()：動作中のアプリケーションの状態をアップデート
 * draw()：画面に絵を描く
 * 
 */


//--------------------------------------------------------------
void ofApp::setup(){
	stars.resize(max_stars_num);
	newStars();
	current_width = ofGetWidth();
	current_height = ofGetHeight();
}
//--------------------------------------------------------------
void ofApp::update() {
	//* resetボタンを押した場合、これまで作った星座を消す
	if (reset) {
		for (auto i = 0; i < line.size(); i++) {
			line[i]->clear();
		}
		while (selected_stars_idx.size() != 0) {
			selected_stars_idx.pop_back();
		}
		reset = false;
	}

	//* newボタンを押した場合、これまで作った星座を消す・星は再配置する
	if (new_sky) {
		newStars();
		new_sky = false;
	}

	//* windowサイズが変更された場合、星座の位置再調整
	for (auto i = 0; i < stars.size(); i++) {
		// 星の位置再調整
		stars[i].rad *= ofGetWidth() / current_width;
		stars[i].mag = stars[i].rad + 3;
		stars[i].coord.x *= ofGetWidth() / current_width;
		stars[i].coord.y *= ofGetHeight() / current_height;
	}
	for (auto i = 0; i < line.size(); i++){
		// 線の位置再調整
		for (auto& vert : line[i]->getVertices()) {
			vert.x *= ofGetWidth()/current_width;
			vert.y *= ofGetHeight()/current_height;
		}
	}
}
//--------------------------------------------------------------
void ofApp::draw(){
	//* 背景色を塗る
	auto bg_pct = cos(ofGetElapsedTimef()*0.05f);
	ofBackground((1-bg_pct)*color_blueSky + (bg_pct * color_sunset)+1);

	float program_timer = ofGetElapsedTimef();
	float twinkling_offset = program_timer * 4.0f - 6.0f * floorf(program_timer * 4.0f/6.0f);
	for (auto i = 0; i < stars.size(); i++) {
		// 星にマウスを近づけると光るように見せる
		if (abs(ofDist(stars[i].coord.x, stars[i].coord.y, mouseX, mouseY)) <= stars[i].mag) {
			stars[i].drawTwinkling(color_star, twinkling_offset);
		}
	}

	//* 星を描く
	ofSetColor(color_star);
	if (save) {
		// セーブ中の確認画面ではこれまで選択した星以外透明になる
		ofSetColor(color_star, 98); 
	}
	for (auto i = 0; i < stars.size()/2; i++) {
		// 星たちの光彩をインデックス基準で半分に分けて設定
		stars[i].drawTwinkling(color_star, 4);
		if(stars.size()/2 > 0) stars[i + (stars.size()/2 - 1)].drawTwinkling(color_star, 6); // stars[-1]に接近するのを防ぐ
	}
	for (auto i = 0; i < stars.size(); i++) {
		ofSetColor(color_star);
		stars[i].drawStar('r');
	}
	ofSetColor(color_star);
	if (save) {
		// セーブ中の確認画面ではこれまで選択した星が大きく見える
		for (auto i = 0; i < selected_stars_idx.size(); i++) {
			stars[selected_stars_idx[i]].drawStar('m');
		}
	}

	//* 線を描く
	if (drawing_line) {
		ofDrawLine(picked.x, picked.y, mouseX, mouseY);
	}
	for (auto i = 0; i < line.size(); i++) {
		line[i]->draw();
	}

	//* イメージファイルを作って保存する
	if (writing_imgfile) {
		saveImage();
		writing_imgfile = false; 
	}

	//* ボタンを描く
	btn_reset.drawButton(save, mouseX, mouseY);
	btn_save.drawButton(save, mouseX, mouseY);
	btn_new.drawButton(save, mouseX, mouseY);

	//* 現在のwindowサイズを格納
	current_width = ofGetWidth();
	current_height = ofGetHeight();
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//* 星をクリックした場合
	for (auto i = 0; i < stars.size(); i++) {
		int dis = abs(ofDist(stars[i].coord.x, stars[i].coord.y, x, y));
		if (dis <= stars[i].mag) {
			drawing_line = true;
			picked = stars[i].coord;
			if (!selected_stars_idx.empty()) {
				// 正確にクリックしなくても選択できるようにする
				bool rePicked = abs(ofDist(stars[selected_stars_idx.back()].coord.x, stars[selected_stars_idx.back()].coord.y, x, y)) <= ofGetWidth()/(default_width/5);
				if (rePicked) {
					// 直前と同じ星を選択した場合、線を引きやめる
					drawing_line = false;
					line.back()->end();
					line.push_back(new ofPolyline);
					break;
				}
			}
			selected_stars_idx.push_back(i); 
			line.back()->addVertex(picked);
		}
	}	

	//* ボタンをクリックした場合
	if (btn_save.isIn(x, y)) { 
		save = true; writing_imgfile = true;
	}
	else save = false;
	reset = btn_reset.isIn(x, y); 
	new_sky = btn_new.isIn(x, y); 
}
//--------------------------------------------------------------
void ofApp::newStars() {
	//* 星座の初期化
	for (auto i = 0; i < line.size(); i++) {
		line[i]->clear(); //reset PolyLine
	}
	drawing_line = false; //line being invisible
	for (int i = 0; i < stars.size(); i++) {
		stars[i].setStarsPos(); //reset Stars
	}
	selected_stars_idx.clear();
	reset = false;
	new_sky = false;
	line.push_back(new ofPolyline);
}
//--------------------------------------------------------------
void ofApp::saveImage() {
	time_t real_timer = time(NULL);
	struct tm* t = localtime(&real_timer);

	string filename = "Constellation-";
	img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	filename = filename + to_string(t->tm_year + 1900) + "-" + to_string(t->tm_mon+1) + "-"
		+ to_string(t->tm_mday) + "-" + to_string(t->tm_hour) + to_string(t->tm_min) + to_string(t->tm_sec);
	img.save(filename.append(".jpg"), OF_IMAGE_QUALITY_BEST);
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	//* Windowサイズが変わるにつれてボタンの位置再調整
	const int btn_yoffset = h - 35;
	btn_reset.setButton(w - 137, btn_yoffset, "reset");
	btn_save.setButton(w - 70, btn_yoffset, "save");
	btn_new.setButton(w - 190, btn_yoffset, "new", 35, 20);
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
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


