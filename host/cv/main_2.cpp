/**
* OpenCV (OCV) - Video capture on BBB
*
* GOAL:
* This program takes continuous images from the host camera and detect a circular
* object of specific color. The color and other configuration are provided as 
* input argument to the program.
*
* INPUT:
* The program takes two arguments: 
* - The video device node (0/1)
* - The object color (blue/red)
*
* OUTPUT:
* On each frame captured by the program, three files are exported into the 
* host filesystem (apache directory specifically):
* - A colored image with a green circular around the circular object
* - A binarized image of the colored image (previous bullet)
* - A text file containing information about the object captured
* In addition to storing images, at each frame iteration, the program executes a shell script
* to communicate information about the frame with the ATMEGA328.
*
* HOST REQUIREMENTS:
* The following packages are required to be installed on the host:
* - Apache2 server
* - OpenSSH server
**/

#include <fstream>
#include <iostream>
#include <cstring>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>

#define PI 3.14159265

using namespace cv;
using namespace std;

// Keep track of video camera frame number
long frameNumber = 0;

// Assign unique ID for each direction. 
// The ID must be in sync with the GUI direction values 
// (Please refer to the documentation for more information about the GUI code)
const int DIR_FULL_SPEED = 0;
const int DIR_LEFT = 1;
const int DIR_RIGHT = 2;
const int DIR_HALF_SPEED = 3;
const int DIR_STOP = 4;
const int DIR_REVERSE = 5;

/**
* Get the distance between the object and the camera
* Please refer to the documentation report for more information 
* about the calculations
* @return Distance in cm
**/
double getDistance(double realDimention, double digitalDimention) {
	double FOCAL_LENGTH = 339.079; // in pixels
	int ERROR_MARGIN = 0; //pixels lost due to selection of circular shape
	return realDimention * FOCAL_LENGTH / (digitalDimention + ERROR_MARGIN);
}

