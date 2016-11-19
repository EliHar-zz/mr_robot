#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {

	// Create matrix from image
	Mat imageSrc = imread("bb1.jpg", CV_LOAD_IMAGE_COLOR);

	// HSV low-high values
	int lowH = 0;
	int highH = 179;

	int lowS = 0;
	int highS = 255;

	int lowV = 0;
	int highV = 255;

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

	cout << x << y << endl;

	return 0;
}
