//
// Created by natas on 08.10.2019.
//

#ifndef LAB1_RECTANGLE_H
#define LAB1_RECTANGLE_H

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include <vector>

#define pi 3.14

using namespace cv;
using namespace std;


void draw_moving_rect(Mat image, vector<double> x, vector<double> y, vector <double> angles);
vector <vector<double>> find_coord(Mat image, string func);


#endif //LAB1_RECTANGLE_H
