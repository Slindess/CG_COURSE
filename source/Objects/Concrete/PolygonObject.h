#ifndef CG_POLYGONOBJECT_H
#define CG_POLYGONOBJECT_H

#include <initializer_list>
#include "../BaseObject.h"
#include "../../Components/Concrete/Polygon.h"

class BoundingSphere {
public:
    std::vector<double> center;
    double radius;

    BoundingSphere(const std::vector<double>& center, double radius) : center(center), radius(radius) {}

    // Метод для проверки пересечения с лучом
    bool Intersects(const std::vector<double>& rayOrigin, const std::vector<double>& rayDir, double& t) {
        std::vector<double> oc = { rayOrigin[0] - center[0], rayOrigin[1] - center[1], rayOrigin[2] - center[2] };
        double a = dot(rayDir, rayDir);
        double b = 2.0 * dot(oc, rayDir);
        double c = dot(oc, oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            return false; // Нет пересечения
        }
        t = (-b - sqrt(discriminant)) / (2.0 * a); // Положение пересечения
        return true; // Пересечение произошло
    }

private:
    double dot(const std::vector<double>& a, const std::vector<double>& b) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
};


class PolygonObject : public BaseObject
{
public:
    PolygonObject(std::initializer_list<std::initializer_list<double>> l);
    PolygonObject(std::vector<std::initializer_list<double>>& l);
    PolygonObject(std::vector<Polygon>& polygons);
    PolygonObject(std::vector<std::vector<double>>& l);
    BoundingSphere Sphere() {return _sphere;};
    ~PolygonObject();

private:
    BoundingSphere _sphere;
    double Distance(const std::vector<double>& a, const std::vector<double>& b) {
        return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
    }

    BoundingSphere CalculateBoundingSphere() {
        // Инициализация переменных для минимальных и максимальных значений координат
        double minX = std::numeric_limits<double>::max(), minY = std::numeric_limits<double>::max(), minZ = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest(), maxY = std::numeric_limits<double>::lowest(), maxZ = std::numeric_limits<double>::lowest();

        // Проходим по компонентам и обновляем минимальные и максимальные координаты для каждого полигона
        for (const auto& component : GetComponents()) {
            auto polygon = std::dynamic_pointer_cast<Polygon>(component);
            if (polygon) {
                // Обновляем минимальные и максимальные координаты на основе вершин текущего полигона
                minX = std::min({minX, polygon->x1, polygon->x2, polygon->x3});
                minY = std::min({minY, polygon->y1, polygon->y2, polygon->y3});
                minZ = std::min({minZ, polygon->z1, polygon->z2, polygon->z3});

                maxX = std::max({maxX, polygon->x1, polygon->x2, polygon->x3});
                maxY = std::max({maxY, polygon->y1, polygon->y2, polygon->y3});
                maxZ = std::max({maxZ, polygon->z1, polygon->z2, polygon->z3});
            }
        }

        // Вычисляем центр граничащей сферы
        std::vector<double> center = {(minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2};

        // Вычисляем радиус, как максимальное расстояние от центра до любой вершины полигонов
        double radius = 0;
        for (const auto& component : GetComponents()) {
            auto polygon = std::dynamic_pointer_cast<Polygon>(component);
            if (polygon) {
                radius = std::max(radius, Distance(center, {polygon->x1, polygon->y1, polygon->z1}));
                radius = std::max(radius, Distance(center, {polygon->x2, polygon->y2, polygon->z2}));
                radius = std::max(radius, Distance(center, {polygon->x3, polygon->y3, polygon->z3}));
            }
        }

        return BoundingSphere(center, radius);
    }
};

#endif //CG_POLYGONOBJECT_H
