#include "process_image.h"
#include "trackbars.h"

static Scalar red = Scalar(0, 0, 255);
static vector<vector<Point>> contours;
static Scalar green = Scalar(0, 255, 0);
static Scalar blue = Scalar(255, 0, 0);
static Scalar yellow = Scalar(0, 255, 255);
static Scalar purple = Scalar(137, 250, 150);
static double max_content; //наибольшая площади контура
static Point max_center; //центр наибольшего контура
static Point lamp;
static vector<Point> red_team;
static vector<Point> green_team;
static vector<Point> blue_team;

Mat process_image_lab12(String imageName, Mat myimg) {
	Mat myimg_hsv;
	cvtColor(myimg, myimg_hsv, COLOR_BGR2HSV);
	Mat element = Mat(); //маска ядра эрозии 3х3 по умолчанию
	dilate(myimg_hsv, myimg_hsv, element); //удаляем лишний шум, мелкие элементы
	create_trackBars(imageName); //настройка с помощью бегунков
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
	Mat red_threshold;
	Mat green_threshold;
	Mat blue_threshold;
	Mat lamp_threshold;
	//threshold = get_threshold_Bars(myimg_hsv);
	Scalar lower_red, higher_red, lower_green, higher_green, lower_blue, higher_blue, lower_lamp, higher_lamp;
	lower_red = Scalar(0, 0, 0);
	higher_red = Scalar(7, 255, 255);
	lower_green = Scalar(35,42,128);
	higher_green = Scalar(84, 255, 255);
	lower_blue = Scalar(84, 0, 0);
	higher_blue = Scalar(152, 255, 255);
	lower_lamp = Scalar(0, 0, 230);
	higher_lamp = Scalar(255, 13, 255);
	//threshold = find_draw_contours(threshold, myimg_hsv, blue);

	inRange(myimg_hsv, lower_lamp, higher_lamp, threshold);
	threshold = draw_lamp(myimg_hsv, threshold); //НА ЭТОЙ ФУНКЦИИ ВСЕ ЛОМАЕТСЯ

	inRange(threshold, lower_red, higher_red, red_threshold);
	threshold = find_draw_contours_robots(red_threshold, threshold, red);

	inRange(threshold, lower_green, higher_green, green_threshold);
	threshold = find_draw_contours_robots(green_threshold, threshold, green);

	inRange(threshold, lower_blue, higher_blue, blue_threshold);
	threshold = find_draw_contours_robots(blue_threshold, threshold, blue);

	threshold = find_nearest_robots(threshold, red_team);
	threshold = find_nearest_robots(threshold, green_team);
	threshold = find_nearest_robots(threshold, blue_team);
	cvtColor(threshold, threshold, COLOR_HSV2BGR);
	return threshold;
}

vector<vector<Point>> find_template(Mat gk_template) {
	Mat threshold_temp;
	cvtColor(gk_template, threshold_temp, COLOR_BGR2GRAY);
	threshold(threshold_temp, threshold_temp, 100, 255, THRESH_BINARY);
	findContours(threshold_temp, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	/*cvtColor(threshold_temp, threshold_temp, COLOR_GRAY2BGR);
	polylines(threshold_temp, contours, true, red, 2, 10); //рисование контуров
	imshow("temp", threshold_temp);
	waitKey(0);*/
	return contours;
}


Mat find_gk(Mat myimg, vector<vector<Point>> temp_contour) {
	Mat threshold;
	Scalar lower_gk = Scalar(0, 0, 0);
	Scalar higher_gk = Scalar(179, 255, 250);
	cvtColor(myimg, myimg, COLOR_BGR2HSV);
	inRange(myimg, lower_gk, higher_gk, threshold);
	threshold = find_choose_contours_gk(threshold, myimg, temp_contour, green);
	return threshold;
}



Mat find_choose_contours_gk(Mat threshold, Mat myimg_hsv, vector<vector<Point>> temp_contour,  Scalar contour_colour) {
	findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //поиск внешних контуров без аппроксимации
	cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR);
	int min_content = 1000;
	int content;
	double max_diff = 1;
	vector<vector<Point>> good_gk;
	for (int i = 0; i < contours.size(); i++) { //находим центр  масс каждого контура
		content = contourArea(contours[i], false); //площадь контура
		double diff = matchShapes(contours[i], temp_contour[0], ShapeMatchModes::CONTOURS_MATCH_I2, 0); //сравниваем с шаблоном
		if ((content > min_content) && (diff < max_diff)) {
			good_gk.push_back(contours[i]);
		}
	
	}
	polylines(threshold, good_gk, true, contour_colour, 2, 10); //рисование контуров
	return threshold;
}




Mat draw_lamp(Mat myimg_hsv, Mat threshold) {
	findContours(threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //поиск внешних контуров без аппроксимации
	cvtColor(myimg_hsv, threshold, COLOR_HSV2BGR); //НА ЭТОЙ СТРОКЕ ВСЕ ЛОМАЕТСЯ В ФАЙЛЕ CHECK.CPP
	int min_content = 170;
	int content;
	for (int i = 0; i < contours.size(); i++) { //находим центр  масс каждого контура
		Moments mom = moments(contours[i]);
		double x = mom.m10 / mom.m00;
		double y = mom.m01 / mom.m00;
		Point center = Point(x, y);
		content = contourArea(contours[i], false);
		if (content >= min_content) {
			circle(threshold, center, 20, yellow, -1, 8, 0); // точка центра контура
			lamp = center;
			break;
		}

	}
	cvtColor(threshold, threshold, COLOR_BGR2HSV);
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
	cvtColor(threshold, threshold, COLOR_BGR2HSV);
	return threshold;
}


Mat draw_caps(Mat threshold, vector<vector<Point>> contours, Scalar colour) {
	int min_content = 110;
	double content;
	for (int i = 0; i < contours.size(); i++) { //находим площадь каждого контура
		Moments mom = moments(contours[i]);
		double x = mom.m10 / mom.m00;
		double y = mom.m01 / mom.m00;
		Point center = Point(x, y);
		content = contourArea(contours[i], false);
		if (content >= min_content) {
			if (colour == red ) {
				double dist = sqrt(pow((lamp.x - center.x), 2) + pow((lamp.y - center.y), 2));
				if (dist > 100) {
					red_team.push_back(center);
				}
				else continue;
			}
			if (colour == green) {
				green_team.push_back(center);
			}
			if (colour == blue) {
				blue_team.push_back(center);
			}
			polylines(threshold, contours[i], true, colour, 2, 10); //рисование контуров роботов

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

Mat find_nearest_robots(Mat threshold, vector<Point> team) {
	double min_dist=1000;
	double dist;
	Point nearest_robot;
	for (int i = 0; i < team.size(); i++) {
		dist = sqrt(pow((lamp.x - team[i].x), 2) + pow((lamp.y - team[i].y), 2));
		if (dist < min_dist) {
			min_dist = dist;
			nearest_robot = team[i];
		}
	}
	circle(threshold, nearest_robot, 5, purple, -1, 8, 0); // точка центра контура
	return threshold;
}