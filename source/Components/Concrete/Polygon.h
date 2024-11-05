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

    double n1x = 0, n1y = 0, n1z = 0; // Нормаль в вершине 1
    double n2x = 0, n2y = 0, n2z = 0; // Нормаль в вершине 2
    double n3x = 0, n3y = 0, n3z = 0;  // Нормаль в вершине 3

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(255, 0, 0) {}

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, Color color) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(color) {} 

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            Color color, 
            double normal1x, double normal1y, double normal1z,
            double normal2x, double normal2y, double normal2z,
            double normal3x, double normal3y, double normal3z) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(color)
            {
                n1x = normal1x; n1y = normal1y; n1z = normal1z;
                n2x = normal2x; n2y = normal2y; n2z = normal2z;
                n3x = normal3x; n3y = normal3y; n3z = normal3z;
            }


    std::vector<double> GetNormal() const;
    std::vector<double> GetVertex1Normal() const;  // Возвращает нормаль в V1
    std::vector<double> GetVertex3Normal() const;  // Возвращает нормаль в V2
    std::vector<double> GetVertex2Normal() const;  // Возвращает нормаль в V3

    ~Polygon() = default;
};

#endif //CG_POLYGON_H
