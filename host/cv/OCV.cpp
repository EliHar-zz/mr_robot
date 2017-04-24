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
#include <stdio.h>
#include <ctime>
#include <unistd.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

// Keep track of video camera frame number
long frameNumber = 0;

// Write to files every time the counter reaches zero
const int FRAME_EVERY = 3;
int currentFrame = FRAME_EVERY;

// Assign unique ID for each direction. 
// The ID must be in sync with the GUI direction values 
// (Please refer to the documentation for more information about the GUI code)

int direction = 0;
clock_t start_time;

const int FORWARD = 1;
const int REVERSE = 2;
const int RIGHT = 3;
const int LEFT = 4;
const int ROTATE = 5;
const int PAUSE = 6;
const int STOP = 7;

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

void drive(int left, int right) {

	stringstream ss;
	ss << "/home/debian/mr_robot/tools/control/write " << left << "," << right << "#" << endl;
	cout << left << "," << right << "#" << endl;
	system(ss.str().c_str());
}

/**
* Drive the car based on the angle and distance
* Decisions are mainly taken based on experiments
* @param angle Car angel
* @param distance Distance of the car from the camera
* @param diameter Digital diameter of the circular object
* @param loop_count Necessary to keep from writing to Atmega328p faster than it can read messages
* @return Direction code
**/
void find_ball(double angle, double distance, double diameter, int &loop_count) {

	int full_speed = 255;
	int rotation_speed = 140;
	int turn_speed = 40;
	int pausing_distance = 45;
	int target_angle = 4;

	if (loop_count > 1) { 

		// If no object found: rotate
		if(diameter == 0 && direction != ROTATE) {
			cout << endl << "rotating ";
			drive(rotation_speed, -rotation_speed);
			direction = ROTATE;
			loop_count = 0;

			start_time = clock();

		// If object is within pausing_distance and visible: pause
		} else if(distance <= pausing_distance && diameter > 0 && direction != PAUSE) {
			cout << endl << "pausing "; 
			drive(0, 0);
			direction = PAUSE;
			loop_count = 0;

			cout << "\n**** BALL FOUND ****\n" << endl;

		// If object more than target_angle degrees to right and farther than pausing distance: turn right
		} else if (angle > target_angle && distance >= pausing_distance && direction != RIGHT){
			cout << endl << "turning right ";
			drive(turn_speed, -turn_speed);
			direction = RIGHT;
			loop_count = 0;

		// If object more than target_angle degrees to left and farther than pausing distance: turn left
		} else if (angle < -target_angle && distance >= pausing_distance && direction != LEFT) {
			cout << endl << "turning left ";
			drive(-turn_speed, turn_speed);
			direction = LEFT;
			loop_count = 0;

		// If ball is past pausing distance and within target_angle: forward
		} else if (distance > pausing_distance && angle < target_angle && angle > -target_angle && direction != FORWARD) {
			cout << endl << "going forward ";
			drive(full_speed, full_speed);
			direction = FORWARD;
			loop_count = 0;

		// If ball rotates ~360 degrees and doesn't see ball: stop
		} else if (direction == ROTATE) {
			clock_t rotation_duration = (clock() - start_time) / (double)(CLOCKS_PER_SEC);
			cout << "\trot time: " << rotation_duration << " s" << endl;
			if (rotation_duration > 12) {
				drive(0, 0);
				cout << "\n**** BALL NOT FOUND ****\n" << endl;
				direction = STOP;
			}
		}
	}

	loop_count++;
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

	// loop_count is used to make sure that the find_ball function has looped at least twice
	// before sending a message to the Atmega328p. This was created in response to a bug found
	// when writing too fast to the Atmega328p. The direction variable would be modified however 
	// the write message wouldn't be interpreted. This causes the car to get stuck in a
	// certain direction. Initialized to 2 so that the car begins moving immediately
	int loop_count = 2;

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
		cout << endl;
		printf("\t%-10s%4.2f\n", "angle:", rotation_angle);
		printf("\t%-10s%4.2f\n", "distance:", distance);
		printf("\t%-10s%4.2f\n", "diameter:", diameter);
		
		// Draw circle at x and y
		Mat tmpSource = imageSrc.clone();
		circle(tmpSource, Point(x_object,y_object), 3, Scalar(229, 240, 76), 2);
		circle(tmpSource, Point(x_object,y_object), diameter/2, Scalar(44, 252, 14), 3);

		// Center
		circle(tmpSource, Point(x_center,y_center), 2, Scalar(255, 255, 255), 2);

		// Logic to navigate to ball
		find_ball(rotation_angle, distance, diameter, loop_count);
		
		// Write images and text into the file system/
		// Director /var/www/html correspond to the path for
		// Apache2 server. All files placed in this directory will be 
		// accessible on all users in the network over host IP and port 80
		
		string path = "/home/debian/mr_robot/dashboard/mr-robot-node/public/debug/";
		string outPath = path + "out.jpg";
		string bwPath = path + "bw.jpg";
		string infoPath = path + "info.txt";                                        

		if(--currentFrame == 0) {
			imwrite(outPath, tmpSource);
			imwrite(bwPath, imageDest);
		}
		ofstream myfile;
		myfile.open (infoPath.c_str());
		myfile << "Distance from camera: " << distance << " cm\n";
		myfile << "Rotation angle: " << rotation_angle << "\n";
		myfile << "Digital diameter: " << diameter << " px\n";
		myfile << "Frame number: " << ++frameNumber << "\n";

		string dir_message = "Direction: ";
		switch (direction) {
			case FORWARD: 
				myfile << dir_message << "Forward" << endl;
				break;
			case REVERSE:
				myfile << dir_message << "Reversing" << endl;
				break;
			case RIGHT:
				myfile << dir_message << "Right" << endl;
				break;
			case LEFT:
				myfile << dir_message << "Left" << endl;
				break;
			case ROTATE:
				myfile << dir_message << "Rotating" << endl;
				break;
			case PAUSE:
				myfile << dir_message << "Pause" << endl;
				break;
			case STOP:
				myfile << dir_message << "Stop" << endl;
				break;
		}
		myfile << "DIR_CODE: " << direction << "\n";
		myfile.close();

		if (direction == STOP) {
			return 0;
		}

	}

	return 0;
}