/**
* Detect the edge point of the object. This information allows the 
* conclusion of the object digial radius
* @param imageDest Current video frame
* @param def Default point value in case no object was found
* @return arbitrary point on the object edge
**/
Point edgePoint(Mat imageDest, Point def) {
	int thresh = 100;

	// Canny Algorithm for object edge detection 
	Canny(imageDest, imageDest, thresh /*threshold1*/, thresh*2 /*threshold2*/, 3/*apertureSize*/);

	// Prepare data structure to store the edge points
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// Perform the countour finder
	findContours(imageDest, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// If no object foud, return the provided default value
	if(contours.size() == 0) {
		return def;
	}
	
	// Return a point from the countour
	return contours[0][0];
}

/**
* Predefined set of colors that can be detected by the program
* The colors supported by the current program are blue and red 
* @param specs Data strucutre to hold the HSV channel color information
* based on the color specified
* @param color Color of the object (blue/red)
**/
void get_color_specs(vector<vector<int> > &specs, string color){

	if (!color.compare("blue")) {
		specs[0][0] = 100;
		specs[0][1] = 115;
		specs[0][2] = 50;
		specs[1][0] = 130;
		specs[1][1] = 255;
		specs[1][2] = 255;
	} else if (!color.compare("red")) {
		specs[0][0] = 0;
		specs[0][1] = 197;
		specs[0][2] = 109;
		specs[1][0] = 182;
		specs[1][1] = 255;
		specs[1][2] = 255;
	} else {
		specs[0][0] = 255;
		specs[0][1] = 255;
		specs[0][2] = 255;
		specs[1][0] = 255;
		specs[1][1] = 255;
		specs[1][2] = 255;
	}
}

/**
* Drive the car based on the angle and distance
* Decisions are mainly taken based on experiments
* @param angle Car angel
* @param distance Distance of the car from the camera
* @param diameter Digital diameter of the circular object
* @return Direction code
**/
int drive(double angle, double distance, double diameter) {
	
	// Speeds values
	int FULL_SPEED = 255;
	int HALF_SPEED = 220;
	int STOPPED = 0;
	
	// Distance threshold
	int STOPPING_DISTANCE = 30;

	// Wheels speeds variables
	double fr_left, fr_right, back;
	int direction;

	// If no object found, reverse
	if(diameter == 0) {
		fr_left = -FULL_SPEED;	
		fr_right = -FULL_SPEED;	
		back = -FULL_SPEED;
		direction = DIR_REVERSE;

	// If object is within 30 cm, stop
	} else if(distance <= STOPPING_DISTANCE) {
		fr_left = STOPPED;
		back = STOPPED;
		direction = DIR_STOP;

	// If object more than 2 degrees to right, turn right
	} else if (angle > 2){
		fr_left = FULL_SPEED;
		fr_right = STOPPED;
		back = HALF_SPEED;
		direction = DIR_RIGHT;

	// If object more than 2 degrees to left, turn left
	} else if (angle < -2) {
		fr_right = FULL_SPEED;
		fr_left = STOPPED;
		back = HALF_SPEED;
		direction = DIR_LEFT;
		
	// Ball must be directly ahead, drive straight
	} else {
		
		// If ball is greater than 50cm, go full speed
		if (distance > 50.0) {
			fr_right = FULL_SPEED;
			fr_left = FULL_SPEED;
			back = FULL_SPEED;
			direction = DIR_FULL_SPEED;

		// If ball is (30, 50] cm away, approach at slower pace
		} else if(distance > STOPPING_DISTANCE) {
			fr_right = HALF_SPEED;
			fr_left = HALF_SPEED;
			back = HALF_SPEED;
			direction = DIR_HALF_SPEED;
		}
	}

	// Execute shell command to drive the car
	// The file /root/mr_robot/tools/lab/lab_5/write can be found at 
	// https://github.com/EliHar/mr_robot/blob/master/tools/lab/lab_5/write.c
	stringstream s;
	s << "/root/mr_robot/tools/lab/lab_5/write " << (int)fr_left << "," << (int)fr_right << "," << (int)back << "#";
	string cmd = s.str();
	cout << cmd << endl;
	system(cmd.c_str());
	return direction;
}

/**
* Start the OpenCV program
**/
int main( int argc, char** argv ) {

	// Capture video number is: /dev/video(0/1)
	int cap_num  = atoi(argv[1]);
	
	// Color blue/red
	string color = argv[2];

	// Start camera
	 VideoCapture cap;
	 if(!cap.open(cap_num))
		 return 1;

	// Configure the camera for fast capture and good resolution
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 432);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
	cap.set(CV_CAP_PROP_FPS , 30);

	// Keep taking pictures
	while(1) {
		
		// Store the frame in a matrix
		Mat imageSrc;
		cap >> imageSrc;

		// Fetch the color information
		vector<vector<int> > color_specs(2, vector<int>(3));
		get_color_specs(color_specs, color);

		// HSV low-high values
		int lowH = color_specs[0][0];
		int highH = color_specs[1][0];

		int lowS = color_specs[0][1];
		int highS = color_specs[1][1];

		int lowV = color_specs[0][2];
		int highV = color_specs[1][2];

		// Destination image
		Mat imageDest;

		// Convert BGR to HSV
		cvtColor(imageSrc, imageDest, COLOR_BGR2HSV);

		// Get colors in specified range
		inRange(imageDest, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imageDest);

		// Morphological opening
		erode(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		dilate(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		// Morphological closing
		dilate(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		erode(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		// Create moment
		Moments mmts = moments(imageDest);

		// Calculate center x and y (Centroids)
		double x_object = mmts.m10 / mmts.m00;
		double y_object = mmts.m01 / mmts.m00;

		// Center of image
		cv::Size size = imageSrc.size();
		double x_center = size.width/2.0f;
		double y_center = size.height/2.0f;

		// Contour
		Mat tmpDest = imageDest.clone();
		Point point = edgePoint(tmpDest, Point(x_center, y_center));

		// Calculate digital diameter in cm
		double diameter = norm(Point(x_object, y_object)- point)*2;
		double realDiameter = 6.5;

		// Calculate the real distance
		double distance = getDistance(realDiameter, diameter);

		// Get rotation angle
		int digitalDiff = x_object - x_center;
		double realDiff = digitalDiff * (realDiameter / diameter);
		double rotation_angle = atan(realDiff / distance) * 180 / PI;

		// If no object found, then diameter is set to zero
		if(isnan((double)x_object) && isnan((double)y_object)) {
			diameter = 0.0;;
		}

		// Log the calculated information
		cout << "distance is: "<< distance << " "<< rotation_angle << " "<< diameter <<  endl;

		// Draw circle at x and y
		Mat tmpSource = imageSrc.clone();
		circle(tmpSource, Point(x_object,y_object), 3, Scalar(229, 240, 76), 2);
		circle(tmpSource, Point(x_object,y_object), diameter/2, Scalar(44, 252, 14), 3);

		// Center
		circle(tmpSource, Point(x_center,y_center), 2, Scalar(255, 255, 255), 2);

		// Get direction code
		int direction = drive(rotation_angle, distance, diameter);

		// Write images and text into the file system/
		// Director /var/www/html correspond to the path for
		// Apache2 server. All files placed in this directory will be 
		// accessible on all users in the network over host IP and port 80
		imwrite("/var/www/html/mr_robot/out.jpg", tmpSource);
		imwrite("/var/www/html/mr_robot/bw.jpg", imageDest);
		ofstream myfile;
		myfile.open ("/var/www/html/mr_robot/info.txt");
		myfile << "Distance from camera: " << distance << " cm\n";
		myfile << "Rotation angle: " << rotation_angle << "\n";
		myfile << "Digital diameter: " << diameter << " px\n";
		myfile << "Frame number: " << ++frameNumber << "\n";

		string dir_message = "Direction: ";
		switch(direction) {
			case DIR_REVERSE:
				myfile << dir_message << "Reversing"  << "\n";
				break;
			case DIR_FULL_SPEED:
				myfile << dir_message << "Full speed"  << "\n";
				break;
			case DIR_HALF_SPEED:
				myfile << dir_message << "Half speed"  << "\n";
				break;
			case DIR_RIGHT:
				myfile << dir_message << "Right"  << "\n";
				break;
			case DIR_LEFT:
				myfile << dir_message << "Left"  << "\n";
				break;
			case DIR_STOP:
				myfile << dir_message << "Stop"  << "\n";
				break;
		} 
		myfile << "DIR_CODE: " << direction << "\n";
		myfile.close();
	}
	return 0;
}
