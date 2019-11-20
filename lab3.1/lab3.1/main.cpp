
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "math.h"
#include <opencv2/core/types.hpp>
#include "windows.h"
#include <conio.h>

using namespace cv;
using namespace std;

static int h_low, h_high;
static int s_low, s_high;
static int v_low, v_high;

static int button; //нажата€ кнопка
static int img_num = 0; //текущий номер изображен€ из папки

Mat get_threshold_Bars(Mat img);
void create_trackBars(string win_name);
void process_image(String imageName);

int main()
{
	vector<string> images; 
	string path = "D:/Study/STV labs/CVS_labs/lab3.1/img_zadan/allababah";
	glob(path, images, false);
	while ((img_num < images.size()) && (img_num >= 0)) {
		String imageName = images[img_num];
		process_image(imageName);
	}
	
	return 0;
}

void create_trackBars(string win_name) {
	int lowH = createTrackbar("Hue_low", win_name, &h_low, 179, 0); // оттенок min
	setTrackbarPos("Hue_low", win_name, 0);
	int highH = createTrackbar("Hue_high", win_name, &h_high, 179, 0); // оттенок max
	setTrackbarPos("Hue_high", win_name, 179);
	int lowS = createTrackbar("Sat_low", win_name, &s_low, 255, 0); // насыщенность min
	setTrackbarPos("Sat_low", win_name, 0);
	int highS = createTrackbar("Sat_high", win_name, &s_high, 255, 0); // насыщенность max
	setTrackbarPos("Sat_high", win_name, 255);
	int lowV = createTrackbar("Value_low", win_name, &v_low, 255, 0); // €ркость min
	setTrackbarPos("Value_low", win_name, 227);
	int highV = createTrackbar("Value_high", win_name, &v_high, 255, 0); // €ркость max
	setTrackbarPos("Value_high", win_name, 255);
}

Mat get_threshold_Bars(Mat img) {
	Scalar lower_hsv, higher_hsv;
	Mat threshold;
	lower_hsv = Scalar(h_low, s_low, v_low);
	higher_hsv = Scalar(h_high, s_high, v_high);
	inRange(img, lower_hsv, higher_hsv, threshold);
	return threshold;

}

void process_image(String imageName) {
	Mat myimg = imread(imageName);
	if (myimg.empty()) {
		std::cout << "failed to open" << imageName << std::endl;
		return;
	}
	else
		std::cout << imageName << " loaded OK" << std::endl;
	Mat myimg_hsv;
	cvtColor(myimg, myimg_hsv, COLOR_BGR2HSV);
	const string win_name = imageName;
	namedWindow(win_name, WINDOW_AUTOSIZE);
	namedWindow("Result", WINDOW_AUTOSIZE);
	Mat element = Mat(); //маска €дра эрозии 3х3 по умолчанию
	dilate(myimg_hsv, myimg_hsv, element); //удал€ем лишний шум, мелкие элементы

	create_trackBars(win_name); //настройка с помощью бегунков
	Mat threshold;
	vector<vector<Point>> contours;
	Scalar green = Scalar(0, 255, 0);
	Scalar red = Scalar(0, 0, 255); //bgr

	while (1) {
		threshold = get_threshold_Bars(myimg_hsv);
		//inRange(myimg_hsv, Scalar(0, 0, 227), Scalar(179, 255, 255), threshold); //выбранные с помощью бегунков значени€
		findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //поиск внешних контуров без аппроксимации
		cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
		polylines(threshold, contours, true, green, 2, 10); //рисование контуров
		double max_content = 0; //наибольша€ площади контура
		Point max_center; //центр наибольшего контура
		for (int i = 0; i < contours.size(); i++) { //находим центр  масс каждого контура
			Moments mom = moments(contours[i]);
			double x = mom.m10 / mom.m00;
			double y = mom.m01 / mom.m00;
			Point center = Point(x, y);
			circle(threshold, center, 0.8, red, 2, 8, 0); // точка центра контура
			if (contourArea(contours[i], false) > max_content) { //находим контур наибольшей площади
				max_content = contourArea(contours[i], false);
				max_center = center;
			}
		}
		line(threshold, Point(max_center.x - 10, max_center.y), Point(max_center.x + 10, max_center.y), red, 1, 8); //рисуем цель дл€ наибольшего контура
		line(threshold, Point(max_center.x, max_center.y - 10), Point(max_center.x, max_center.y + 10), red, 1, 8);
		imshow(win_name, myimg_hsv);
		imshow("Result", threshold);
		button = waitKey(33);
		//std::cout << button << std::endl;
		if (button == 100) { //D - следующее изображение
			img_num++;
			destroyAllWindows();
			break;
		}
		else if (button == 97) { //A - предыдущее изображение
			img_num--;
			destroyAllWindows();
			break;
		}
		else {
			continue;
		}	
	}
	return;
}