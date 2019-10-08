//
// Created by natas on 07.10.2019.
//

#ifndef LAB1_APPROXIMATION_H
#define LAB1_APPROXIMATION_H

#include "matrix.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include <vector>

#define pi 3.14

using namespace cv;
using namespace std;

vector <double> approximate(vector<double> rows_, vector<double> cols, Mat image, int roi_num , int degree);
Matrix find_matrix_A ( vector <double > roi_x, vector <double> roi_y, unsigned int n);
void angles(Matrix A, vector <double > xi, unsigned int n);








#endif //LAB1_APPROXIMATION_H
