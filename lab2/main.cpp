#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include "windows.h"

using namespace cv;
using namespace std;

int main() {
    Mat myimg=imread("dog.jpg", IMREAD_GRAYSCALE); // convert image to the single channel grayscale image
    int vertice_x, vertice_y = 0;
    Mat roi;
    double sum ;
    //Mat filter = myimg.clone();
    //blur(filter, filter, Size(3,3));
    imwrite("Saves/non_filtered_dog.jpg", myimg);
    for (vertice_x = 0; vertice_x < myimg.cols-3; vertice_x+=3) {
        for (vertice_y = 0; vertice_y < myimg.rows-3; vertice_y+=3) {
            sum = 0;
            // Select ROI
            Rect RectangleToSelect(vertice_x, vertice_y, 3, 3);
            roi = myimg(RectangleToSelect);
            //imshow("Filter", roi);

            //waitKey(0);
            //data = roi.clone(); //roi is non-continuous Mat, copy it to reshape and count average
            /*int roi_size = data.rows * data.cols;
            data = data.reshape(1, roi_size); //reshape 1 channel matrix of size rows x cols to be a matrix of shape ((rows*cols) x 1)
            reduce(data, data, 1, REDUCE_AVG); // compute the average row-wise
            data = data.reshape(1, roi_size); //reshape to 1 channel matrix*/
            for (int row=0; row<3; row++) {
                for (int col = 0; col<3; col++ ) {
                    sum += roi.at<uchar>(row,col);
                }
            }
            for (int row=0; row<3; row++) {
                for (int col = 0; col<3; col++ ) {
                    myimg.at<uchar>(vertice_y+col, vertice_x+row) = (int)sum/9;
                }
            }
        }
        //if (vertice_x == 960)
          //  cout<<"vertice_x =  "<< vertice_x<< "vertice_y = " << vertice_y << endl;
    }
    //cout<<"End loop 2"<<endl;
    imwrite("Saves/filtered_dog.jpg", myimg);
    imshow("Filter", myimg);


    waitKey(0);




    return 0;
}