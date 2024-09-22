#ifndef CG_LINE_H
#define CG_LINE_H

#include "../BaseComponent.h"
#include "../../Utils/Color.h"

class Line : public BaseComponent
{
public:
    double x1, y1, z1;
    double x2, y2, z2;
    Color color;

    Line(double x1, double y1, double z1, double x2, double y2, double z2) :
    x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), color(0, 0, 0) {}

    Line(double x1, double y1, double z1, double x2, double y2, double z2, Color color) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), color(color) {}

};


#endif //CG_LINE_H
