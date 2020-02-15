#include "ShowManyImg.h"

void ShowManyImages(string title, Mat img1, Mat img2, Mat img3, Mat img4) {
    Mat win_mat(Size(1064, 1084), CV_32F);

    img1.copyTo(win_mat(Rect(0, 0, 512, 512)));
    img2.copyTo(win_mat(Rect(512, 513, 540, 540)));
    img3.copyTo(win_mat(Rect(0, 513, 540, 540)));
    img4.copyTo(win_mat(Rect(540,0, 512, 512)));

    resize(win_mat, win_mat, win_mat.size()/2, 0.5, 0.5, INTER_NEAREST);
    imshow(title, win_mat);
    waitKey(0);
}