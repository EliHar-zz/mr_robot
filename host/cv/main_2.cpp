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

#define PI 3.14159265

using namespace cv;
using namespace std;

double getDistance(double realDimention, double digitalDimention) {
	double FOCAL_LENGTH = 948.6071428572; // in pixels
	int ERROR_MARGIN = 18; //pixels lost due to selection of circular shape
	return realDimention * FOCAL_LENGTH / (digitalDimention + ERROR_MARGIN);
}

void printCamConfig(VideoCapture &cam) {
	cout << cam.get(CV_CAP_PROP_POS_MSEC) << endl;
	cout << cam.get(CV_CAP_PROP_POS_FRAMES) << endl;
	cout << cam.get(CV_CAP_PROP_POS_AVI_RATIO) << endl;
	cout << cam.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << cam.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	cout << cam.get(CV_CAP_PROP_FPS) << endl;
	cout << cam.get(CV_CAP_PROP_FOURCC) << endl;
	cout << cam.get(CV_CAP_PROP_FRAME_COUNT) << endl;
	cout << cam.get(CV_CAP_PROP_FORMAT) << endl;
	cout << cam.get(CV_CAP_PROP_MODE) << endl;
	cout << cam.get(CV_CAP_PROP_BRIGHTNESS) << endl;
	cout << cam.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << cam.get(CV_CAP_PROP_SATURATION) << endl;
	cout << cam.get(CV_CAP_PROP_HUE) << endl;
	cout << cam.get(CV_CAP_PROP_GAIN) << endl;
	cout << cam.get(CV_CAP_PROP_EXPOSURE) << endl;
	cout << cam.get(CV_CAP_PROP_CONVERT_RGB) << endl;
	cout << cam.get(CV_CAP_PROP_RECTIFICATION) << endl;	
}

Point edgePoint(Mat imageDest, Point def) {
	int thresh = 100;

	Canny(imageDest, imageDest, thresh /*threshold1*/, thresh*2 /*threshold2*/, 3/*apertureSize*/);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imageDest, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	if(contours.size() == 0) {
		return def;
	}
	return contours[0][0];
}

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

void drive(double angle, double distance) {
	int MAX_ANGLE = 45;

	int PWM_RANGE = 55;
	int MIN_PWM = 200;
	int MAX_PWM = 255; // for back wheels (might be change to respect distace to cover)

	double fr_left, fr_right, back;

	if (angle > 0){
		
		// Go right

		fr_left = PWM_RANGE - (angle / MAX_ANGLE) * PWM_RANGE + MIN_PWM;

		fr_right = (angle / MAX_ANGLE) * PWM_RANGE + MIN_PWM;

		back = MAX_PWM;
	} else if (angle < 0) {

		// Go left

		fr_right = PWM_RANGE - (-angle / MAX_ANGLE) * PWM_RANGE + MIN_PWM;

		fr_left = (-angle / MAX_ANGLE) * PWM_RANGE + MIN_PWM;

		back = MAX_PWM;
	} else {
		
		// Go straight
		if (distance >= 100.0) {
			fr_right = MAX_PWM;
			fr_left = MAX_PWM;
			back = MAX_PWM;
		} else {
			fr_right = 200;
			fr_left = 200;
			back = 200;
		}
	}

	string cmd = string("/root/mr_robot/tools/lab/lab_5/write ") + to_string((int)fr_left) + "," + to_string((int)fr_right) + "," + to_string((int)back) + string("#"); 
	cout << cmd << endl;
	system(cmd.c_str());
}

int main( int argc, char** argv ) {

	int cap_num  = atoi(argv[1]);
	string color = argv[2];

	 VideoCapture cap;

	 if(!cap.open(cap_num))
		 return 1;
	//printCamConfig(cap);

	// Configure cam
	cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,180);
	// cap.set(CV_CAP_PROP_FRAME_COUNT, 100); // Not supported by cam
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));


	while(1) {
		Mat imageSrc;
		cap >> imageSrc;

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
		erode(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)) );
		dilate(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)) );

		// Morphological closing
		dilate(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)) );
		erode(imageDest, imageDest, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)) );

		// Create moment
		Moments mmts = moments(imageDest);

		// Calculate center x and y (Centroids)
		double x_object = mmts.m10 / mmts.m00;
		double y_object = mmts.m01 / mmts.m00;

		// Center of image
		cv::Size size = imageSrc.size();
		double x_center = size.width/2.0f;
		double y_center = size.height/2.0f;

		// contour
		Mat tmpDest = imageDest.clone();
		Point point = edgePoint(tmpDest, Point(x_center, y_center));

		double diameter = norm(Point(x_object, y_object)- point)*2;
		double realDiameter = 6.5;

		double distance = getDistance(realDiameter, diameter);

		// Get rotation angle
		int digitalDiff = x_object - x_center;
		double realDiff = digitalDiff * (realDiameter / diameter);
		double rotation_angle = atan(realDiff / distance) * 180 / PI;

		cout << "distance is: "<< distance << " "<< rotation_angle << " "<< diameter <<  endl;

		// Draw circle at x and y
		Mat tmpSource = imageSrc.clone();
		circle(tmpSource, Point(x_object,y_object), 3, Scalar(229, 240, 76), 4);
		circle(tmpSource, Point(x_object,y_object), diameter/2, Scalar(44, 252, 14), 5);

		// Center
		circle(tmpSource, Point(x_center,y_center), 10, Scalar(255, 255, 255), 10);

		imwrite("/var/www/html/mr_robot/out.jpg", tmpSource);
		imwrite("/var/www/html/mr_robot/bw.jpg", imageDest);
		ofstream myfile;
		myfile.open ("/var/www/html/mr_robot/info.txt");
		myfile << "Distance from camera: " << distance << " cm\n";
		myfile << "Rotation angle: " << rotation_angle << "\n";
		myfile << "Digital diameter: " << diameter << " cm\n";
		myfile.close();

		drive(rotation_angle, distance);
	}

	// Show images in windows
//	imshow("Destination", imageDest);
//	imshow("Source", tmpSource);

//	waitKey(0);

	return 0;
}
