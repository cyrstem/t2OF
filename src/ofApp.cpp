#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    glShadeModel(GL_SMOOTH);
    ofEnableDepthTest();
    ofSetWindowShape(1400,900);
    ofBackground(ofColor::black);
    shader.load("shader");

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    ico.set(1,6);
    mesh =ico.getMesh();
    this->gui.setup();
     gui.setTheme(new MyTheme());
    this->guiVisible = true;

    ambient.setAmbientColor(ofFloatColor::white);
    point.setPointLight();     
    point.setPosition(20,20,120); 

   

    //save config
    XML_load_app(init, XML_path);
        init.add(speed);
        init.add(zoom);
        init.add(decay);
        init.add(waves);
        init.add(eqcolor);
        init.add(red);
        init.add(green);
        init.add(blue);

    //fbo
    ofFboSettings s;
    s.width = ofGetWidth();
    s.height =ofGetHeight();
    s.useDepth = true;
    s.numSamples =8;
    s.internalformat =GL_RGBA32F;
    fbo.allocate(s);
        
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.setPosition(0,0,zoom);
    fbo.begin();
    ofClear(0);
    fbo.end();
   
}

//--------------------------------------------------------------
void ofApp::draw(){
fbo.begin();
cam.begin();
ofSetColor(255);
ofEnableLighting();
point.enable();
ambient.enable();
shader.begin();
    shader.setUniform1f("perlins",1.0);
    shader.setUniform1f("time",ofGetElapsedTimef());
    shader.setUniform1f("pointscale", 10.0);
    shader.setUniform1f("decay",decay);
    shader.setUniform1f("complex", 0.0);
    shader.setUniform1f("waves",waves);
    shader.setUniform1f("eqcolor", eqcolor);
    shader.setUniform1i("fragment",false);
    shader.setUniform1f("dnoise", 0.0);
    shader.setUniform1f("qnoise", 4.0);
    shader.setUniform1f("r_color", red);
    shader.setUniform1f("g_color", green);
    shader.setUniform1f("b_color",blue);
    shader.setUniform1i("speed",speed);
        //ico.draw();
    ofScale(2,2,2);
   
       mesh.draw();
      
      
    shader.end();
     point.draw();
cam.end();
fbo.end();
fbo.draw(0,0); 
// Gui
	this->mouseOverGui = false;
	if (this->guiVisible)
	{
		this->mouseOverGui = this->imGui();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     if (key == OF_KEY_TAB)
    {
        guiVisible = !guiVisible;
    }
       if (key == OF_KEY_RETURN)
    {
        ofToggleFullscreen();
    }

}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fbo.allocate(ofGetWidth(),ofGetHeight()); 
}
//--------------------------------------------------------------
bool ofApp::imGui()
{
    auto mainSettings = ofxImGui::Settings();
    this->gui.begin();
    {
          if (ofxImGui::BeginWindow("Controls", mainSettings, false))
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::SameLine();
            if (ImGui::Button("Save PNG"))
                {
                    imgSave.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
                    imgSave.save("preview/capture"+ofToString(ofRandom(0,1000),0)+".png",OF_IMAGE_QUALITY_BEST); 
                }
            ImGui::SameLine();
             if (ImGui::Button("Load Settings"))
                {
                    XML_load_app(init,XML_path);
                }
            ImGui::SameLine();
             if (ImGui::Button("Save Settings"))
                {
                    XML_save_app(init,XML_path);
                }
             ImGui::Dummy(ImVec2(0.0f, 20.0f));
		{
             if (ofxImGui::BeginTree(this->init,mainSettings))
            {
                ofxImGui::AddParameter(this->speed);
                ofxImGui::AddParameter(this->zoom);
                ofxImGui::EndTree(mainSettings);
            }

             if (ofxImGui::BeginTree(this->perlin,mainSettings))
            {
                ofxImGui::AddParameter(this->decay);
                ofxImGui::AddParameter(this->waves);
                ofxImGui::EndTree(mainSettings);
            }
             if (ofxImGui::BeginTree(this->color,mainSettings))
            {
                 ofxImGui::AddParameter(this->eqcolor);
                 ofxImGui::AddParameter(this->red);
                 ofxImGui::AddParameter(this->green);
                 ofxImGui::AddParameter(this->blue);
                 ofxImGui::EndTree(mainSettings);
            } 
            ofxImGui::EndWindow(mainSettings);
        }
    }
    this->gui.end();
    return mainSettings.mouseOverGui;
}
//--------------------------------------------------------------
void ofApp::XML_load_app(ofParameterGroup &g, string path){
    ofLogNotice("ofApp")<<"XML_load_app"<<path;
    ofXml settings;
    settings.load(path);
    ofDeserialize(settings,g);
}
//--------------------------------------------------------------
void ofApp::XML_save_app(ofParameterGroup &g, string path){
    ofLogNotice("ofApp")<<"XML_save_app"<<path;
    ofXml settings;
    ofSerialize(settings,g);
    settings.save(path);
}
//--------------------------------------------------------------
void ofApp::exit(){
    // XML_save_app(init,XML_path);
}