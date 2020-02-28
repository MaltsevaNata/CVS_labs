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
	normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
									// viewable image form (float between values 0 and 1).
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

Mat krasivSpektr1(Mat magI) {
	//Mat magI = make_magnitude(complexI);
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



Mat my_fourier(Mat image, Size dftSize) {
	Mat padded; //expand input image to optimal size
	copyMakeBorder(image, padded, 0, dftSize.height - image.rows, 0, dftSize.width - image.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<uint8_t>(padded), Mat::zeros(padded.size(), CV_8UC1) };
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
	int N = complexI.cols;
	int M = complexI.rows;
	double Real ;//output
	double Im ;

	double degree;
	Mat result = Mat(complexI.size(), CV_32FC2, Scalar(0, 0));
	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {
			Real = 0;
			Im = 0;
			for (int i = 0; i < N; i++) {
				for (int k = 0; k < M; k++) {
					degree = -_2PI * ((double)n * i / N + (double)m * k / M);
					Real += complexI.at<uint8_t>(k, i) * cos(degree);
					Im += complexI.at<uint8_t>(k, i) * sin(degree);
				}
			}
			result.at<Vec2f>(m, n) = Vec2f(Real, Im);
		}
		

	}
	result = krasivSpektr(result);
	namedWindow("my fourier", WINDOW_NORMAL);
	imshow("my fourier", result);
	waitKey(0);
	return result;
}

uint32_t clp2(uint32_t x) {
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}


Mat Radix2(Mat image, Size dftSize) {
	Mat complexI;
	copyMakeBorder(image, complexI, 0, dftSize.height - image.rows, 0, dftSize.width - image.cols, BORDER_CONSTANT, Scalar::all(0));
	int N = complexI.cols;
	int M = complexI.rows;
	int step_rows = N;
	int step_cols = M;
	float* result_Real_rows = new float[N];
	float* result_Im_rows = new float[N];
	
	float* result_Real_cols = new float[M];
	float* result_Im_cols = new float[M];
	Mat tempR = Mat(complexI.size(), CV_32FC1, Scalar(0, 0));
	Mat tempI = Mat(complexI.size(), CV_32FC1, Scalar(0, 0));
	uint8_t* str;
	for (int m = 0; m < M; m++) {
		for (int i = 0; i < N; i++) {
			result_Im_rows[i] = 0;
		}
		str = complexI.ptr<uint8_t>(m);
		//деление пополам  и сортировка до 1 элемента по строкам
		split_uint(str, result_Real_rows, N, step_rows); //потом вызывается split для float
		//преобразование
		butterfly(result_Real_rows, result_Im_rows, N, 0);
		for (int k = 0; k < N; k++) {
			tempR.at<float>(m, k) = result_Real_rows[k];
			tempI.at<float>(m,k) = result_Im_rows[k];
		}
	}
	
	Mat resultR = tempR.t();
	Mat resultI = tempI.t();
	
	//по столбцам
	Mat result = Mat(complexI.size(), CV_32FC2, Scalar(0, 0));
	for (int n = 0; n < N; n++) {
		float* strR = resultR.ptr<float>(n);
		float* strI = resultI.ptr<float>(n);
		split(strR, result_Real_cols, M, step_cols);
		split(strI, result_Im_cols, M, step_cols);
		//преобразование
		butterfly(result_Real_cols, result_Im_cols, M, 0);
		for (int k = 0; k < M; k++) {
			//rows[m].push_back(result_rows[k]);
			result.at<Vec2f>(n, k) = Vec2f(result_Real_cols[k], result_Im_cols[k]);
		}
	}
	//result = result.t();
	//result = krasivSpektr(result);
	
	/*
	Mat result = Mat(complexI.size(), CV_32FC2, Scalar(0, 0));
	for (int n = 0; n < M; n++) {
		for (int k = 0; k < N; k++) {
			result.at<Vec2f>(n, k) = Vec2f(tempR.at<float>(n, k), tempI.at<float>(n, k));
		}
	}*/
	//result = result.t();
	//result = make_magnitude(result);
	result = krasivSpektr(result);
	namedWindow("my fourier", WINDOW_NORMAL);
	imshow("my fourier", result);
	waitKey(0);

	delete[] result_Real_rows;
	delete[] result_Im_rows;
	delete[] result_Real_cols;
	delete[] result_Im_cols;
	return image;
}


