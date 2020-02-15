#include "CV_Fourier.h"

Mat make_magnitude(Mat complexI) {
	Mat planes[] = { Mat_<float>(complexI), Mat::zeros(complexI.size(), CV_32F) };
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
	Mat magI = planes[0];
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);
	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	return magI;
}


Mat krasivSpektr(Mat complexI) {
	Mat magI = make_magnitude(complexI);
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));// Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));// Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));// Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy));// Bottom-Right

	Mat tmp;// swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);// swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
	return magI;
}


Mat fourier_transform(Mat image, Size dftSize) {
	Mat padded; //expand input image to optimal size
	copyMakeBorder(image, padded, 0, dftSize.height - image.rows, 0, dftSize.width - image.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	dft(complexI, complexI, DFT_COMPLEX_OUTPUT);            // this way the result may fit in the source matrix
	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	return complexI;

}