#include "Polygon.h"

#include <vector>
#include <iostream>

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

std::vector<double> alignNormal(const std::vector<double>& target, const std::vector<double>& reference) {
    double dot = target[0] * reference[0] + target[1] * reference[1] + target[2] * reference[2];
    if (dot < 0) {
        return { -target[0], -target[1], -target[2] }; // Если направлены противоположно, разворачиваем
    }
    return target;
}

std::vector<double> addThreeVectors(const std::vector<double>& v1, const std::vector<double>& v2, const std::vector<double>& v3) {
    if (v1.size() != v2.size() || v1.size() != v3.size()) {
        throw std::invalid_argument("Все векторы должны быть одинакового размера");
    }

    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i] + v3[i];
    }
    
    double length = std::sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);
    if (length != 0) {
        result[0] /= length;
        result[1] /= length;
        result[2] /= length;
    }
        
    return result;
}

std::vector<double> formNormal(std::vector<double> &v1, std::vector<double> &v2)
{
    std::vector<double> normal = {
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
    };
    /*
    double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }*/
    return normal;
}

std::vector<double> normalize(const std::vector<double>& vec) {
    double length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    if (length > 0) {
        return { vec[0] / length, vec[1] / length, vec[2] / length };
    }
    return vec; // Если длина нулевая, возвращаем нулевой вектор
}

std::vector<double> Polygon::GetVertex1Normal() const
{
    std::vector n = {n1x, n1y, n1z};
    return normalize(n);
}

std::vector<double> Polygon::GetVertex2Normal() const
{
    std::vector n = {n2x, n2y, n2z};
    return normalize(n);
}

std::vector<double> Polygon::GetVertex3Normal() const
{
    std::vector n = {n3x, n3y, n3z};
    return normalize(n);
}

/*
std::vector<double> Polygon::GetVertex1Normal() const
{
    // Векторы для формирования нормалей
    std::vector<double> v1 = { (x1 - neighbor1x), (y1 - neighbor1y), (z1 - neighbor1z) };
    std::vector<double> v2 = { (x2 - neighbor1x), (y2 - neighbor1y), (z2 - neighbor1z) };
    std::vector<double> v3 = { (x1 - neighbor2x), (y1 - neighbor2y), (z1 - neighbor2z) };
    std::vector<double> v4 = { (x3 - neighbor2x), (y3 - neighbor2y), (z3 - neighbor2z) };

    // Получение нормалей
    std::vector<double> normal1 = normalize(GetNormal());
    std::vector<double> normal2 = alignNormal(normalize(formNormal(v2, v1)), normal1);
    std::vector<double> normal3 = alignNormal(normalize(formNormal(v3, v4)), normal1);

    // Сложение и нормализация результирующей нормали
    std::vector<double> combinedNormal = normalize(addThreeVectors(normal1, normal2, normal3));
    return combinedNormal;
}

std::vector<double> Polygon::GetVertex2Normal() const
{
    // Векторы для формирования нормалей
    std::vector<double> v1 = { (x1 - neighbor1x), (y1 - neighbor1y), (z1 - neighbor1z) };
    std::vector<double> v2 = { (x2 - neighbor1x), (y2 - neighbor1y), (z2 - neighbor1z) };
    std::vector<double> v3 = { (x2 - neighbor3x), (y2 - neighbor3y), (z2 - neighbor3z) };
    std::vector<double> v4 = { (x3 - neighbor3x), (y3 - neighbor3y), (z3 - neighbor3z) };

    // Получение нормалей
    std::vector<double> normal1 = normalize(GetNormal());
    std::vector<double> normal2 = alignNormal(normalize(formNormal(v2, v1)), normal1);
    std::vector<double> normal3 = alignNormal(normalize(formNormal(v4, v3)), normal1);

    // Сложение и нормализация результирующей нормали
    std::vector<double> combinedNormal = normalize(addThreeVectors(normal1, normal2, normal3));
    return combinedNormal;
}

std::vector<double> Polygon::GetVertex3Normal() const
{
    // Векторы для формирования нормалей
    std::vector<double> v1 = { (x1 - neighbor2x), (y1 - neighbor2y), (z1 - neighbor2z) };
    std::vector<double> v2 = { (x3 - neighbor2x), (y3 - neighbor2y), (z3 - neighbor2z) };
    std::vector<double> v3 = { (x2 - neighbor3x), (y2 - neighbor3y), (z2 - neighbor3z) };
    std::vector<double> v4 = { (x3 - neighbor3x), (y3 - neighbor3y), (z3 - neighbor3z) };

    // Получение нормалей
    std::vector<double> normal1 = normalize(GetNormal());
    std::vector<double> normal2 = alignNormal(normalize(formNormal(v1, v2)),normal1);
    std::vector<double> normal3 = alignNormal(normalize(formNormal(v4, v3)), normal1);

    // Сложение и нормализация результирующей нормали
    std::vector<double> combinedNormal = normalize(addThreeVectors(normal1, normal2, normal3));
    return combinedNormal;
}*/

/*
std::vector<double> Polygon::GetVertex1Normal() const
{
    std::vector<double> v1 = { (x1 - neighbor1x), (y1 - neighbor1y), (z1 - neighbor1z) };
    std::vector<double> v2 = { x2 - neighbor1x, y2 - neighbor1y, z2 - neighbor1z };

    std::vector<double> v3 = { x1 - neighbor2x, y1 - neighbor2y, z1 - neighbor2z };
    std::vector<double> v4 = { x3 - neighbor2x, y3 - neighbor2y, z3 - neighbor2z };

    std::vector<double> normal1 = GetNormal();
    std::vector<double> normal2 = formNormal(v2, v1);
    std::vector<double> normal3 = formNormal(v3, v4);

    return addThreeVectors(normal1, normal2, normal3);
}

std::vector<double> Polygon::GetVertex3Normal() const
{
    std::vector<double> v1 = { x1 - neighbor2x, y1 - neighbor2y, z1 - neighbor2z };
    std::vector<double> v2 = { x3 - neighbor2x, y3 - neighbor2y, z3 - neighbor2z };

    std::vector<double> v3 = { x2 - neighbor3x, y2 - neighbor3y, z2 - neighbor3z };
    std::vector<double> v4 = { x3 - neighbor3x, y3 - neighbor3y, z3 - neighbor3z };

    std::vector<double> normal1 = GetNormal();
    std::vector<double> normal2 = formNormal(v1, v2);
    std::vector<double> normal3 = formNormal(v4, v3);

    return addThreeVectors(normal1, normal2, normal3);
}

std::vector<double> Polygon::GetVertex2Normal() const
{
    std::vector<double> v1 = { x1 - neighbor1x, y1 - neighbor1y, z1 - neighbor1z };
    std::vector<double> v2 = { x2 - neighbor1x, y2 - neighbor1y, z2 - neighbor1z };

    std::vector<double> v3 = { x2 - neighbor3x, y2 - neighbor3y, z2 - neighbor3z };
    std::vector<double> v4 = { x3 - neighbor3x, y3 - neighbor3y, z3 - neighbor3z };

    std::vector<double> normal1 = GetNormal();
    //std::cout << "n1: " << normal1[0] << " " << normal1[1] << " " << normal1[2] << "\n";
    std::vector<double> normal2 = formNormal(v2, v1);
    //std::cout << "n2: " << normal2[0] << " " << normal2[1] << " " << normal2[2] << "\n";
    std::vector<double> normal3 = formNormal(v4, v3);
    //std::cout << "n3: " << normal3[0] << " " << normal3[1] << " " << normal3[2] << "\n";

    return addThreeVectors(normal1, normal2, normal3);
}
*/