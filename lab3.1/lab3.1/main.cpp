
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <opencv2/core/types.hpp>
#include "windows.h"

#include "libs/read&check.h"
#include "libs/process_image.h"

extern string current_path;
extern String path_roboti;

int main()
{
	Mat processed;
	int img_num = 0;
	int new_img_num = img_num;
	vector<string> images; 
	glob(current_path, images, false);
	while ((img_num < images.size()) && (img_num >= 0)) {
		String imageName = images[img_num];
		Mat myimg = read_img(imageName);
		namedWindow(imageName, WINDOW_AUTOSIZE);
		if (current_path == path_roboti) {
			Mat tempProcessed = process_image_lab3(imageName, myimg);
			while (img_num == new_img_num) {
				processed = update_image_robots(tempProcessed);
				namedWindow("Result", WINDOW_AUTOSIZE);
				imshow("Result", processed);
				new_img_num = check_pressed_but(img_num);
			}
		}
		else {
			Mat tempProcessed = process_image_lab12(imageName, myimg);
			while (img_num == new_img_num) {
				processed = update_image(tempProcessed);
				namedWindow("Result", WINDOW_AUTOSIZE);
				imshow("Result", processed);
				//imshow(imageName, myimg);
				new_img_num = check_pressed_but(img_num);
			}
		}

		img_num = new_img_num;
	}
	return 0;
}



