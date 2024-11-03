#include "Polygon.h"

std::vector<double> Polygon::GetNormal() const
{
    std::vector<double> v1 = { x2 - x1, y2 - y1, z2 - z1 };
    std::vector<double> v2 = { x3 - x1, y3 - y1, z3 - z1 };

    std::vector<double> normal = {
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
    };

    double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
    return normal;
}
