#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/types.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "windows.h"

using namespace cv;
using namespace std;


Mat get_threshold_Bars(Mat img);
void create_trackBars(string win_name);