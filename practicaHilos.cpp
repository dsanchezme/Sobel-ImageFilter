
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define THREADS_DFLT 2

using namespace cv;
using namespace std;

static int THREADS = 0;

cv::Mat img, img_gray, img_invert, img_smoothing, img_final, dst;
int sigmaX, sigmaY, rows, cols;
double dx = 0, dy = 0;
double v = 0;

void *ConvertirGris(void *arg) {
	int initIterationRow, endIterationRow, initIterationCol, endIterationCol, threadId = *(int *)arg;


	initIterationRow = (floor(rows/THREADS)) * threadId;
	endIterationRow = ((ceil(rows/THREADS)) * (threadId+1));
	initIterationCol = (floor(cols/THREADS)) * threadId;
	endIterationCol = ((ceil(cols/THREADS)) * (threadId+1));


	for (int i = initIterationRow; i < endIterationRow; i++) {
		for (int j = 0; j < cols; j++) {
			dst.at<uchar>(i, j) = 0.2126 * (float)img.at<cv::Vec3b>(i, j)[2]
				+ 0.7152 * (float)img.at<cv::Vec3b>(i, j)[1]
				+ 0.0722 * (float)img.at<cv::Vec3b>(i, j)[0];
		}
	}

	return 0;
}

void *Sobelfilter(void *arg) {
	int initIterationRow, endIterationRow, initIterationCol, endIterationCol, threadId = *(int *)arg;


	initIterationRow = (floor(rows/THREADS)) * threadId;
	endIterationRow = ((ceil(rows/THREADS)) * (threadId+1));
	initIterationCol = (floor(cols/THREADS)) * threadId;
	endIterationCol = ((ceil(cols/THREADS)) * (threadId+1));


	for (int i = initIterationRow; i < endIterationRow; i++) {
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
	return 0;
}

int main(int argc, char** argv )
{

	if(atoi(argv[3]) < 1){
		THREADS = THREADS_DFLT;
		cout<<"Number of threads must be greater than 1"<<endl;
	}else{
		THREADS = atoi(argv[3]);
	}

	int threadId[THREADS], i, *retval;
    pthread_t thread[THREADS];

	struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);


    //img = imread("imagen1.jpg", IMREAD_COLOR);
    img = imread(argv[1], IMREAD_COLOR);
//    img = imread("house4k.jpg", IMREAD_COLOR);
//    img = imread("house1080.jpg", IMREAD_COLOR);


    rows = img.rows;
    cols = img.cols;

	img_final = cv::Mat::zeros(rows, cols, CV_8UC1);
	dst = cv::Mat::zeros(rows, cols, CV_8UC1);

//	ConvertirGris();
//	Sobelfilter();
	for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        // pthread_create(&thread[i], NULL, (void *)ConvertirGris, &threadId[i]);
		pthread_create(&thread[i], NULL, ConvertirGris, &threadId[i]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

	for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        // pthread_create(&thread[i], NULL, (void *)Sobelfilter, &threadId[i]);
		pthread_create(&thread[i], NULL, Sobelfilter, &threadId[i]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

	gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    // printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("%ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    // namedWindow( "Final", WINDOW_AUTOSIZE );
    // imshow( "Final", img_final);
    imwrite(argv[2], img_final);

    waitKey(0);



    return 0;
}
