#include "CarcasDrawAdapter.h"

#include <memory>
#include <vector>

#include "../../Drawer/QtDrawer.h"
#include "../../Objects/Concrete/CarcasObject.h"
#include "../../Components/Concrete/Line.h"

CarcasDrawAdapter::CarcasDrawAdapter(std::shared_ptr<QtDrawer> drawer) : _drawer(drawer)
{}

void CarcasDrawAdapter::Draw(BaseObject &object, Camera &camera)
{
    std::list<std::shared_ptr<BaseComponent>> components = object.GetComponents();
    for (auto component : components)
    {
        std::shared_ptr<Line> line = std::dynamic_pointer_cast<Line>(component);
        if (line)
        {
            std::vector<double> vec1 = camera.GetPointProjection(line->x1, line->y1, line->z1);
            std::vector<double> vec2 = camera.GetPointProjection(line->x2, line->y2, line->z2);
            _drawer->drawLine(vec1[0], vec1[1], vec2[0], vec2[1]);
        }
    }
}


CarcasDrawAdapter::~CarcasDrawAdapter(){}

