//
// Created by Михаил Слиняков on 22.09.2024.
//
/*
double zz = ys * sin(pitch) + zs * cos(pitch);
double xxx = xs * cos(yaw) + zz * sin(yaw);
double yyy = ys * cos(pitch) - zs * sin(pitch);
double zzz = -1 * xs * sin(yaw) + zz * cos (yaw);
 */

/*
zs = zs * cos(pitch) + ys * sin(pitch);
ys = -1 * zs * sin(pitch) + ys * cos(pitch);
*/

#include <cmath>

#include "Camera.h"
#include <iostream>

std::vector<double> Camera::GetPointProjection(double x, double y, double z)
{
    double xs = x_screen - x_view;
    double ys = y_screen - y_view;
    double zs = z_screen - z_view;

    xs = xs + x_view;
    ys = ys + y_view;
    zs = zs + z_view;

    double y_proj, x_proj;
    double d = z_view - zs;

    y_proj = -1 * (ys - d * (ys - y) / (z_view - z));
    x_proj = xs + d * (x - xs) / (z_view - z);


    //std::cout << zs << " " << z << std::endl;
    std::vector<double> vec = {x_proj, y_proj};

    return vec;
}
