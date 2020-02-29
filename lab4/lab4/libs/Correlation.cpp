#include "Correlation.h"

void Correlation(Mat number, Mat letter, String title) {
	number.convertTo(number, CV_32FC1);
	letter.convertTo(letter, CV_32FC1);
	Mat mean_number; Mat result_number;
	Mat mean_letter; Mat result_letter;
	Mat dev;
	meanStdDev(number, mean_number, dev);
	meanStdDev(letter, mean_letter, dev);
	result_number = (number - mean_number) / mean_number;
	result_letter = (letter - mean_letter) / mean_letter;
	//namedWindow("mean_let", WINDOW_NORMAL);
	imshow("mean_let", result_letter);

	//преобразование номера
	Size dftSize;
	dftSize.width = getOptimalDFTSize(result_number.cols+result_letter.cols-1);
	dftSize.height = getOptimalDFTSize(result_number.rows+result_letter.rows -1);
	Mat modif_num = fourier_transform(result_number, dftSize);

	//Преобразование буквы
	Mat modif_let = fourier_transform(result_letter , dftSize);

	//сопряженная матрица формируется засчет параметра conjB = true
	Mat product;
	mulSpectrums(modif_num, modif_let, product, 0, true); //перемножение спектров
	dft(product, product, DFT_INVERSE | DFT_REAL_OUTPUT); //обратное
	normalize(product, product, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
										// viewable image form (float between values 0 and 1).

	//show
	Rect oi(0, 0, result_number.cols, result_number.rows);
	product = product(oi);
	namedWindow("product", WINDOW_AUTOSIZE);
	//imshow("product", product);
	double maxVal;
	double minVal;
	Point minLoc;
	Point maxLoc;
	double value = 0.07;
	if (title == "7_little") {
		value = 0.02;
	}
	//product.convertTo(product, CV_8UC1);
	minMaxLoc(product, &minVal, &maxVal, &minLoc, &maxLoc);
	Mat result;
	
	threshold(product, result, maxVal - value, 255, THRESH_BINARY);
	//normalize(result, result, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
										// viewable image form (float between values 0 and 1).

	namedWindow("mean_num", WINDOW_NORMAL);
	imshow("mean_num", result_number);
	namedWindow("product", WINDOW_NORMAL);
	imshow("product", product);
	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", result);
	waitKey(0);
		


	//}
	//ShowManyImages(title, result_number, product, result_letter, result, false);

}

//namedWindow("threshold", WINDOW_NORMAL);
//int lowH = createTrackbar("threshold", "threshold", &value, 179, 0); // оттенок min
//while (!waitKey(0)) {
	//setTrackbarPos("threshold", title, value);