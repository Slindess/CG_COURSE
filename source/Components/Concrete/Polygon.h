#ifndef CG_POLYGON_H
#define CG_POLYGON_H

#include <memory>
#include <vector>
#include "../BaseComponent.h"
#include "../../Utils/Color.h"

class Polygon : public BaseComponent
{
public:
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    Color color; // TODO: Texture

    double neighbor1x, neighbor1y, neighbor1z; // Вершина смежная с 1-2
    double neighbor2x, neighbor2y, neighbor2z; // Вершина смежная с 1-3
    double neighbor3x, neighbor3y, neighbor3z;  // Вершина смежная с 2-3

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(255, 0, 0),
            neighbor1x(x1), neighbor1y(y1), neighbor1z(z1), neighbor2x(x2), neighbor2y(y2), neighbor2z(z2),
            neighbor3x(x3), neighbor3y(y3), neighbor3z(z3) {}

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, Color color) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(color),
            neighbor1x(x1), neighbor1y(y1), neighbor1z(z1), neighbor2x(x2), neighbor2y(y2), neighbor2z(z2),
            neighbor3x(x3), neighbor3y(y3), neighbor3z(z3) {} 

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            Color color, 
            double neighbor1x, double neighbor1y, double neighbor1z,
            double neighbor2x, double neighbor2y, double neighbor2z,
            double neighbor3x, double neighbor3y, double neighbor3z) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(color),
            neighbor1x(neighbor1x), neighbor1y(neighbor1y), neighbor1z(neighbor1z),
            neighbor2x(neighbor2x), neighbor2y(neighbor2y), neighbor2z(neighbor2z),
            neighbor3x(neighbor3x), neighbor3y(neighbor3y), neighbor3z(neighbor3z) {}

    std::vector<double> GetNormal() const;
    std::vector<double> GetVertex1Normal() const;  // Возвращает нормаль в V1
    std::vector<double> GetVertex3Normal() const;  // Возвращает нормаль в V2
    std::vector<double> GetVertex2Normal() const;  // Возвращает нормаль в V3

    ~Polygon() = default;
};

#endif //CG_POLYGON_H
