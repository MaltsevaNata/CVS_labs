//
// Created by natas on 08.10.2019.
//

#include "rectangle.h"
#define points_num 180


vector <vector<double>> find_coord(Mat image, string func) {
    vector<double> x;
    x.resize(points_num);//значения координат точек, по которым будет двигаться тележка
    vector<double>y;
    y.resize(points_num);
    for (int i = 0; i < points_num; i++) {
        if (func == "sin") {
            y[i] = image.rows * (1 - sin(i * pi / points_num)) + image.rows/8;
        }
        else if (func == "cos") {
            y[i] = image.rows * (1 - cos(i * pi / points_num)) + image.rows/8;
        }
        else {
            cout<<"Unknown function"<<endl;
            exit(1);
        }
        x[i] = image.cols  * i / points_num;
    }
    vector <vector<double>> coordinates;
    coordinates.push_back(x);
    coordinates.push_back(y);
    x.clear();
    y.clear();
    return coordinates;
}


void draw_moving_rect(Mat image, vector<double> x, vector<double> y, vector <double> angles) {
    Mat image_copy = image.clone();
    char* window_name = "mars";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(255,0,0);
    Point2f prev_center = {0,0};
    Point2f vertices2f[4];
    Point vertices[4];
    for (int i=0; i<180; i++) {
        RotatedRect rRect = RotatedRect(Point2f(x[i], y[i]) /* center*/ , Size2f(50,100) /*rect size*/, angles[i] + 90);
        rRect.points(vertices2f);
        for (int i = 0; i < 4; i++) {
            line(image_copy, vertices2f[i], vertices2f[(i + 1) % 4], green, 2); //draw rRect
            vertices[i] = vertices2f[i];
        }
        fillConvexPoly(image_copy, vertices, 4, green);
        prev_center = rRect.center;
        line(image, prev_center, rRect.center, blue, 2 ); //trajectory
        imshow(window_name, image_copy);
        if (i==90) {
            imwrite( "Saves/mars_trolley.jpg", image_copy );
        }
        image_copy = image.clone();
        waitKey(50);
    }


}