#include "libs/approximation.h"
#include "libs/rectangle.h"


//выбираем количество обл интереса roi, по точкам траектории аппроксимируем полиномом, находим производную в каждой точке -> угол поворота тележки
int main() {
    Mat myimg=imread("mars.jpg");
    vector <vector<double>> x_y = find_coord(myimg, "sin"); //координаты точек, по которым движется тележка
    vector <double> angle = approximate(x_y[0], x_y[1], myimg, 5, 3); //аппроксимируем синусоиду в 5 roi кубической функцией и получаем угол наклона тележки в каждой точке
    draw_moving_rect(myimg, x_y[0], x_y[1], angle); // по вычисленным координатам движется тележка по касательной к функции
    x_y.clear(); angle.clear();
    return 0;
}




