#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAntiAliasing();
    ofEnableDepthTest();
    ofDisableArbTex();
    ofSetWindowShape(1400,900);
    ofBackground(ofColor::black);
    shader.load("shader");

    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    ico.set(1,8);
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
    s.numSamples =16;
    s.useStencil = true;
    s.depthStencilAsTexture =true;
    s.internalformat =GL_RGBA;
    fbo.allocate(s);
    
    

    sample.load(ofToDataPath("output.wav"));

        

    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */

    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    // the higher the value, the more accurate will be the fft analysis
    fftSize = 1024;
    fft.setup(fftSize, 512, 256);
    /* this has to happen at the end of setup - it switches on the DAC */
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    something =0.0f;
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

for (int i = 0; i < fftSize; i++)
{
    something = ofLerp(0.0f,1,fft.magnitudes[i]);
}



//float something = float(ofGetWidth()) / float(fftSize) /2.f;
       
    // for(int i = 0; i < fftSize ;i++){
    //     int r = float(255) / float(fftSize) * i;
    //     int g = 40;
    //     int b = 255 - r;
    //     //ofSetColor(r, g, b);
    //     float outvar = 0.0f;
      //  ofLog()<<fft.magnitudes[i];
        
// float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp=false)
     // waves = ofMap(outvar,fft.magnitudes[i],fft.magnitudes[i]*2,0.000,100.0);

        // ofDrawCircle(ofGetWidth()/2 +something * i,
        //              ofGetHeight()/2, fft.magnitudes[i] * 2);

        // ofDrawCircle(ofGetWidth()/2 -something * i,
        //              ofGetHeight()/2, fft.magnitudes[i] * 2);
        //ofDrawRectangle(i * 2, ofGetHeight(), 2, -(fft.magnitudesDB[i]) * 8);
    //}


shader.begin();
    shader.setUniform1f("perlins",1.0);
    shader.setUniform1f("time",ofGetElapsedTimef());
    shader.setUniform1f("pointscale", 10.0);
    shader.setUniform1f("decay",decay );
    shader.setUniform1f("complex", 0.0);
    shader.setUniform1f("waves",waves  * threshold );
    shader.setUniform1f("eqcolor", eqcolor );
    shader.setUniform1i("fragment",false);
    shader.setUniform1f("dnoise", 0.0 );
    shader.setUniform1f("qnoise", 4.0);
    shader.setUniform1f("r_color", red *something   );
    shader.setUniform1f("g_color", green  );
    shader.setUniform1f("b_color",blue );
    shader.setUniform1i("speed",speed);
  
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
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
 float rms = 0.0;
    int numCounted = 0;

    for (int i = 0; i < bufferSize; i++){
        wave = sample.play();
        //fft
        if(fft.process(wave)){
            fft.magsToDB();
        }

        mymix.stereo(wave, outputs, 0.5);
        output[i*nChannels    ] = outputs[0];
        output[i*nChannels + 1] = outputs[1];
        //beat detection 

        float left = outputs [0];
        float right = outputs[1];
        rms += left * left;
        rms += right * right;
        numCounted += 2;
    }
    rms /= (float)numCounted;
    rms = sqrt(rms);

    if (enableSmooth) {
        smoothedVolume *= smoothedVal;
        smoothedVolume += 0.07 * rms;
    } else {
        smoothedVolume = rms;
    }

    //onset detection as in http://openframeworks.cc/ofBook/chapters/sound.html
    threshold = ofLerp(threshold, minThreshold, decayRate);
    if(rms > threshold) {
        threshold = rms;
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
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
    if (key == 'p')
    {
       
    }
    if (key == 's')
    {
        // sample.stop();
    }
    
    

}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fbo.allocate(ofGetWidth(),ofGetHeight(),16); 
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
             if (ofxImGui::BeginTree(this->sound,mainSettings))
            {
                ofxImGui::AddParameter(this->smoothedVal);
                ofxImGui::AddParameter(this->minThreshold);
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