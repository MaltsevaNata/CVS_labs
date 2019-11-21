#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/types.hpp>
#include "windows.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat process_image_lab12(String imageName, Mat myimg);
Mat process_image_lab3(String imageName, Mat myimg);
Mat find_draw_contours(Mat threshold, Mat myimg_hsv, Scalar contour_colour);
Mat draw_firepoint(Mat threshold, vector<vector<Point>> contours);
Mat update_image(Mat myimg_hsv);
Mat find_draw_contours_robots(Mat threshold, Mat myimg_hsv, Scalar contour_colour);
Mat update_image_robots(Mat myimg_hsv);
Mat draw_caps(Mat threshold, vector<vector<Point>> contours, Scalar colour);