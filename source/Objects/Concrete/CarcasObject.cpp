#include "CarcasObject.h"
#include "../../Components/Concrete/Line.h"
#include <vector>
#include <memory>

CarcasObject::CarcasObject(std::initializer_list<std::initializer_list<double>> l)
{
    for (auto e : l)
    {
        std::vector<double> vec(e);
        _components.push_back(std::make_shared<Line>(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]));

    }
}

CarcasObject::~CarcasObject()
{

}