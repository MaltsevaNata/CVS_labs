#include "process_image.h"
#include "trackbars.h"

static Scalar red = Scalar(0, 0, 255);
vector<vector<Point>> contours;
Scalar green = Scalar(0, 255, 0);
Scalar blue = Scalar(255, 0, 0);
Scalar yellow = Scalar(0, 255, 255);
double max_content; //наибольшая площади контура
Point max_center; //центр наибольшего контура

Mat process_image_lab12(String imageName, Mat myimg) {
	Mat myimg_hsv;
	cvtColor(myimg, myimg_hsv, COLOR_BGR2HSV);
	Mat element = Mat(); //маска ядра эрозии 3х3 по умолчанию
	dilate(myimg_hsv, myimg_hsv, element); //удаляем лишний шум, мелкие элементы
	//create_trackBars(imageName); //настройка с помощью бегунков
	return myimg_hsv;
}


Mat process_image_lab3(String imageName, Mat myimg) {
	Mat myimg_hsv;
	cvtColor(myimg, myimg_hsv, COLOR_BGR2HSV);
	Mat element;
	element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	erode(myimg_hsv, myimg_hsv, element, Point(-1, -1), 1);
	dilate(myimg_hsv, myimg_hsv, element); //удаляем лишний шум, мелкие элементы
	//create_trackBars(imageName); //настройка с помощью бегунков
	return myimg_hsv;
}

Mat update_image(Mat myimg_hsv) {
	Mat threshold;
	threshold = get_threshold_Bars(myimg_hsv);
	//inRange(myimg_hsv, Scalar(0, 0, 227), Scalar(179, 255, 255), threshold); //выбранные с помощью бегунков значения для 1 лабы
	threshold = find_draw_contours(threshold, myimg_hsv, blue);
	return threshold;
}


Mat update_image_robots(Mat myimg_hsv) {
	Mat threshold;
	Mat green_threshold;
	Mat blue_threshold;
	Mat lamp_threshold;
	//threshold = get_threshold_Bars(myimg_hsv);
	Scalar lower_red, higher_red, lower_green, higher_green, lower_blue, higher_blue, lower_lamp, higher_lamp;
	lower_red = Scalar(0, 0, 0);
	higher_red = Scalar(7, 255, 255);
	lower_green = Scalar(23,42,128);
	higher_green = Scalar(84, 255, 255);
	lower_blue = Scalar(84, 0, 0);
	higher_blue = Scalar(152, 255, 255);
	lower_lamp = Scalar(0, 0, 250);
	higher_lamp = Scalar(179, 14, 255);
	//threshold = find_draw_contours(threshold, myimg_hsv, blue);
	inRange(myimg_hsv, lower_red, higher_red, threshold);
	threshold = find_draw_contours_robots(threshold, myimg_hsv, red);
	cvtColor(threshold, threshold, COLOR_BGR2HSV);
	inRange(threshold, lower_green, higher_green, green_threshold);
	threshold = find_draw_contours_robots(green_threshold, threshold, green);
	cvtColor(threshold, threshold, COLOR_BGR2HSV);
	inRange(threshold, lower_blue, higher_blue, blue_threshold);
	threshold = find_draw_contours_robots(blue_threshold, threshold, blue);
	cvtColor(threshold, threshold, COLOR_BGR2HSV);
	inRange(threshold, lower_lamp, higher_lamp, lamp_threshold);
	threshold = draw_lamp(lamp_threshold, threshold);
	return threshold;
}


Mat draw_lamp(Mat threshold, Mat myimg_hsv) {
	findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //поиск внешних контуров без аппроксимации
	cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
	for (int i = 0; i < contours.size(); i++) { //находим центр  масс каждого контура
		Moments mom = moments(contours[3]);
		double x = mom.m10 / mom.m00;
		double y = mom.m01 / mom.m00;
		Point center = Point(x, y);
		circle(threshold, center, 20, yellow, -1, 8, 0); // точка центра контура
	}
	return threshold;
}


Mat find_draw_contours(Mat threshold, Mat myimg_hsv, Scalar contour_colour) {
	findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //поиск внешних контуров без аппроксимации
	cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
	polylines(threshold, contours, true, contour_colour, 2, 10); //рисование контуров
	threshold = draw_firepoint(threshold, contours);
	return threshold;
}

Mat find_draw_contours_robots(Mat threshold, Mat myimg_hsv, Scalar contour_colour) {
	findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //поиск внешних контуров без аппроксимации
	cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
	threshold = draw_caps(threshold, contours, contour_colour);
	//polylines(threshold, contours, true, contour_colour, 2, 10); //рисование контуров
	//threshold = draw_firepoint(threshold, contours);
	return threshold;
}


Mat draw_caps(Mat threshold, vector<vector<Point>> contours, Scalar colour) {
	int min_content = 110;
	double content;
	for (int i = 0; i < contours.size(); i++) { //находим площадь каждого контура
		content = contourArea(contours[i], false);
		if (content >= min_content) {
			polylines(threshold, contours[i], true, colour, 2, 10); //рисование контуров
		}
	}
	return threshold;
}

Mat draw_firepoint(Mat threshold, vector<vector<Point>> contours) {
	max_content = 0; //наибольшая площади контура
	max_center; //центр наибольшего контура
	for (int i = 0; i < contours.size(); i++) { //находим центр  масс каждого контура
		Moments mom = moments(contours[i]);
		double x = mom.m10 / mom.m00;
		double y = mom.m01 / mom.m00;
		Point center = Point(x, y);
		circle(threshold, center, 1, red, 2, 8, 0); // точка центра контура
		if (contourArea(contours[i], false) > max_content) { //находим контур наибольшей площади
			max_content = contourArea(contours[i], false);
			max_center = center;
		}
	}
	line(threshold, Point(max_center.x - 10, max_center.y), Point(max_center.x + 10, max_center.y), red, 1, 8); //рисуем цель для наибольшего контура
	line(threshold, Point(max_center.x, max_center.y - 10), Point(max_center.x, max_center.y + 10), red, 1, 8);
	return threshold;
}
