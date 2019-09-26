#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include "windows.h"

#define pi 3.14

using namespace cv;

int main() {
    Mat myimg=imread("mars.jpg");
    Mat myimg_copy = myimg.clone();
    char* window_name = "mars";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    double height, width = 0;
    Point2f vertices2f[4];
    Point vertices[4];
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(255,0,0);
    Point2f prev_center = {0,0};
    for (int i=0; i<180; i++) {
        height =  myimg_copy.rows * (1-sin(i*pi/180)) + myimg_copy.rows/8;
        width =  myimg_copy.cols * i /180;
        RotatedRect rRect = RotatedRect(Point2f(width, height) /* center*/, Size2f(50,100) /*rect size*/, i/1.1 /*angle*/);
        rRect.points(vertices2f);
        for (int i = 0; i < 4; i++) {
            line(myimg_copy, vertices2f[i], vertices2f[(i + 1) % 4], green, 2); //draw rRect
            vertices[i] = vertices2f[i];
        }
        fillConvexPoly(myimg_copy, vertices, 4, green);
        prev_center = rRect.center;
        line(myimg, prev_center, rRect.center, blue, 2 ); //trajectory
        imshow(window_name, myimg_copy);
        myimg_copy = myimg.clone();
        waitKey(30);
    }
    return 0;
}