void split_uint(uint8_t* input_mas, float* result_mas, int size, int step) {
	if (step % 2 == 0) {
		for (int st = 0; st < size / step; st++) {
			vector <float> even;
			vector <float> uneven;
			for (int n = 0; n < step; n++) {
				if (n % 2 == 0) {
					int num = step * st + n;
					even.push_back(float(input_mas[step * st + n]));
				}
				else {
					uneven.push_back(float(input_mas[step * st + n]));
				}
			}
			for (int i = 0; i < step; i++) {
				if (i < step / 2) {
					result_mas[step * st + i] = even[i];
				}
				else result_mas[step * st + i] = uneven[i - step / 2];
			}
		}
	}
	else {
		return;
	}
	step = step / 2;
	split(result_mas, result_mas, size, step);
}

void split(float* input_mas, float* result_mas, int size, int step) {
	if (step % 2 == 0) { //если возможно поровну поделить пополам массив
		for (int st = 0; st < size / step; st++) { //st -количество шагов в массиве, равное size/step
			vector <float> even;
			vector <float> uneven;
			for (int n = 0; n < step; n++) {
				if (n % 2 == 0) { //четные записываем в вектор четных 
					//int num = step * st + n;
					even.push_back(input_mas[step * st + n]); // элемент номер длина_шага * номер_шага + n элементов внутри шага
				}
				else {
					uneven.push_back(input_mas[step * st + n]); //в вектор нечетных
				}
			}
			for (int i = 0; i < step; i++) { //из векторов четных и нечетных в общий массив - сначала четные, потом после середины нечетные
				if (i < step / 2) {
					result_mas[step * st + i] = even[i];
				}
				else result_mas[step * st + i] = uneven[i - step / 2];
			}
		}
	}
	else {
		return;
	}
	step = step / 2;
	split(result_mas, result_mas, size, step);
}

