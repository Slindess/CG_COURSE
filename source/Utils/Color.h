#ifndef CG_COLOR_H
#define CG_COLOR_H

class Color
{
public:
    double red;
    double green;
    double blue;

    Color(double r, double g, double b) : red(r), green(g), blue(b) {}

    Color(Color &color) : red(color.red), green(color.green), blue(color.blue) {}

    Color(Color&& other) noexcept : red(other.red), green(other.green), blue(other.blue) {}
};

#endif //CG_COLOR_H
