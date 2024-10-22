#include "PolygonObject.h"

#include "../../Components/Concrete/Polygon.h"
#include <vector>
#include <memory>
#include <iostream>

PolygonObject::PolygonObject(std::initializer_list<std::initializer_list<double>> l)
{
    for (auto e : l)
    {
        std::vector<double> vec(e);
        std::vector<double> v0 = { vec[0], vec[1], vec[2] };
        std::vector<double> v1 = { vec[3], vec[4], vec[5] };
        std::vector<double> v2 = { vec[6], vec[7], vec[8] };

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]));
    }
}

PolygonObject::~PolygonObject()
{

}
