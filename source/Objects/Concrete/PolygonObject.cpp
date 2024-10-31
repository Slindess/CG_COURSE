#include "PolygonObject.h"

#include "../../Components/Concrete/Polygon.h"
#include <vector>
#include <memory>
#include <iostream>
#include "../../Utils/Color.h"

PolygonObject::PolygonObject(std::initializer_list<std::initializer_list<double>> l) : _sphere(CalculateBoundingSphere())
{
    for (auto e : l)
    {
        std::vector<double> vec(e);
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color));
    }

    _sphere = CalculateBoundingSphere();
}

PolygonObject::PolygonObject(std::vector<std::initializer_list<double>>& l) : _sphere(CalculateBoundingSphere())
{
    for (const auto& e : l)
    {
        std::vector<double> vec(e);

        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2],
                                                        v1[0], v1[1], v1[2],
                                                        v2[0], v2[1], v2[2],
                                                        color));
    }
    _sphere = CalculateBoundingSphere();
}

PolygonObject::PolygonObject(std::vector<Polygon>& polygons) : _sphere(CalculateBoundingSphere())
{
    for (const auto& polygon : polygons) {
        // Создание триугольников из полигонов
        _components.push_back(std::make_shared<Polygon>(
                polygon.x1, polygon.y1, polygon.z1,
                polygon.x2, polygon.y2, polygon.z2,
                polygon.x3, polygon.y3, polygon.z3,
                polygon.color)); // Используем цвет из полигона
    }
    _sphere = CalculateBoundingSphere();
}

PolygonObject::PolygonObject(std::vector<std::vector<double>>& l) : _sphere(CalculateBoundingSphere())
{
    for (const auto& vec : l) {

        // Извлекаем координаты и цвет
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(
                v0[0], v0[1], v0[2],
                v1[0], v1[1], v1[2],
                v2[0], v2[1], v2[2],
                color));
    }
    _sphere = CalculateBoundingSphere();
}

PolygonObject::~PolygonObject()
{

}
