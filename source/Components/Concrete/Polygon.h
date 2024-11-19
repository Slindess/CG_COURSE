#ifndef CG_POLYGON_H
#define CG_POLYGON_H

#include <memory>
#include <vector>
#include "../BaseComponent.h"
#include "../../Utils/Color.h"
#include "../../Texture/BaseTexture.h"
#include "../../Texture/Concrete/NoTexture.h"

class Polygon : public BaseComponent
{
public:
    double x1, y1, z1;
    double x2, y2, z2;
    double x3, y3, z3;

    Color color;
    std::shared_ptr<BaseTexture> texture;

    double n1x = 0, n1y = 0, n1z = 0; // Нормаль в вершине 1
    double n2x = 0, n2y = 0, n2z = 0; // Нормаль в вершине 2
    double n3x = 0, n3y = 0, n3z = 0;  // Нормаль в вершине 3

    double nx = 0, ny = 0, nz = 0;

    // Конструктор без цвета и текстуры (текстура - NoTexture по умолчанию)
    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            std::shared_ptr<BaseTexture> texture = std::make_shared<NoTexture>()) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3),
            color(255, 0, 0), texture(texture) {}

    // Конструктор с цветом (текстура - NoTexture по умолчанию)
    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            Color color, std::shared_ptr<BaseTexture> texture = std::make_shared<NoTexture>()) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3),
            color(color), texture(texture) {}

    // Конструктор с нормалями в вершинах (текстура - NoTexture по умолчанию)
    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            Color color,
            double normal1x, double normal1y, double normal1z,
            double normal2x, double normal2y, double normal2z,
            double normal3x, double normal3y, double normal3z,
            std::shared_ptr<BaseTexture> texture = std::make_shared<NoTexture>()) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3),
            color(color), texture(texture) {
        n1x = normal1x; n1y = normal1y; n1z = normal1z;
        n2x = normal2x; n2y = normal2y; n2z = normal2z;
        n3x = normal3x; n3y = normal3y; n3z = normal3z;
    }

    // Конструктор с общей нормалью (текстура - NoTexture по умолчанию)
    Polygon(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3,
            Color color,
            double normalx, double normaly, double normalz,
            std::shared_ptr<BaseTexture> texture = std::make_shared<NoTexture>()) :
            x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), x3(x3), y3(y3), z3(z3),
            color(color), texture(texture) {
        nx = normalx;
        ny = normaly;
        nz = normalz;
    }

    std::vector<double> GetNormal() const;
    std::vector<double> GetVertex1Normal() const;  // Возвращает нормаль в V1
    std::vector<double> GetVertex3Normal() const;  // Возвращает нормаль в V2
    std::vector<double> GetVertex2Normal() const;  // Возвращает нормаль в V3

    ~Polygon() = default;
};

#endif //CG_POLYGON_H
