#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


bool Histogram(Mat &srcImg, vector<long long> &hist) {
	int loc;
	for (int i = 0; i < srcImg.rows; i++) {
		for (int j = 0; j < srcImg.cols; j++) {
			loc = int(srcImg.at<uchar>(i,j));
			hist[loc]++;
		}
	}

	return true;
}



/*
	Probability of density function of a histogram
	Probability of occurances in terms of total pixel
*/

bool computePDF(vector<long long> &hist, vector<double> &pdf, double totalPix) {
	for (int i = 0; i < hist.size(); i++) {
		pdf[i] = double(hist[i]) / totalPix;
	}

	return true;
}



/*
Compute CDF and transformation

CDF example
A = [0.19,0.67,0.51,0.1]
bin = [0,1,2,3]

len = 4;

0.19 x (len - 1)
(0.19 + 0.67) x (len - 1)
(0.19 + 0.67 + 0.51) x (len - 1)
(0.19 + 0.67 + 0.51 + 0.1) x (len - 1)

The operation without multiplication of (len - 1) is CDF
With multiplicaiton is Transformation
*/


bool computeCDFTrasfromation(vector<double> &cdf, vector<double> &pdf) {
	double sum = 0.0;
	for (int i = 0; i < pdf.size(); i++) {
		sum = sum + pdf[i];
		cdf[i] = round(sum * (pdf.size() - 1));
	}


	return true;
}


/*
Mapping CDF transformed histogram into an transformed image
 Read each pixel of inImg
 Take that Pixel as a Bin and and read the Pixel value of CdfTransHist
 Replace that value in the OutImg location
*/


bool mappingPixels(Mat &srcImg, Mat &outImg, vector<double> &cdf) {
	unsigned int temp;
	for (int i = 0; i < srcImg.rows; i++) {
		for (int j = 0; j < srcImg.cols; j++) {
			temp = srcImg.at<uchar>(i, j);
			outImg.at<uchar>(i, j) = unsigned char(cdf[temp]);
		}
	}
	return true;
}



bool HistogramEqulization(Mat &srcImg, Mat &outImg) {

	vector<long long> hist(256, 0);	//256 lenth array with value 0
	Histogram(srcImg, hist);	//compute histogram

	vector<double> pdf(256,0.0);
	computePDF(hist, pdf, double(srcImg.rows * srcImg.cols));	//compute PDF

	vector<double> cdf(256, 0.0);
	computeCDFTrasfromation(cdf, pdf); //compute CDF transformation

	mappingPixels(srcImg, outImg, cdf); // mapping pixel




	hist.clear();
	pdf.clear();
	cdf.clear();

	return true;
}



int main(int argc, char** argv) {
	//Checking number of different argumnets
	cout << "You have entered " << argc << " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";

	//Throwing out the argumnet expectations
	if (argc == 1) {
		printf("Error: found in input arguments! \n");
		printf("Expected: 1st input argument the directory of images \n");
		printf("Expected: 2nd input argument the extension of the images that need to be processed \n");
		printf("Example c:\imageDatabase .jpg\n");
		return 0;
	}

	string imgName = argv[1];
	char key;

	Mat img = imread(imgName, IMREAD_GRAYSCALE);

	string windowName = "Input Image";
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, img);
	key = waitKey();

	////accesing each pixel
	//for (int i = 0; i < img.rows; i++) {
	//	for (int j = 0; j < img.cols; j++) {
	//		cout << int(img.at<uchar>(i, j)) << "	";
	//	}
	//	cout << endl;
	//}


	//Recording image data in csv
	ofstream ImagePixRecord;
	ImagePixRecord.open("PixelRecord.csv", ios::out);

	//Recording Data
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			ImagePixRecord << int(img.at<uchar>(i, j)) << ",";
		}
		ImagePixRecord << endl;
	}

	ImagePixRecord.close();


	//Histogram and record
	vector<long long> hist(256, 0);
	Histogram(img, hist);

	ofstream histRecord;
	histRecord.open("HistData.csv", ios::out);

	for (int i = 0; i < hist.size(); i++) {
		histRecord << i << "," << hist[i] << endl;
	}

	histRecord.close();

	Mat outImg(Size(img.rows, img.cols), CV_8UC1, Scalar(0));
	HistogramEqulization(img, outImg);

	string windowNameO = "Histogram equalized Image";
	namedWindow(windowNameO, WINDOW_AUTOSIZE);
	imshow(windowNameO, outImg);
	key = waitKey();



	system("pause");

	outImg.release();
	img.release();


	return 0;
}