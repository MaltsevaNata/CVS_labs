#include "libs/global.h"
#include "libs/read&check.h"

#include "libs/Convolution.h"
#include "libs/CV_Fourier.h"

String current_path = "D:/Study/STV_labs/CVS_labs/lab4/templates";

int main()
{
	Mat processed;
	int img_num = 0;
	int new_img_num = img_num;
	vector<string> images;
	glob(current_path, images, false);

	Mat image = imread(images[1], IMREAD_GRAYSCALE);
	
	if (image.empty()) {
		std::cout << "failed to open image" << std::endl;
		return -1;
	}
	else
		std::cout << "image loaded OK" << std::endl;

	for (int filter = (int)SOBEL_HOR; filter <= (int)LAPLAS; ++filter) {
		convolution(image, filter);
	}
	return 0;
}
