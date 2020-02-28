#include "Convolution.h"
#include "CV_Fourier.h"
#include "ShowManyImg.h"
#include "Filters.h"

void convolution(Mat image, int filter) {
	string filterName;
	Mat filt(3, 3, CV_32F);
	switch (filter)
	{
	case SOBEL_HOR: {
		filterName = "Sobel filter horizontal";
		float filterData[3][3] = { { 1, 0, -1 },{ 2, 0, -2 },{ 1, 0, -1 } };
		std::memcpy(filt.data, filterData, 3 * 3 * sizeof(float));
		break;
	}
	case SOBEL_VERT: {
		filterName = "Sobel filter vertical";
		float filterData[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };
		std::memcpy(filt.data, filterData, 3 * 3 * sizeof(float));
		break;
	}
	case BOXFILTER: {
		filterName = "Box filter";
		float filterData[3][3] = { { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 },{ 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 },{ 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 } };
		std::memcpy(filt.data, filterData, 3 * 3 * sizeof(float));
		break;
	}
	case LAPLAS: {
		filterName = "Laplas filter";
		float filterData[3][3] = { { 0, 1, 0 },{ 1, -4, 1 },{ 0, 1, 0 } };
		std::memcpy(filt.data, filterData, 3 * 3 * sizeof(float));
		break;
	}
	default:
		printf("Filter not found");
		return;
	}

	//get the canvas size
	Size dftSize;
	dftSize.width = getOptimalDFTSize(image.cols + filt.cols - 1);
	dftSize.height = getOptimalDFTSize(image.rows + filt.rows - 1);

	Mat modif_filter = fourier_transform(filt, dftSize); //прямое 
	Mat modif_img = fourier_transform(image, dftSize);

	Mat beaut_img = krasivSpektr(modif_img); //includes make_magnitude
	Rect oi(0, 0, image.cols, image.rows);
	   	 
	normalize(beaut_img, beaut_img, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
														// viewable image form (float between values 0 and 1).

	Mat product;
	mulSpectrums(modif_img, modif_filter, product, 0, 0); //перемножение спектров
	dft(product, product, DFT_INVERSE | DFT_REAL_OUTPUT); //обратное
		
	Mat roi = product(oi); //обрезка
	normalize(roi, roi, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
											// viewable image form (float between values 0 and 1).

	modif_filter.convertTo(modif_filter, CV_32F);
	Mat beaut_filt = krasivSpektr(modif_filter);
	normalize(beaut_filt, beaut_filt, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
													  // viewable image form (float between values 0 and 1).
	roi.convertTo(roi, CV_32F);

	dft(modif_img, image, DFT_INVERSE | DFT_REAL_OUTPUT);
	image = image(oi);
	normalize(image, image, 0, 1, NORM_MINMAX);
	ShowManyImages(filterName, image, beaut_img, beaut_filt, roi, true);
}
