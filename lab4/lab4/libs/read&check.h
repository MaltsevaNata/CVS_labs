#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/types.hpp>
#include "windows.h"

using namespace cv;
using namespace std;

Mat read_img(String imageName);
int check_pressed_but(int img_num);