#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;


//struct has been used here for fetching multiple outputs from the function
struct histStatistics{
	int mode;
	double mean;
};


//Calculate hist statistics For now just mean and mode have been implemented
struct histStatistics histParameters(int *histArray, const int totalBins){

	struct histStatistics resultIndex;
	double sumNum = 0.0;
	double sumDum = 0.0;
	int temp = 0;


	for (int i = 0; i < totalBins; i++){
		//calculating numerator and denominator of mean
		sumNum = sumNum + i * histArray[i]; // sum of product or weight
		sumDum = sumDum + histArray[i];		// sum of bins

		// looking for the bin with maximum occurances
		if (histArray[i] > temp){
			temp = histArray[i];
			resultIndex.mode = i;
		}
		else{
		}
	}
	resultIndex.mean = sumNum / sumDum;

	return resultIndex;

}

// creating histogram with fixed bin numbers 256
void createHist(int rows, int cols, unsigned char **pGray, int *histArray){

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			histArray[pGray[y][x]]++;
			
		}
	}

	////print Histogram values
	//for (int i = 0; i < totalBins; i++){
	//	cout << histArray[i] << endl;
	//}
}





int main(){
	IplImage *img = cvLoadImage("cameraman.png");

	//check images has been read properly or not
	if(img){
		cout << "successfully read image"<< endl;
	}
	else{
		cout << "Failed to read image!" << endl;	
		abort();			// abort if there is no image
	}

	int rows, cols;

	cols = img->width;
	rows = img->height;


	// creating a 2 dim array pGray for holding all pixel values
	unsigned char **pGray;
	pGray = new unsigned char *[rows];

	for(int i = 0; i < rows; i++){
		pGray[i] = new unsigned char[cols];
	}	


	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			pGray[y][x] = img->imageData[img->widthStep * y + x * 1];
			//cout << (int) pGray[y][x] << " ";		// Print Image data (comment this line if not necessary)
		}
		//cout << endl;
	}



	const int totalBins = 256;
	int histArray[totalBins] = {0};
	createHist(rows, cols, pGray, histArray);		//calculate histogram

	struct histStatistics result = histParameters(histArray, totalBins);		//calculate mean and mode

	cout << "ModeX: " << result.mode << endl;
	cout << "MeanX: " << result.mean << endl;


	getch();
	return 0;

}