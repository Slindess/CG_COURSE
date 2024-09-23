//
// Created by Михаил Слиняков on 22.09.2024.
//

#ifndef CG_CAMERA_H
#define CG_CAMERA_H


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
};


#endif //CG_CAMERA_H
