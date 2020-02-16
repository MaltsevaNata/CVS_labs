#include "ShowManyImg.h"

void ShowManyImages(string title, Mat img1, Mat img2, Mat img3, Mat img4, bool resiz) {
    double maxVal;
    double minVal;
    Point minLoc;
    Point maxLoc;

    Mat win_mat(Size(img1.cols+img2.cols +1, img1.rows + img3.rows +100), CV_8U);

    minMaxLoc(img1, &minVal, &maxVal, &minLoc, &maxLoc);
    img1.convertTo(img1, CV_8U, 255.0 / (maxVal - minVal), -255.0 * minVal / (maxVal - minVal));
    img1.copyTo(win_mat(Rect(0, 0, img1.cols, img1.rows)));

    minMaxLoc(img2, &minVal, &maxVal, &minLoc, &maxLoc);
    img2.convertTo(img2, CV_8U, 255.0 / (maxVal - minVal), -255.0 * minVal / (maxVal - minVal));
    img2.copyTo(win_mat(Rect(img1.cols, img1.rows, img2.cols, img2.rows)));

    minMaxLoc(img3, &minVal, &maxVal, &minLoc, &maxLoc);
    img3.convertTo(img3, CV_8U, 255.0 / (maxVal - minVal), -255.0 * minVal / (maxVal - minVal));
    img3.copyTo(win_mat(Rect(0, img1.rows, img3.cols, img3.rows)));

    minMaxLoc(img4, &minVal, &maxVal, &minLoc, &maxLoc);
    img4.convertTo(img4, CV_8U, 255.0 / (maxVal - minVal), -255.0 * minVal / (maxVal - minVal));
    img4.copyTo(win_mat(Rect(img1.cols, 0, img4.cols, img4.rows)));
    if (resiz) {
        resize(win_mat, win_mat, win_mat.size() / 2, 0.5, 0.5, INTER_NEAREST);
    }
    imshow(title, win_mat);
    waitKey(0);
}