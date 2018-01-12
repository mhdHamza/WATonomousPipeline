#include <opencv\highgui.h>
#include <opencv\cv.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
using namespace cv;


int main()
{
	Mat imgSrc, imgGray, imgBlur, imgFiltered,imgEdges, imgRoadLines;
	
	string imgDir = "solidWhiteRight.mp4";

	VideoCapture cap;
	cap.open(imgDir);


	while (1)
	{
		cap >> imgSrc;
	
		cvtColor(imgSrc, imgGray, CV_RGB2GRAY);

		//Blurring image to remove noise
		
		int i = 7; //used in Size(i,i) function. Value was chosen after experimenting with different values	
		blur(imgGray, imgBlur, Size(i, i), Point(-1, -1));

	
		int thresholdValue = 145;	// This value was predetermined by playing with the trackbar to adjust threshold values. About 171 provivdes good filtering
		int binaryValue = 255;	// This value is used in the threshold() function, and is used for defining the value of the toggled pixels (i.e. not filtered)
		int binaryType = 0;		// Used in the threshold function. A Binary Type value of 0 means that the Threshold Type operation is type "Binary"

		const int MAX_THRESHOLD = 255;

		//Filtering image so that only really bright areas are shown (i.e. road lines)
		threshold(imgBlur, imgFiltered, thresholdValue, binaryValue, binaryType);


		int lowThresholdCanny = 50;	//After experimenting with different values, it didn't make too much of a difference, so a value of 50 was conveniently choses
		int highThresholdCanny = lowThresholdCanny * 3;	//In tutorials it's been recommended that the upper limit be generally 3 times the lower limit
		int kernelSize = 3;	//In tutorials it's been recommended that the kernel size value can generally be 3.0

		//Detecting edges
		Canny(imgFiltered, imgEdges, lowThresholdCanny, highThresholdCanny, kernelSize);

		vector<Vec4i> lines;	//this vector will store the data for straight lines in image

		int rho = 1, linesThreshold = 50, minLineLength = 50, maxLineGap = 10;	//These were the values used in a certain tutorial
		double theta = CV_PI / 180.0;

		//Detecting straight lines 
		HoughLinesP(imgEdges, lines, rho, theta, linesThreshold, minLineLength, maxLineGap);

		cvtColor(imgEdges, imgRoadLines, CV_GRAY2BGR);

		//Drawing the lines
		int lineThickness = 3;

		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(imgRoadLines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), lineThickness, CV_AA); //making lines
		}

		string windowName = "Detected Road Lines";
		imshow(windowName, imgRoadLines);

		waitKey(33);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code with Trackbar in order to determine best values for threshold filtering. I used this as my rough work.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*

Mat solidWhiteCurve, solidWhiteCurveGray, solidWhiteCurveFiltered;

Mat dst;

int thresholdValue = 0;
int binaryValue = 255;	
int binaryType = 0;		

const int MAX_THRESHOLD = 255;	

string windowNameImageFiltered = "Solid White Curve";

void threshold(int, void*)	//this function will filter out certain parts of the grayscale image so that only necessary parts (i.e. road lines) remain
{
	threshold(solidWhiteCurveGray, dst, thresholdValue, binaryValue, binaryType);
	imshow(windowNameImageFiltered, dst);
}

int main()
{

	
solidWhiteCurve = imread("solidWhiteCurve.jpg");

cvtColor(solidWhiteCurve, solidWhiteCurveGray, CV_RGB2GRAY);	

string trackbarName = "Threshold Trackbar", trackbarWindowName = "Trackbar";	// Trackbar will be used to adjust threshold value for filtering road lines out

namedWindow(trackbarWindowName, CV_WINDOW_AUTOSIZE);	//create window for trackbar to be in

createTrackbar(trackbarName, trackbarWindowName, &thresholdValue, MAX_THRESHOLD, threshold);

waitKey();

}

*/

