#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myLidarLite = ThreadedLidarLite();
	myLidarLite.begin();
	
	// Exit if the lidar lite didn't initialize properly
	if (!myLidarLite.hasBegun()) ofApp::exit();
	
	// Print the hardware version of the Lidar Lite
	cout << "LIDAR Lite hardware version: " << myLidarLite.hardwareVersion() << endl;
	cout << "LIDAR Lite software version: " << myLidarLite.softwareVersion() << endl;
	
	wDistance = -1;
    
    myLidarLite.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    int success = myLidarLite.startRead();
    if (success) {
        cout << "SUCCESS: myLidarLite.startRead();" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSleepMillis(10);
	
	cout << "Frame rate = " << std::fixed << std::setw( 5 ) << std::setprecision( 2 ) 
          << std::setfill( '0' ) <<ofGetFrameRate() << " Hz, ";
          		
    if (myLidarLite.isOutputNew()) {
        int distance = -1;
        int signalStrength = -1;
        
        // Read the distance and signalStrength
        myLidarLite.getOutput(distance, signalStrength);
        
        cout << "Distance = " << distance << " cm, ";
        
        // Read the status (useful for debug)
        //int status = myLidarLite.status();
        //cout << myLidarLite.statusString(status);
        
        // Power user technique: 
        // Weighting the new distance value by the measured signal strength
        // Helps eliminate noise created by the sun and by not detecting any objects 
        int minSigStrength = 20; // signal strength minimum
        int fullSigStrength = 80; // signal strength that gets full weight
        float weight;
        if (signalStrength < minSigStrength) {
            // We're below the min signal strength
            // Set distance to -1 to indicate no object was found
            wDistance = -1; // Indicates no object was found
        } else {
            // Weight by the signal strength
            weight = ofMap(signalStrength, minSigStrength, fullSigStrength, 0.05f, 1.f, true);
            // Calculate the weighted distance
            wDistance = ((float) distance)*weight + ((float) wDistance)*(1-weight);
        }
        cout << "wDistance = " << wDistance << " cm, ";
        cout << "signalStrength = " << signalStrength << ", ";
        cout << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

void ofApp::exit() {
	// Stop running threads
	myLidarLite.stop();
}