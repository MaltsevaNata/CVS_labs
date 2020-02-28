#include "libs/global.h"
#include "libs/read&check.h"

#include "libs/Convolution.h"
#include "libs/CV_Fourier.h"
#include "libs/LF_HF_filter.h"
#include "libs/Correlation.h"

#define apply_my_four false

String lenna_path = "D:/Study/STV_labs/CVS_labs/lab4/templates";
String number_path = "D:/Study/STV_labs/CVS_labs/lab4/numbers";

int main()
{	
	if (apply_my_four) {
		Mat image = imread(lenna_path + "/2.jpg", IMREAD_GRAYSCALE);
		namedWindow("img", WINDOW_NORMAL);
		imshow("img", image);
		if (image.empty()) {
			std::cout << "failed to open image" << std::endl;
			return -1;
		}
		Size dftSize;
		//dftSize.width = clp2(image.cols )  ;
		//dftSize.height = clp2(image.rows );
		dftSize.width = getOptimalDFTSize(image.cols);
		dftSize.height = getOptimalDFTSize(image.rows);
		Mat modif_img = fourier_transform(image, dftSize);

		Mat beaut_img = krasivSpektr(modif_img); //includes make_magnitude
		normalize(beaut_img, beaut_img, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
										// viewable image form (float between values 0 and 1).
		namedWindow("sys fourier", WINDOW_NORMAL);
		imshow("sys fourier", beaut_img);
		my_fourier(image, dftSize);
	}
	//Radix2(image, dftSize);

	else {
			Mat image = imread(lenna_path + "/Lenna.png", IMREAD_GRAYSCALE);
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
	}

	return 0;
}
