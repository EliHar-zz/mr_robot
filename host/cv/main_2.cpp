#include <iostream>
#include <cstring>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

double getDistance(double realDimention, double digitalDimention) {
	double FOCAL_LENGTH = 638.111425; // in pixels
	int ERROR_MARGIN = 18; //pixels lost due to selection of circular shape
	return realDimention * FOCAL_LENGTH / (digitalDimention + ERROR_MARGIN);
}

Point edgePoint(Mat imageDest) {
	int thresh = 100;

	Canny(imageDest, imageDest, thresh /*threshold1*/, thresh*2 /*threshold2*/, 3/*apertureSize*/);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imageDest, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	return contours[0][0];
}

int main( int argc, char** argv ) {

	// Create matrix from imagea
	Mat imageSrc = imread("1.jpg", CV_LOAD_IMAGE_COLOR);
	int blue_lower[] = {100,115,50};
	int blue_higher[] = {130,255,255};

	int red_lower[] = {0,160,99};
	int red_higher[] = {36,255,255};

	int green_lower[] = {60,100,20};
	int green_higher[] = {98,239,255};

	// HSV low-high values
	int lowH = blue_lower[0];
	int highH = blue_higher[0];

	int lowS = blue_lower[1];
	int highS = blue_higher[1];

	int lowV = blue_lower[2];
	int highV = blue_higher[2];

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
	double x = mmts.m10 / mmts.m00;
	double y = mmts.m01 / mmts.m00;

	cv::Size size = imageSrc.size();
	double x_center = size.width/2.0f;
	double y_center = size.height/2.0f;

	// contour
	Mat tmpDest = imageDest.clone();
	Point point = edgePoint(tmpDest);

	double diameter = norm(Point(x_center, y_center)- point)*2;

	double distance = getDistance(7.5, diameter);

	cout << "diameter is: "<< diameter << endl;
	cout << "distance is: "<< distance << endl;

	// Draw circle at x and y
	Mat tmpSource = imageSrc.clone();
	circle(tmpSource, Point(x,y), 10, Scalar(0, 120, 200), 10);
	circle(tmpSource, point, 10, Scalar(120, 0, 0), 10);

	// Show images in windows
	imshow("Destination", imageDest);
	imshow("Source", tmpSource);

	waitKey(50000);

	return 0;
}
