// ObjectDetector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <time.h>
#include <sstream>


using namespace cv;
using namespace std;


Mat img, roiImg; /* roiImg - the part of the image in the bounding box */
int select_flag = 0;
time_t start, endtime;

Mat mytemplate;

void track(cv::Mat &img, const cv::Mat &templ)
{
	static int n = 0;

	if (select_flag)
	{
		templ.copyTo(mytemplate);
		select_flag = false;

	}


	cv::Mat result;
	/// Do the Matching and Normalize
	matchTemplate(img, mytemplate, result, TM_SQDIFF_NORMED);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	rectangle(img, matchLoc, Point(matchLoc.x + mytemplate.cols, matchLoc.y + mytemplate.rows), CV_RGB(255, 255, 255), 3);

	std::cout << matchLoc << "\n";
}

///MouseCallback function

void objectDetector()
{
	roiImg = imread("templateImage.png");
	select_flag = 1;

	track(img, roiImg);
}
///Main function
int main()
{
	/*
		VideoCapture cap(0);
		if (!cap.isOpened())
		return 1;
	*/
	VideoCapture cap;

	cap.open("./Video_1.mp4");
	if (!cap.isOpened())
	{
		printf("Unable to open video file\n");
		return -1;
	}
	// Set video to 320x240
	cap.set(CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CAP_PROP_FRAME_HEIGHT, 240);

	cap >> img;
	imshow("image", img);
	//time(&start);
	while (1)
	{
		cap >> img;
		if (img.empty())
			break;

		objectDetector();

		if (select_flag == 1)
			imshow("Template", roiImg);

		imshow("image", img);
		if (waitKey(30) >= 0)
			break;
		//time(&endtime);

		//double dif = difftime(endtime, start);
		//printf("Elasped time is %.2lf seconds.", dif);
		//if (dif == 10)
		//{
		//	std::cout << "DONE" << dif << std::endl;
		//	break;
		//}
	}


	return 0;
}


