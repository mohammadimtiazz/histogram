#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 

using namespace std;
typedef unsigned char u_int8_t;


//Measure histogram for binwidth 1
void imhist(u_int8_t **image, int row, int col, int histogram[], int length)
{
 
    // initialize all intensity values to 0
    for(int i = 0; i < length; i++)
    {
        histogram[i] = 0;
    }
	
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < row; y++){
        for(int x = 0; x < col; x++){
			//cout << (unsigned int)image[y][x] << endl;
            histogram[(unsigned int)image[y][x]]++;
		}

	}
 
}


//Measure chi square using follwoing equation (Quadric chi)
double m2_chiSquare(double ovservationHist[], double expectedHist[], int length){

	double result = 0, num = 0, dum = 0;
	
	for (int i = 0; i < length; i++){
		num = ovservationHist[i] - expectedHist[i];
		dum = ovservationHist[i] + expectedHist[i];

		//skip denominator with 0 in the calculation
		if (dum!=0){
			result += (num*num)/dum;
		}
		else{
		}
	}

	return result;

}

//Measure chi square using follwoing equation (Traitional)
double m1_chiSquare(double ovservationHist[], double expectedHist[], int length){

	double result = 0, index = 0;
	
	for (int i = 0; i < length; i++){
		//skip denominator with 0 in the calculation
		if (expectedHist[i] != 0){
			index = ovservationHist[i] - expectedHist[i];
			result += (index*index)/expectedHist[i];
		}
		else{
		}
	}

	return result;

}



//Normalization using Trapezoidal numerical integration
void normHistUsingTrapz(int in_hist[], double out_hist[], int len){

    // initialize all intensity values to 0
    for(int i = 0; i < len; i++)
    {
        out_hist[i] = 0;
    }


	double trapz = 0;
	for(int i=1; i < len - 1; i++){
		trapz += 2 * in_hist[i];
	}
	
	trapz = in_hist[0] + trapz + in_hist[len-1];
	trapz = trapz/2;

	for(int i = 0; i < len; i++){
		out_hist[i] = in_hist[i]/trapz;
	}
}


//Normalization using PDF equal to 1
void normHistUsingPdf(int in_hist[], double out_hist[], int len, double binWid){
    // initialize all intensity values to 0
    for(int i = 0; i < len; i++)
    {
        out_hist[i] = 0;
    }

	double sum = 0.0;
	double binWidth = 0.0;

	for(int i=0; i < len; i++){
		sum += in_hist[i];
	}

	for(int i = 0; i < len; i++){
		out_hist[i] = in_hist[i]/(sum * binWid);
	}
}


//Measure intersection
double intersection(double ovservationHist[], double expectedHist[],int len){
	double inter = 0.0;

	for(int i=0; i < len; i++){
		inter += min(ovservationHist[i], expectedHist[i]);
	}

	return inter;
}

//measure correlation
double correlation(double ovservationHist[], double expectedHist[],int len){
	double avg_ovservationHist = 0.0, avg_expectedHist = 0.0;
	double num = 0.0, dem1 = 0.0, dem2 = 0.0;
	double corr;


	for(int i = 0; i < len; i++){
		avg_ovservationHist += ovservationHist[i];
		avg_expectedHist += expectedHist[i];
	}


	avg_ovservationHist = avg_ovservationHist/len;
	avg_expectedHist = avg_expectedHist/len;

	
	for(int i = 0; i < len; i++){
		num += ((ovservationHist[i] - avg_ovservationHist) * (expectedHist[i] - avg_expectedHist));
		dem1 += pow((ovservationHist[i] - avg_ovservationHist), 2);
		dem2 += pow((expectedHist[i] - avg_expectedHist),2);
	}


	corr = num / sqrt(dem1 * dem2);

	return corr;

}

//Measure Bhattacharyya distance
double bhattacharyyaDis(double ovservationHist[], double expectedHist[],int len){
	double avg_ovservationHist = 0.0, avg_expectedHist = 0.0, num1 = 0.0, bDis;

	for(int i = 0; i < len; i++){
		avg_ovservationHist += ovservationHist[i];
		avg_expectedHist += expectedHist[i];
		num1 += sqrt(ovservationHist[i] * expectedHist[i]);
	}


	avg_ovservationHist = avg_ovservationHist/len;
	avg_expectedHist = avg_expectedHist/len;


	bDis = sqrt(1 - ((1 / sqrt(avg_ovservationHist * avg_expectedHist * len * len)) * num1));


	return bDis;
}



int main(){
	// Read Image A
	IplImage *imgA = cvLoadImage("imgA.bmp");

	if (imgA)
		cout << "read image A successfully!" << endl;
	else{
		cout << "Fail to read image A!" << endl;
		abort();
	}

	//Read Image B
	IplImage *imgB = cvLoadImage("imgB.bmp");

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


	unsigned char **aGray, **bGray;
	int rowA, colA, rowB, colB;
	rowA = imgA->height;
	colA = imgA->width;
	rowB = imgB->height;
	colB = imgB->width;

	//Create and fillup 2dim array with all IplImage values
	//instead of using IplImage as function's input aGray and bGray has been used 
	aGray = new unsigned char *[rowA];
	bGray = new unsigned char *[rowB];

	for(int i = 0; i < rowA; i++){
		aGray[i] = new unsigned char[colA];
	}

	for (int y = 0; y < rowA; y++){
		for (int x = 0; x < colA; x++){
			aGray[y][x] = grayA->imageData[grayA->widthStep * y + x * 1];
		}
	}


	for(int i = 0; i < rowB; i++){
		bGray[i] = new unsigned char[colB];
	}

	for (int y = 0; y < rowB; y++){
		for (int x = 0; x < colB; x++){
			bGray[y][x] = grayB->imageData[grayB->widthStep * y + x * 1];
		}
	}



	int histA[256], histB[256];
	int len = 256;

	//Measure Histogram
	//here bin width is 1 0,1,2...255
	imhist(aGray, rowA, colA, histA, len);
	imhist(bGray, rowA, colA, histB, len);


	////normalization using Trapezoidal numerical integration
	//double normHistA[256], normHistB[256];
	//normHistUsingTrapz(histA, normHistA, len);
	//normHistUsingTrapz(histB, normHistB, len);

	//Normalization using pdf
	double normHistA[256], normHistB[256];
	double binwidth = 1.0;
	normHistUsingPdf(histA, normHistA, len, binwidth);
	normHistUsingPdf(histB, normHistB, len, binwidth);

	double chiResult1, chiResult2;

	chiResult1 = m1_chiSquare(normHistA, normHistB, len);
	cout << "Chi-square1 result: " << chiResult1 << endl;
	chiResult2 = m2_chiSquare(normHistA, normHistB, len);
	cout << "Chi-square2 result: " << chiResult2 << endl;

	double m_intersection;
	m_intersection = intersection(normHistA, normHistB, len);
	cout << "intersection result: " << m_intersection << endl;
	
	double m_corr;
	m_corr = correlation(normHistA, normHistB, len);
	cout << "Correlation result: " << m_corr << endl;

	double mBtDis;
	mBtDis = bhattacharyyaDis(normHistA, normHistB, len);
	cout << "Bhattacharyya distance result: " << mBtDis << endl;

	getch();
	return 0;

}