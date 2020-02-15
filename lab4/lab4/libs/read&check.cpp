#include "read&check.h"


Mat read_img(String imageName) {
	Mat myimg = imread(imageName);
	if (myimg.empty()) {
		std::cout << "failed to open" << imageName << std::endl;
	}
	else {
		std::cout << imageName << " loaded OK" << std::endl;

	}
	return myimg;
}

int check_pressed_but(int img_num) {
	int button = waitKey(33);
	//std::cout << button << std::endl;
	if ((button == 100) || (button == 226)) { //D - следующее изображение
		img_num++;
		destroyAllWindows();
	}
	else if ((button == 97) || (button == 244)) { //A - предыдущее изображение
		img_num--;
		destroyAllWindows();
	}
	return img_num;
}