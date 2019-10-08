//
// Created by natas on 07.10.2019.
//

#include "approximation.h"

vector <double> angle; // углы поворота тележки в градусах

vector <double> approximate(vector<double> rows, vector<double> cols, Mat image, int roi_num , int degree) {
    int roi_width = int(round(image.cols / roi_num));
    int index = 0;
    for (int segment = 1; segment <= roi_num; segment++) { //разделение изображения на roi_num областей интереса
        vector<double > roi_x;
        vector<double > roi_y;
        for (index; (rows[index] < roi_width * segment) && (index<rows.size()); index++) {
            roi_x.push_back(rows[index]);
            roi_y.push_back(cols[index]);
        }
        Matrix A = find_matrix_A(roi_x, roi_y, degree); //аппроксимация синусоиды в выбранном отрезке полиномом степени degree, нахождение коэф аi
        angles(A, roi_x, 3); //вычисление угла в каждой точке отрезка
        roi_x.clear();
        roi_y.clear();
    }
    return angle;
}

Matrix find_matrix_A ( vector <double > roi_x, vector <double > roi_y, unsigned int n) {
    vector<double> X;
    X.resize(pow(n+1, 2)); //matrix 4x4
    for (int i = 0; i < n+1; i++) {
        for (int j = 0; j < n+1; j++) {
            for (int xi = 0; xi < roi_x.size(); xi++) {
                X[i*(n+1) + j] += pow(roi_x[xi], (i + j));
            }
        }
    }
    X[0] = roi_x.size();
    vector<double> Y;
    Y.resize(n+1); //matrix 4x1
    for (int i = 0; i < n+1; i++) {
        for (int xiyi = 0; xiyi < roi_x.size(); xiyi ++) {
            Y[i] += pow(roi_x[xiyi], i) * roi_y[xiyi];
        }
    }
    Matrix X1 = {n+1, n+1, X};
    Matrix Y1 = {n+1, 1, Y};
    Y.clear();
    X.clear();
    Matrix inverted_X = X1.invert(); //все верно
    Matrix result = inverted_X * Y1;
    return result;
};

void angles(Matrix A, vector <double > xi, unsigned int n) {
    double derivative;
    for (int i = 0; i < xi.size(); i++) {
        derivative = 0;
        for (int degree = 1; degree < n+1; degree ++) {
            derivative += A.data[degree] * (degree) * pow(xi[i], degree-1 );
        }
        angle.push_back((atan(derivative))*180/pi);
    }

}