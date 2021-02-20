#pragma once

#include "ofMain.h"
#include "ofxAutoReloadedShader.h"
#include "ofxImGui.h"
#include "MyTheme.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		ofxAutoReloadedShader shader;
		
		ofMesh mesh;
		ofIcoSpherePrimitive ico;
		ofCamera cam;
		

		ofImage imgSave;
		ofLight ambient, point;

	private:	
	bool imGui();
	ofxImGui::Gui gui;
	bool mouseOverGui;
	bool guiVisible;
	//save data
		string XML_path ="settings.xml";
		void XML_load_app(ofParameterGroup &g, string path);
		void XML_save_app(ofParameterGroup &g, string path);
	//end save data

	ofParameterGroup init{"Setup"};

	ofParameter<float>speed{"speed",0.1,0.001,1};
	ofParameter<float>zoom{"zoom",7.6,-10.0,100};

	ofParameterGroup perlin{"Perlin"};
	ofParameter<float>decay{"decay",0.35,0,1.0};
	ofParameter<float>waves{"waves",2,0,100};

	ofParameterGroup color{"Color"};
	ofParameter<float>eqcolor{"eqColor",3.0,0.0,50.0};
	ofParameter<float>red{"Red",0.4,0.0,1.0};
	ofParameter<float>green{"Green",0.4,0.0,1.0};
	ofParameter<float>blue{"Blue",0.4,0.0,1.0};
};
