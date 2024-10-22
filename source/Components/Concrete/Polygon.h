#ifndef CG_POLYGON_H
#define CG_POLYGON_H

#include "../BaseComponent.h"
#include "../../Utils/Color.h"

class Polygon : public BaseComponent
{
public:
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    Color color; // TODO: Texture

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(255, 0, 0) {}

    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, Color color) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3), color(color) {}


    ~Polygon() = default;


};

#endif //CG_POLYGON_H
