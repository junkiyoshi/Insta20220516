#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	vector<glm::vec2> location_list;
	vector<ofColor> color_list;

	ofColor color;
	for (int i = 0; i < 80; i++) {

		auto location = glm::vec2(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -320, 320), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -320, 320));
		location_list.push_back(location);
		color.setHsb(ofRandom(255), 180, 255);
		color_list.push_back(color);
	}

	vector<float> size_list;
	for (int i = 0; i < location_list.size(); i++) {

		for (int k = i + 1; k < location_list.size(); k++) {

			if (i == k) { continue; }

			auto distance = glm::distance(location_list[i], location_list[k]);
			if (distance < 80) {

				auto rad = atan2(location_list[k].y - location_list[i].y, location_list[k].x - location_list[i].x);
				auto location = location_list[i] + (location_list[k] - location_list[i]) * 0.5;

				this->draw_MAGATAMA(location, ofMap(distance, 0, 80, 40, 1), 180 + rad * RAD_TO_DEG, color_list[i]);
				this->draw_MAGATAMA(location, ofMap(distance, 0, 80, 40, 1), rad * RAD_TO_DEG, color_list[k]);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_MAGATAMA(glm::vec2 location, float radius, float rotate_deg, ofColor color) {

	float len = radius * 0.5522;
	float deg = 0;

	ofPushMatrix();
	ofTranslate(location);
	ofRotate(rotate_deg);

	vector<glm::vec2> vertices;
	vector<pair<glm::vec2, glm::vec2>> handle_list;

	vertices.push_back(glm::vec2(0, 0));
	deg = 180;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos(deg * DEG_TO_RAD), len * 0.5 * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos((deg + 180) * DEG_TO_RAD), len * 0.5 * sin((deg + 180) * DEG_TO_RAD)))));

	vertices.push_back(glm::vec2(radius * 0.5, radius * -0.5));
	deg = 90;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos(deg * DEG_TO_RAD), len * 0.5 * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos((deg + 180) * DEG_TO_RAD), len * 0.5 * sin((deg + 180) * DEG_TO_RAD)))));

	vertices.push_back(glm::vec2(0, radius * -1));
	deg = 0;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos(deg * DEG_TO_RAD), len * 0.5 * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * cos(deg * DEG_TO_RAD), len  * sin(deg * DEG_TO_RAD)))));

	vertices.push_back(glm::vec2(radius, 0));
	deg = 270;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * cos(deg * DEG_TO_RAD), len * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * cos((deg + 180) * DEG_TO_RAD), len * sin((deg + 180) * DEG_TO_RAD)))));

	vertices.push_back(glm::vec2(0, radius));
	deg = 0;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * cos(deg * DEG_TO_RAD), len * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos((deg + 180) * DEG_TO_RAD), len * 0.5 * sin((deg + 180) * DEG_TO_RAD)))));

	vertices.push_back(glm::vec2(radius * -0.5, radius * 0.5));
	deg = 90;
	handle_list.push_back(make_pair<glm::vec2, glm::vec2>(
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos(deg * DEG_TO_RAD), len * 0.5 * sin(deg * DEG_TO_RAD))),
		glm::vec2(vertices.back() + glm::vec2(len * 0.5 * cos((deg + 180) * DEG_TO_RAD), len * 0.5 * sin((deg + 180) * DEG_TO_RAD)))));

	ofSetColor(color);
	ofFill();
	ofBeginShape();
	for (int i = 0; i < vertices.size(); i++) {

		int n = (i + 1) % vertices.size();

		ofVertex(vertices[i]);
		ofBezierVertex(handle_list[i].second, handle_list[n].first, vertices[n]);
	}

	ofNextContour(true);
	for (deg = 0; deg <= 360; deg += 5) {

		ofVertex(glm::vec2(0, radius * 0.5) + glm::vec2(radius * 0.2 * cos(deg * DEG_TO_RAD), radius * 0.2 * sin(deg * DEG_TO_RAD)));
	}
	ofEndShape(true);

	ofSetColor(ofColor(255, color.a));
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < vertices.size(); i++) {

		int n = (i + 1) % vertices.size();

		ofVertex(vertices[i]);
		ofBezierVertex(handle_list[i].second, handle_list[n].first, vertices[n]);
	}

	ofNextContour(true);
	for (deg = 0; deg <= 360; deg += 5) {

		ofVertex(glm::vec2(0, radius * 0.5) + glm::vec2(radius * 0.2 * cos(deg * DEG_TO_RAD), radius * 0.2 * sin(deg * DEG_TO_RAD)));
	}
	ofEndShape(true);

	ofPopMatrix();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}