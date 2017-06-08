#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 

using namespace std;


int main(){
	IplImage *imgA = cvLoadImage("imgC.bmp");

	if (imgA)
		cout << "read image A successfully!" << endl;
	else{
		cout << "Fail to read image A!" << endl;
		abort();
	}


	IplImage *imgB = cvLoadImage("imgD.bmp");

	if (imgB)
		cout << "read image A successfully!" << endl;
	else{
		cout << "Fail to read image A!" << endl;
		abort();
	}


	// conversion of color to gray
	IplImage *grayA = cvCreateImage(cvGetSize(imgA), 8, 1);
	cvCvtColor(imgA, grayA, CV_BGR2GRAY);

	IplImage *grayB = cvCreateImage(cvGetSize(imgB), 8, 1);
	cvCvtColor(imgB, grayB, CV_BGR2GRAY);


    //planes to obtain the histogram, in this case just one
    IplImage * planesA[] = {grayA};
	IplImage * planesB[] = {grayB};


	CvHistogram *hist;
	int a_bins = 256;
	int b_bins = 256;

	// histSize array with a_bins, b_bins
	int histSize[] = {a_bins, b_bins};

	//array with 0 and 256
	float a_ranges[] = {0,256};
	float b_ranges[] = {0,256};

	//List with array inside
	float *ranges[] = {a_ranges, b_ranges};

    //get the histogram and some info about it
	CvHistogram* histA;
	CvHistogram* histB;

    histA = cvCreateHist(1, histSize, CV_HIST_ARRAY, ranges, 1);
    cvCalcHist(planesA, histA, 0, NULL);

    histB = cvCreateHist(1, histSize, CV_HIST_ARRAY, ranges, 1);
    cvCalcHist(planesB, histB, 0, NULL);

	float maxValue = 0, minValue = 0;
	cvGetMinMaxHistValue(histA, &minValue, &maxValue);
	printf("min: %f, max: %f\n", minValue, maxValue);

	maxValue = 0, minValue = 0;
	cvGetMinMaxHistValue(histB, &maxValue, &minValue);
	printf("min: %f, max: %f\n", minValue, maxValue);


    //create an 8 bits single channel image to hold the histogram
    //paint it white
	IplImage* imgHistogramA = cvCreateImage(cvSize(a_bins, 200), 8, 1);
    cvRectangle(imgHistogramA, cvPoint(0, 0), cvPoint(256, 200), CV_RGB(255, 255, 255), -1);
    //draw the histogram 
    //value and normalized value
    float value;
    int normalized;
    for (int i = 0; i < a_bins; i++) {
        value = cvQueryHistValue_1D(histA, i);
        normalized = cvRound(value * 200 / minValue);
        cvLine(imgHistogramA, cvPoint(i, 200), cvPoint(i, 200 - normalized), CV_RGB(0, 0, 0));
        printf("%d\n", normalized);
    }


    //Create 3 windows to show the results
    cvNamedWindow("original A", 1);
    cvNamedWindow("gray A", 1);
    cvNamedWindow("histogram A", 1);

    //show the image results
    cvShowImage("original A", imgA);
    cvShowImage("gray A", grayA);
    cvShowImage("histogram A", imgHistogramA);

    //create an 8 bits single channel image to hold the histogram
    //paint it white
	IplImage* imgHistogramB = cvCreateImage(cvSize(b_bins, 200), 8, 1);
    cvRectangle(imgHistogramB, cvPoint(0, 0), cvPoint(256, 200), CV_RGB(255, 255, 255), -1);
    //draw the histogram 
    //value and normalized value
    for (int i = 0; i < b_bins; i++) {
        value = cvQueryHistValue_1D(histB, i);
        normalized = cvRound(value * 200 / minValue);
        cvLine(imgHistogramB, cvPoint(i, 200), cvPoint(i, 200 - normalized), CV_RGB(0, 0, 0));
        printf("%d\n", normalized);
    }


    //Create 3 windows to show the results
    cvNamedWindow("original B", 1);
    cvNamedWindow("gray B", 1);
    cvNamedWindow("histogram B", 1);

    //show the image results
    cvShowImage("original B", imgB);
    cvShowImage("gray B", grayB);
    cvShowImage("histogram B", imgHistogramB);

    cvWaitKey();

	cvReleaseImage(&grayA);
	cvReleaseImage(&grayB);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
	cvReleaseHist(&histA);
	cvReleaseHist(&histB);

	getch();
	return 0;
}
