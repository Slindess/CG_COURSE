//
// Created by Михаил Слиняков on 20.10.2024.
//

#include "PolygonDrawAdapter.h"
#include "../../Light/Light.h"
#include "../../Drawer/QtDrawer.h"
#include "../../Objects/Concrete/PolygonObject.h"
#include "../../Components/Concrete/Polygon.h"
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>

std::vector<double> cross(const std::vector<double>& a, const std::vector<double>& b) {
    return {
            a[1] * b[2] - a[2] * b[1],  // Cx
            a[2] * b[0] - a[0] * b[2],  // Cy
            a[0] * b[1] - a[1] * b[0]   // Cz
    };
}

double dot(const std::vector<double>& a, const std::vector<double>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


 bool RayIntersectsTriangle(
        const std::vector<double>& rayOrigin,
        const std::vector<double>& rayDir,
        const Polygon& polygon,
        double t0,
        std::vector<double>& intersectionPoint, std::ofstream &outFile)
{
    // Координаты вершин треугольника
    std::vector<double> v0 = { polygon.x1, polygon.y1, polygon.z1 };
    std::vector<double> v1 = { polygon.x2, polygon.y2, polygon.z2 };
    std::vector<double> v2 = { polygon.x3, polygon.y3, polygon.z3 };

    double a = v0[0] - v1[0];
    double b = v0[1] - v1[1];
    double c = v0[2] - v1[2];
    double d = v0[0] - v2[0];
    double e = v0[1] - v2[1];
    double f = v0[2] - v2[2];

    double g = (rayDir[0]);
    double h = (rayDir[1]);
    double i = (rayDir[2]);

    // Разность между началом луча и первой вершиной треугольника
    double j = v0[0] - rayOrigin[0];
    double k = v0[1] - rayOrigin[1];
    double l = v0[2] - rayOrigin[2];

    /*
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    std::cout << "d = " << d << "\n";
    std::cout << "e = " << e << "\n";
    std::cout << "f = " << f << "\n";
    std::cout << "g = " << g << "\n";
    std::cout << "h = " << h << "\n";
    std::cout << "i = " << i << "\n";
    std::cout << "j = " << j << "\n";
    std::cout << "k = " << k << "\n";
    std::cout << "l = " << l << "\n";
     */

    outFile << "a = " << a << "\n";
    outFile << "b = " << b << "\n";
    outFile << "c = " << c << "\n";
    outFile << "d = " << d << "\n";
    outFile << "e = " << e << "\n";
    outFile << "f = " << f << "\n";
    outFile << "g = " << g << "\n";
    outFile << "h = " << h << "\n";
    outFile << "i = " << i << "\n";
    outFile << "j = " << j << "\n";
    outFile << "k = " << k << "\n";
    outFile << "l = " << l << "\n";



    // Вычисляем определитель матрицы |A|
    double detA = a * (e * i - f * h) + b * (f * g - d * i) + c * (d * h - e * g);
    outFile << "detA = " << detA << "\n";

    if (fabs(detA) < 1e-6)
    {
        return false;
    }

    // Вычисляем параметр t (время пересечения)
    double t = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / detA;

    if (t <= t0 || t <= 0)
    {
        return false; // Луч направлен в обратную сторону
    }

    // Вычисляем параметр γ
    double gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / detA;
    if (gamma < 0 || gamma > 1)
    {
        return false; // Точка пересечения находится вне треугольника
    }

    // Вычисляем параметр β
    double beta = (j * (e * i - f * h) + k * (f * g - d * i) + l * (d * h - e * g)) / detA;
    if (beta < 0 || beta + gamma > 1)
    {
        return false; // Точка пересечения вне треугольника
    }

    // Вычисляем точку пересечения
    intersectionPoint = {
            rayOrigin[0] + t * rayDir[0],
            rayOrigin[1] + t * rayDir[1],
            rayOrigin[2] + t * rayDir[2]
    };

    return true; // Пересечение найдено
}

/*
bool RayIntersectsTriangle(
        const std::vector<double>& rayOrigin,
        const std::vector<double>& rayDir,
        const Polygon& polygon,
        double& t)


{
    std::vector<double> v0 = { polygon.x1, polygon.y1, polygon.z1 };
    std::vector<double> v1 = { polygon.x2, polygon.y2, polygon.z2 };
    std::vector<double> v2 = { polygon.x3, polygon.y3, polygon.z3 };
    const double EPSILON = 1e-9;
    std::vector<double> edge1 = { v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2] };
    std::vector<double> edge2 = { v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2] };
    std::vector<double> h = cross(rayDir, edge2);
    double a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // Луч параллелен треугольнику

    double f = 1.0 / a;
    std::vector<double> s = { rayOrigin[0] - v0[0], rayOrigin[1] - v0[1], rayOrigin[2] - v0[2] };
    double u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;

    std::vector<double> q = cross(s, edge1);
    double v = f * dot(rayDir, q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    t = f * dot(edge2, q); // Пересечение найдено
    return t > EPSILON;
}
*/

PolygonDrawAdapter::PolygonDrawAdapter(std::shared_ptr<QtDrawer> drawer) : _drawer(drawer)
{}

void PolygonDrawAdapter::Draw(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
{
    LightSource lightSource(20, 0, 0);
    std::ofstream outFile("../../p.txt");
    for (int x = 0; x < camera->width; ++x)
    {
        for (int y = 0; y < camera->height; ++y)
        {
            std::vector<double> rayOrigin = { camera->x_view, camera->y_view, camera->z_view };

            double physX = camera->x_screen + (x - camera->width / 2);
            double physY = camera->y_screen + (y - camera->height / 2);
            double physZ = camera->z_screen;
            std::vector<double> rayDir = { physX - camera->x_view, physY - camera->y_view, physZ - camera->z_view };
            double magnitude = sqrt(rayDir[0] * rayDir[0] + rayDir[1] * rayDir[1] + rayDir[2] * rayDir[2]);

            rayDir[0] /= magnitude;
            rayDir[1] /= magnitude;
            rayDir[2] /= magnitude;

            double t0 = (camera->z_view - rayOrigin[2]) / rayDir[2];

            if (physX == 0.0 && physY == 0.0)
                std::cout << physX << " " << physY << " " << " Direction: " << rayDir[0] << " " << rayDir[1] << " " << rayDir[2] << " ORIGIN: " <<  rayOrigin[0] << " " << rayOrigin[1] << " " << rayOrigin[2] << " " << t0 << "\n";
            for (const auto& object : scene->objects)
            {
                for (const auto& component : object->GetComponents())
                {
                    auto polygon = std::dynamic_pointer_cast<Polygon>(component);
                    if (polygon)
                    {
                        std::vector<double> intersectionPoint;
                        if (RayIntersectsTriangle(rayOrigin, rayDir, *polygon, t0, intersectionPoint, outFile))
                        {
                            double imageX = physX;
                            double imageY = -1 * physY;
                            Color illuminatedColor = polygon->color;
                            _drawer->setColor(0, 0, 255);
                            _drawer->drawPoint(imageX, imageY);
                            _drawer->setColor(255, 0, 0);
                            _drawer->drawPoint(0, 0);
                        }
                    }
                }
            }
        }
    }
    outFile.close();
}


PolygonDrawAdapter::~PolygonDrawAdapter(){}