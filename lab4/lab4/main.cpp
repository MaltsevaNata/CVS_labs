#include "libs/global.h"
#include "libs/read&check.h"

#include "libs/Convolution.h"
#include "libs/CV_Fourier.h"
#include "libs/LF_HF_filter.h"
#include "libs/Correlation.h"

String lenna_path = "D:/Study/STV_labs/CVS_labs/lab4/templates";
String number_path = "D:/Study/STV_labs/CVS_labs/lab4/numbers";

int main()
{
	Mat image = imread(lenna_path + "/Lenna.png", IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cout << "failed to open image" << std::endl;
		return -1;
	}

	//Task 1 
	for (int filter = (int)SOBEL_HOR; filter <= (int)LAPLAS; ++filter) {
		convolution(image, filter);
	}

	//Task 2
	LF_HF_filter(image);

	//Task 3
	String Letters[3] = { "A", "M", "7" };
	Mat number = imread(number_path + "/" + "avto_nomer" + ".jpg", IMREAD_GRAYSCALE);
	for (int i = 0; i < 3; i++) {
		Mat letter = imread(number_path + "/" + Letters[i] + ".jpg", IMREAD_GRAYSCALE);
		Correlation(number, letter, Letters[i]);
	}

	return 0;
}
