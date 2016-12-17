#include <iostream>
#include <cstring>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>
#include <math.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

double getDistance(double realDimention, double digitalDimention) {
	double FOCAL_LENGTH = 948.6071428572; // in pixels
	int ERROR_MARGIN = 18; //pixels lost due to selection of circular shape
	return realDimention * FOCAL_LENGTH / (digitalDimention + ERROR_MARGIN);
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

int main( int argc, char** argv ) {

	string image_file_name = argv[1];
	string color = argv[2];

	Mat imageSrc = imread(image_file_name, CV_LOAD_IMAGE_COLOR);

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
	double rotation_angle = atan((x_object - x_center)/distance) * 180 / PI;;

	cout << "distance is: "<< distance << " "<< rotation_angle << " "<< diameter <<  endl;

	int n = system("echo test");

	// Draw circle at x and y
	Mat tmpSource = imageSrc.clone();
	circle(tmpSource, Point(x_object,y_object), 3, Scalar(229, 240, 76), 4);
	circle(tmpSource, Point(x_object,y_object), diameter/2, Scalar(44, 252, 14), 5);

	// Center
	circle(tmpSource, Point(x_center,y_center), 10, Scalar(255, 255, 255), 10);

	// Show images in windows
	imshow("Destination", imageDest);
	imshow("Source", tmpSource);

	waitKey(0);

	return 0;
}
