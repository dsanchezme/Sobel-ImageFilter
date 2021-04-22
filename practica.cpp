
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

using namespace cv;
using namespace std;





int main(int argc, char** argv )
{
	struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);

    Mat img, img_gray, img_invert, img_smoothing;
    int sigmaX, sigmaY, rows, cols;

    img = imread( argv[1], IMREAD_COLOR);

    rows = img.rows;
    cols = img.cols;

	double dx = 0, dy = 0;
	double v = 0;
	cv::Mat img_final = cv::Mat::zeros(rows, cols, CV_8UC1);
	cv::Mat dst = cv::Mat::zeros(rows, cols, CV_8UC1);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dst.at<uchar>(i, j) = 0.2126 * (float)img.at<cv::Vec3b>(i, j)[2]
				+ 0.7152 * (float)img.at<cv::Vec3b>(i, j)[1]
				+ 0.0722 * (float)img.at<cv::Vec3b>(i, j)[0];
		}
	}

    for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			v = 0;
			if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
				img_final.at<uchar>(i, j) = 0;
			}
			else {
				

				dx = dst.at<uchar>(i - 1, j + 1) - dst.at<uchar>(i - 1, j - 1)
					+ 2 * dst.at<uchar>(i, j + 1) - 2 * dst.at<uchar>(i, j - 1) +
					dst.at<uchar>(i + 1, j + 1) - dst.at<uchar>(i + 1, j - 1); 
				dy = dst.at<uchar>(i + 1, j - 1) - dst.at<uchar>(i - 1, j - 1)
					+ 2 * dst.at<uchar>(i + 1, j) - 2 * dst.at<uchar>(i - 1, j) +
					dst.at<uchar>(i + 1, j + 1) - dst.at<uchar>(i - 1, j + 1);
				v = abs(dx) + abs(dy);
				v = fmax(v, 0);
				v = fmin(v, 255);
				img_final.at<uchar>(i, j) = (uchar)v;
			}

		}
    }

	gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	
    // namedWindow( "Final", WINDOW_AUTOSIZE );
    // imshow( "Final", img_final);
    imwrite(argv[2], img_final);

    waitKey(0);

    return 0;
}
