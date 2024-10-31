#ifndef CG_COLOR_H
#define CG_COLOR_H

#include <QColor>
#include <algorithm>

class Color {
public:
    double r, g, b;

    Color() : r(0), g(0), b(0) {}

    // Конструктор
    Color(double red, double green, double blue) : r(red), g(green), b(blue) {}

    // Конструктор копирования
    Color(const Color& other) : r(other.r), g(other.g), b(other.b) {}

    // Оператор присваивания
    Color& operator=(const Color& other) {
        if (this != &other) {
            r = other.r;
            g = other.g;
            b = other.b;
        }
        return *this;
    }

    // Оператор умножения на интенсивность
    Color operator*(double intensity) const {
        return Color(r * intensity, g * intensity, b * intensity);
    }

    // Преобразование в QColor
    QColor toQColor() const {
        int red = std::clamp(static_cast<int>(r * 255), 0, 255);
        int green = std::clamp(static_cast<int>(g * 255), 0, 255);
        int blue = std::clamp(static_cast<int>(b * 255), 0, 255);
        return QColor(red, green, blue);
    }

    bool isBG()
    {
        return r == -1 and b == -1 and g == -1;
    }

    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }
};


#endif //CG_COLOR_H
