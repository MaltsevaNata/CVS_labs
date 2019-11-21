#include "trackbars.h"
#include "global.h"

static int h_low, h_high;
static int s_low, s_high;
static int v_low, v_high;

void create_trackBars(string win_name) {
	int lowH = createTrackbar("Hue_low", win_name, &h_low, 179, 0); // оттенок min
	int highH = createTrackbar("Hue_high", win_name, &h_high, 179, 0); // оттенок max
	int lowS = createTrackbar("Sat_low", win_name, &s_low, 255, 0); // насыщенность min
	int highS = createTrackbar("Sat_high", win_name, &s_high, 255, 0); // насыщенность max
	int lowV = createTrackbar("Value_low", win_name, &v_low, 255, 0); // €ркость min
	int highV = createTrackbar("Value_high", win_name, &v_high, 255, 0); // €ркость max
	if (current_path == path_allababah) {
		setTrackbarPos("Hue_low", win_name, 0);
		setTrackbarPos("Hue_high", win_name, 179);
		setTrackbarPos("Sat_low", win_name, 0);
		setTrackbarPos("Sat_high", win_name, 255);
		setTrackbarPos("Value_low", win_name, 227);
		setTrackbarPos("Value_low", win_name, 227);
		setTrackbarPos("Value_high", win_name, 255);
	}
	if (current_path == path_teplovizor) {
		setTrackbarPos("Hue_low", win_name, 0);
		setTrackbarPos("Hue_high", win_name, 46);
		setTrackbarPos("Sat_low", win_name, 0);
		setTrackbarPos("Sat_high", win_name, 255);
		setTrackbarPos("Value_low", win_name, 0);
		setTrackbarPos("Value_low", win_name, 0);
		setTrackbarPos("Value_high", win_name, 255);
	}
}

Mat get_threshold_Bars(Mat img) {
	Scalar lower_hsv, higher_hsv;
	Mat threshold;
	lower_hsv = Scalar(h_low, s_low, v_low);
	higher_hsv = Scalar(h_high, s_high, v_high);
	inRange(img, lower_hsv, higher_hsv, threshold);
	return threshold;

}