#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include "windows.h"

using namespace cv;
using namespace std;

double compare_imgs (Mat img1, Mat img2);
Mat myblur(Mat img);

int main() {
    TickMeter time;
    Mat myimg=imread("dog.jpg", IMREAD_GRAYSCALE); // convert image to the single channel grayscale image
    Mat filter = myimg.clone();
    Mat dif;
    time.start();
    blur(filter, filter, Size(3,3));
    time.stop();
    cout << "Blur working time "<< time.getTimeSec() << " sec"<< endl;
    time.reset();
    time.start();
    Mat filtered_img = myblur(myimg);
    time.stop();
    cout << "My filter working time "<< time.getTimeSec() << " sec" << endl;
    double percentage = compare_imgs(filter, filtered_img);
    cout <<"Persent of similarity " << round(percentage*100)/100 << "%" << endl; //with 2 decimal places
    imshow("Original image", myimg);
    imshow("My filter", filtered_img);
    imshow("Blur filter", filter);
    absdiff(filter, filtered_img, dif);
    imshow("Difference", dif);
    waitKey(0);
    return 0;
}

double compare_imgs (Mat img1, Mat img2){ //for images of equal sizes, returns number of similar pixels in %
    double dif = 0; //number of different pixels
    int pix1, pix2;
    int size = img1.rows*img1.cols;
    for (int row=0; row < img1.rows; row++) {
        for (int col = 0; col < img1.cols; col++ ) {
            pix1 = img1.at<uchar>(row, col);
            pix2 = img2.at<uchar>(row, col);
            if (pix1 != pix2)
                dif ++;
        }
    }
    return 100 - (dif * 100 / size);
}

Mat myblur(Mat img){
    int vertice_x, vertice_y = 0;
    Mat roi;
    double sum ;
    int average;
    Mat res_img = img.clone();
    for (vertice_x = 0; vertice_x < img.cols-2; vertice_x++) {
        for (vertice_y = 0; vertice_y < img.rows-2; vertice_y++) {
            sum = 0;
            // Select ROI
            Rect RectangleToSelect(vertice_x, vertice_y, 3, 3);
            roi = img(RectangleToSelect);
            for (int row=0; row<3; row++) { //sum the pixels inside the 3x3 box
                for (int col = 0; col<3; col++ ) {
                    sum += roi.at<uchar>(col,row);
                }
            }
            average = round(sum/9);
            //replace pixel by the average
            res_img.at<uchar>(vertice_y+1, vertice_x+1) = average;
        }
    }
    return res_img;
}
