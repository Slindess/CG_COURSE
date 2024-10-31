//
// Created by Михаил Слиняков on 22.09.2024.
//

#ifndef CG_CAMERA_H
#define CG_CAMERA_H

#include <vector>

class Camera {
public:
    /* ТУТ ЦЕНТР ЭКРАНА ПРОЕЦИРОВАНИЯ */
    double x_screen;
    double y_screen;
    double z_screen;

    /* ТОЧКА ВЗГЛЯДА КАМЕРЫ */
    double x_view;
    double y_view;
    double z_view;

    /* АВИАЦИЯ */
    double pitch; // Тангаж
    double yaw; // Рыскание
    double roll; // Крен

    int height;
    int width;

    Camera(double xs, double ys, double zs, double xv, double yv, double zv) :
    x_screen(xs), y_screen(ys), z_screen(zs), x_view(xv), y_view(yv), z_view(zv),
    pitch(0), yaw(0), roll(0), height(625), width(570) {}
    std::vector<double> GetPointProjection(double x, double y, double z);
};


#endif //CG_CAMERA_H
