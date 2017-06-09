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


    //print occurance values
	cout << "For example occurance values before normalization: " << endl;
	float value;
    for (int i = 0; i < a_bins; i++) {
        value = cvQueryHistValue_1D(histA, i);
        printf("%f\n", value);
    }

	//normalize histogram 
	cvNormalizeHist(histA, 1.0);
	cvNormalizeHist(histB, 1.0);


    //print occurance values
	cout << "For example occurance values after normalization: " << endl;
    for (int i = 0; i < a_bins; i++) {
        value = cvQueryHistValue_1D(histA, i);
        printf("%f\n", value);
    }


	double compHist_corr, compHist_chiSqr, compHist_intersection, compHist_bcDis;


	compHist_corr = cvCompareHist(histA, histB, CV_COMP_CORREL);
	compHist_chiSqr = cvCompareHist(histA, histB, CV_COMP_CHISQR);
	compHist_intersection = cvCompareHist(histA, histB, CV_COMP_INTERSECT);
	compHist_bcDis = cvCompareHist(histA, histB, CV_COMP_BHATTACHARYYA );

	cout << "Correlation score: " << compHist_corr << endl;
	cout << "Chi square score: " << compHist_chiSqr << endl;
	cout << "Intersection score: " << compHist_intersection << endl;
	cout << "Bhattacharyya distance score: " << compHist_bcDis << endl;


	cvReleaseImage(&grayA);
	cvReleaseImage(&grayB);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
	cvReleaseHist(&histA);
	cvReleaseHist(&histB);

	cout << "Hello world!" << endl;

	getch();
	return 0;
}