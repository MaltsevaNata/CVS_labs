#include "LF_HF_filter.h"

void LF_filter(Mat image) {
	Size dftSize;
	dftSize.width = getOptimalDFTSize(image.cols);
	dftSize.height = getOptimalDFTSize(image.rows);
	Mat modif_img = fourier_transform(image, dftSize);
	Mat beaut_img = krasivSpektr1(modif_img); //для преобразования

	//применение маски
	Mat mask(beaut_img.size(), CV_8UC1, Scalar(0, 0, 0));
	Point c(beaut_img.cols / 2, beaut_img.rows / 2);
	circle(mask, c, 50, Scalar(255,255,255), FILLED);

	Mat lf_img(Size(beaut_img.cols, beaut_img.rows), CV_8UC1);
	beaut_img.copyTo(lf_img, mask);
	Mat lf_img_show = make_magnitude(lf_img);

	//обратное преобразование
	lf_img.convertTo(lf_img, CV_32F);
	Mat res_img(lf_img.size(), CV_32F);
	res_img = krasivSpektr1(lf_img);
	dft(res_img, res_img, DFT_INVERSE | DFT_REAL_OUTPUT);
	image.convertTo(image, CV_32F);
	normalize(image, image, 0, 1, NORM_MINMAX);
	ShowManyImages("Low frequency", image, make_magnitude(res_img), lf_img_show, make_magnitude(modif_img));
}