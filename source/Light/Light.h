//
// Created by Михаил Слиняков on 20.10.2024.
//

#ifndef CG_LIGHT_H
#define CG_LIGHT_H

class LightSource {
public:
    double direction_x;
    double direction_y;
    double direction_z;

    LightSource(double dx, double dy, double dz) : direction_x(dx), direction_y(dy), direction_z(dz)
    {}


    std::vector<double> getDirection() const
    {
        return { direction_x, direction_y, direction_z };
    }
};


#endif //CG_LIGHT_H
