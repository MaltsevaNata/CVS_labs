#include "Correlation.h"

void Correlation(Mat number, Mat letter, String title) {
	number.convertTo(number, CV_32F);

	//преобразование номера
	Size dftSize;
	dftSize.width = getOptimalDFTSize(number.cols+letter.cols-1);
	dftSize.height = getOptimalDFTSize(number.rows+letter.rows -1);
	Mat modif_num = fourier_transform(number, dftSize);

	//Преобразование буквы
	Mat modif_let = fourier_transform(letter , dftSize);

	//сопряженная матрица формируется засчет параметра conjB = true
	Mat product;
	mulSpectrums(modif_num, modif_let, product, 0, true); //перемножение спектров
	dft(product, product, DFT_INVERSE | DFT_REAL_OUTPUT); //обратное
	normalize(product, product, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
										// viewable image form (float between values 0 and 1).
	//show
	Rect oi(0, 0, number.cols, number.rows);
	product = product(oi);

	double maxVal;
	double minVal;
	Point minLoc;
	Point maxLoc;
	minMaxLoc(product, &minVal, &maxVal, &minLoc, &maxLoc);
	Mat result;
	threshold(product, result, maxVal - 0.05, 255, THRESH_BINARY);

	Mat new_letter(number.size(), CV_32F);
	letter.copyTo(new_letter);

	ShowManyImages(title, number, product, letter, result, false);

}