void butterfly(float* XReal, float* XIm, int size, int K) {
	float W_Real1;
	float W_Im1;
	float W_Real2;
	float W_Im2;
	float im1 =0;
	float real1=0;
	float im2 = 0;
	float real2 = 0;
	for (int el = 0; el < size; el++) { 
		for (int k = 0; k <= K; k++) { //проход по паре детей из (K+1) элементов массива для получения родителя из (K+1)*2 элементов
			W_Real1 = cos(_2PI * k / (2 * ( K + 1)));
			W_Im1 = -sin(_2PI * k / (2 * (K + 1)));
			W_Real2 = cos(_2PI * (k + K + 1) / (2 * (K + 1)));
			W_Im2 = -sin(_2PI * (k + K + 1) / (2 * (K + 1)));
			real1 = XReal[el] + W_Real1 * XReal[el + K + 1] - W_Im1 * XIm[el + K + 1]; //el + K + 1 это индекс элемента с такой же k частотой во 
																					// втором ребенке 
			im1 = XIm[el] + W_Real1 * XIm[el + K + 1] + W_Im1 * XReal[el + K + 1];
			real2 = XReal[el + K + 1] + W_Real2 * XReal[K] - W_Im2 * XIm[K];
			im2 = XIm[el + K + 1] + W_Real2 * XIm[K] + W_Im2 * XReal[K];
			XReal[el] = real1;
			XReal[el + K + 1] = real2;
			XIm[el] = im1;
			XIm[el + K + 1] = im2;
			el++; //переход к элементу со следующей частотой
		}
		el += K; //переход к следующей паре детей (прыжок через 2го ребенка, которого мы уже учли)
	}
	K = (K * 2) + 1;
	if (K < size-1) {
		butterfly(XReal, XIm, size, K);
	}
	else
		return;
}
/*
void butterfly_uint(uint8_t* XReal, uint8_t* XIm, int size, int K) {
	float W_Real1;
	float W_Im1;
	float W_Real2;
	float W_Im2;
	float im1 = 0;
	float real1 = 0;
	float im2 = 0;
	float real2 = 0;
	for (int el = 0; el < size; el++) {
		for (int k = 0; k <= K; k++) { //проход по паре детей из (K+1) элементов массива для получения родителя из (K+1)*2 элементов
			W_Real1 = cos(_2PI * k / (K + 1));
			W_Im1 = -sin(_2PI * k / (K + 1));
			W_Real2 = cos(_2PI * (k + K + 1) / (K + 1));
			W_Im2 = -sin(_2PI * (k + K + 1) / (K + 1));
			real1 = XReal[el] + W_Real1 * XReal[el + K + 1] - W_Im1 * XIm[el + K + 1]; //el + K + 1 это индекс элемента с такой же k частотой во 
																					// втором ребенке 
			im1 = XIm[el] + W_Real1 * XIm[el + K + 1] + W_Im1 * XReal[el + K + 1];
			real2 = XReal[el + K + 1] + W_Real2 * XReal[K] - W_Im2 * XIm[K];
			im2 = XIm[el + K + 1] + W_Real2 * XIm[K] + W_Im2 * XReal[K];
			XReal[el] = real1;
			XReal[el + K + 1] = real2;
			XIm[el] = im1;
			XIm[el + K + 1] = im2;
			el++; //переход к элементу со следующей частотой
		}
		el += K; //переход к следующей паре детей (прыжок через 2го ребенка, которого мы уже учли)
	}
	K = (K * 2) + 1;
	if (K < size - 1) {
		butterfly(XReal, XIm, size, K);
	}
	else
		return;
}
*/
/*
void butterfly(uint8_t** XReal, uint8_t** XIm, uint8_t* inputX_Real, uint8_t* inputX_Im, int size, int K) {
	double W_Real;
	double W_Im;
	for (int el = 0; el < size; el ++) {
		W_Real = cos(_2PI * K / (size));
		W_Im = -sin(_2PI * K / (size));
		if (K < size / 2) { 
			uint8_t s1 = inputX_Real[el];
			uint8_t s2 = W_Real * inputX_Real[el + 1];
			uint8_t s3 = W_Im * inputX_Im[el + 1];
			XReal[K][el] = inputX_Real[el] + W_Real * inputX_Real[el + 1] - W_Im * inputX_Im[el + 1];
			XIm[K][el] = inputX_Im[el] + W_Real * inputX_Im[el + 1] + W_Im * inputX_Real[el + 1];
		}
		else {
			XReal[K][el] = inputX_Real[el - size / 2] + W_Real * inputX_Real[el - size / 2 + 1] - W_Im * inputX_Im[el - size / 2 + 1];
			XIm[K][el] = inputX_Im[el-size / 2] + W_Real * inputX_Im[el -size / 2 + 1] + W_Im * inputX_Real[el - size / 2 + 1];
		}
		el ++;
	}
	
	int p = pow(2, K);
	if (K < (log10(size * p) / log10(2))) {
		K++;
		butterfly(XReal, XIm, XReal[K - 1], XIm[K - 1], size / 2, K);
	}
	else return;
}

*/
	/*vector<vector<uint8_t>> result;
	if (mas.size() % 2 == 0) {
		for (int n = 0; n < mas.size(); n++) {
			if (n < mas.size() / 2) {
				result[0].push_back(mas[n]);

			}
			else {
				result[1].push_back(mas[n]);
			}
		}
		result.at(0) = split_strings(result[0]);
		result.at(1) = split_strings(result[1]);
	}

	return result;
}*/