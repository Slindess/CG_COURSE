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


    std::vector<double> GetNormal() const {
        // Векторы от первой вершины к другим двум
        std::vector<double> v1 = { x2 - x1, y2 - y1, z2 - z1 };
        std::vector<double> v2 = { x3 - x1, y3 - y1, z3 - z1 };

        // Нормаль = векторное произведение v1 и v2
        std::vector<double> normal = {
                v1[1] * v2[2] - v1[2] * v2[1],
                v1[2] * v2[0] - v1[0] * v2[2],
                v1[0] * v2[1] - v1[1] * v2[0]
        };

        // Нормализация
        double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        if (length != 0) {
            normal[0] /= length;
            normal[1] /= length;
            normal[2] /= length;
        }
        return normal;
    }

    ~Polygon() = default;


};

#endif //CG_POLYGON_H
