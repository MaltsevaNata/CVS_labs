
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

static int button; //������� ������
static int img_num = 0; //������� ����� ���������� �� �����

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
	int lowH = createTrackbar("Hue_low", win_name, &h_low, 179, 0); // ������� min
	setTrackbarPos("Hue_low", win_name, 0);
	int highH = createTrackbar("Hue_high", win_name, &h_high, 179, 0); // ������� max
	setTrackbarPos("Hue_high", win_name, 179);
	int lowS = createTrackbar("Sat_low", win_name, &s_low, 255, 0); // ������������ min
	setTrackbarPos("Sat_low", win_name, 0);
	int highS = createTrackbar("Sat_high", win_name, &s_high, 255, 0); // ������������ max
	setTrackbarPos("Sat_high", win_name, 255);
	int lowV = createTrackbar("Value_low", win_name, &v_low, 255, 0); // ������� min
	setTrackbarPos("Value_low", win_name, 227);
	int highV = createTrackbar("Value_high", win_name, &v_high, 255, 0); // ������� max
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
	Mat element = Mat(); //����� ���� ������ 3�3 �� ���������
	dilate(myimg_hsv, myimg_hsv, element); //������� ������ ���, ������ ��������

	create_trackBars(win_name); //��������� � ������� ��������
	Mat threshold;
	vector<vector<Point>> contours;
	Scalar green = Scalar(0, 255, 0);
	Scalar red = Scalar(0, 0, 255); //bgr

	while (1) {
		threshold = get_threshold_Bars(myimg_hsv);
		//inRange(myimg_hsv, Scalar(0, 0, 227), Scalar(179, 255, 255), threshold); //��������� � ������� �������� ��������
		findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //����� ������� �������� ��� �������������
		cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
		polylines(threshold, contours, true, green, 2, 10); //��������� ��������
		double max_content = 0; //���������� ������� �������
		Point max_center; //����� ����������� �������
		for (int i = 0; i < contours.size(); i++) { //������� �����  ���� ������� �������
			Moments mom = moments(contours[i]);
			double x = mom.m10 / mom.m00;
			double y = mom.m01 / mom.m00;
			Point center = Point(x, y);
			circle(threshold, center, 0.8, red, 2, 8, 0); // ����� ������ �������
			if (contourArea(contours[i], false) > max_content) { //������� ������ ���������� �������
				max_content = contourArea(contours[i], false);
				max_center = center;
			}
		}
		line(threshold, Point(max_center.x - 10, max_center.y), Point(max_center.x + 10, max_center.y), red, 1, 8); //������ ���� ��� ����������� �������
		line(threshold, Point(max_center.x, max_center.y - 10), Point(max_center.x, max_center.y + 10), red, 1, 8);
		imshow(win_name, myimg_hsv);
		imshow("Result", threshold);
		button = waitKey(33);
		//std::cout << button << std::endl;
		if (button == 100) { //D - ��������� �����������
			img_num++;
			destroyAllWindows();
			break;
		}
		else if (button == 97) { //A - ���������� �����������
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