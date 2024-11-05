#include "PolygonObject.h"

#include "../../Components/Concrete/Polygon.h"
#include <vector>
#include <memory>
#include <iostream>
#include "../../Utils/Color.h"

bool areVerticesEqual(const std::vector<double>& v, const std::vector<double>& n) {
    double epsilon = 1e-6;
    return std::fabs(v[0] - n[0]) < epsilon &&
           std::fabs(v[1] - n[1]) < epsilon &&
           std::fabs(v[2] - n[2]) < epsilon;
}

std::vector<double> formNormall(const std::vector<double> &v0, const std::vector<double> &v1, const std::vector<double> &v2)
{
    std::vector<double> vec1 = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
    std::vector<double> vec2 = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};

    std::vector<double> normal = {
            vec1[1] * vec2[2] - vec1[2] * vec2[1],
            vec1[2] * vec2[0] - vec1[0] * vec2[2],
            vec1[0] * vec2[1] - vec1[1] * vec2[0]
    };
    
    double length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
    return normal;
}


std::vector<double> alignNormall(const std::vector<double>& target, const std::vector<double>& reference) {
    double dot = target[0] * reference[0] + target[1] * reference[1] + target[2] * reference[2];
    if (dot < 0) {
        return { -target[0], -target[1], -target[2] }; // Если направлены противоположно, разворачиваем
    }
    return target;
}

PolygonObject::PolygonObject(std::initializer_list<std::initializer_list<double>> l) : _sphere(CalculateBoundingSphere())
{
    for (auto e : l)
    {
        // Пусть тут v - вершины текущие
        std::vector<double> vec(e);
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        
        Color color(vec[9], vec[10], vec[11]);

        double nx = vec[13]; double ny = vec[12]; double nz = vec[14];

        // ДАЛЕЕ ИДЕЯ ТАКАЯ: УСРЕДНЯТЬ НОРМАЛИ ПОСТЕПЕННО, ПОЭТОМУ ВРЕМЕННАЯ НОРМАЛЬ ЕСТЬ И ИХ СЧЕТЧИК
        std::vector<double> normal0_tmp = formNormall(v0, v1, v2); 
        normal0_tmp = alignNormall(normal0_tmp, v0);
        std::cout << normal0_tmp[0] << " " << normal0_tmp[1] << " " << normal0_tmp[2] << "\n";
        
        double normal0_count = 1;

        std::vector<double> normal1_tmp = formNormall(v0, v1, v2); 
        double normal1_count = 1;

        std::vector<double> normal2_tmp = formNormall(v0, v1, v2); 
        double normal2_count = 1;
        
        // А тут U это вершины других полигонов
        for (auto u : l)
        {
            std::vector<double> uvec(u);
            std::vector<double> u0 = { uvec[1], uvec[0], uvec[2] };
            std::vector<double> u1 = { uvec[4], uvec[3], uvec[5] };
            std::vector<double> u2 = { uvec[7], uvec[6], uvec[8] };

            double u_nx = vec[13]; double u_ny = vec[12]; double u_nz = vec[14];

            int eq_count = 0; // Чтоб потом проверить, что вершины идентичны
            double a1 = 0, a2 = 0, a3 = 0;
            std::vector<double> n0_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 1
            std::vector<double> n1_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 2
            std::vector<double> n2_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 3
            if (areVerticesEqual(v0, u0) || areVerticesEqual(v0, u1) || areVerticesEqual(v0, u2)) 
            {
                n0_tmp = formNormall(u0, u1, u2);
                if (normal0_count >= 1)
                    n0_tmp = alignNormall(n0_tmp, {u_nx, u_ny, u_nz});
                eq_count++;
                a1 = 1.0;
            }

            if (areVerticesEqual(v1, u0) || areVerticesEqual(v1, u1) || areVerticesEqual(v1, u2)) 
            {
                n1_tmp = formNormall(u0, u1, u2);
                if (normal0_count >= 1)
                    n1_tmp = alignNormall(n1_tmp, {u_nx, u_ny, u_nz});
                eq_count++;
                a2 = 1.0;
            }

            if (areVerticesEqual(v2, u0) || areVerticesEqual(v2, u1) || areVerticesEqual(v2, u2)) 
            {
                n2_tmp = formNormall(u0, u1, u2);
                if (normal2_count >= 1)
                    n2_tmp = alignNormall(n2_tmp, {u_nx, u_ny, u_nz});
                eq_count++;
                a3 = 1.0;
            }

            if (eq_count == 3) continue; // Та же самая вершина

            if (a1 > 0) std::cout << n0_tmp[0] << " " << n0_tmp[1] << " " << n0_tmp[2] << "A\n";
            normal0_tmp[0] += n0_tmp[0];
            normal0_tmp[1] += n0_tmp[1];
            normal0_tmp[2] += n0_tmp[2];
            normal0_count += a1;

            normal1_tmp[0] += n1_tmp[0];
            normal1_tmp[1] += n1_tmp[1];
            normal1_tmp[2] += n1_tmp[2];
            normal1_count += a2;

            normal2_tmp[0] += n2_tmp[0];
            normal2_tmp[1] += n2_tmp[1];
            normal2_tmp[2] += n2_tmp[2];
            normal2_count += a3;
        }
        
        
        std::vector<double> normal0 = {normal0_tmp[0] / normal0_count, normal0_tmp[1] / normal0_count, normal0_tmp[2] / normal0_count}; 

        std::vector<double> normal1 = {normal1_tmp[0] / normal1_count, normal1_tmp[1] / normal1_count, normal1_tmp[2] / normal1_count}; 

        std::vector<double> normal2 = {normal2_tmp[0] / normal2_count, normal2_tmp[1] / normal2_count, normal2_tmp[2] / normal2_count}; 
        
        std::cout << v0[0] << " " << v0[1] << " " << v0[2] << "\n";
        std::cout << normal0_count << " " << normal0[0] << " " << normal0[1] << " " << normal0[2] << "j\n\n";
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color,
                                                    normal0[0], normal0[1], normal0[2],
                                                    normal1[0], normal1[1], normal1[2],
                                                    normal2[0], normal2[1], normal2[2]));
    }

    _sphere = CalculateBoundingSphere();
}


PolygonObject::PolygonObject(std::vector<std::initializer_list<double>>& l) : _sphere(CalculateBoundingSphere())
{
    for (const auto& e : l)
    {
        std::vector<double> vec(e);

        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        // ДАЛЕЕ ИДЕЯ ТАКАЯ: УСРЕДНЯТЬ НОРМАЛИ ПОСТЕПЕННО, ПОЭТОМУ ВРЕМЕННАЯ НОРМАЛЬ ЕСТЬ И ИХ СЧЕТЧИК
        std::vector<double> normal0_tmp = {0, 0, 0}; 
        double normal0_count;

        std::vector<double> normal1_tmp = {0, 0, 0}; 
        double normal1_count;

        std::vector<double> normal2_tmp = {0, 0, 0}; 
        double normal2_count;
        
        // А тут U это вершины других полигонов
        for (auto u : l)
        {
            std::vector<double> uvec(u);
            std::vector<double> u0 = { uvec[1], uvec[0], uvec[2] };
            std::vector<double> u1 = { uvec[4], uvec[3], uvec[5] };
            std::vector<double> u2 = { uvec[7], uvec[6], uvec[8] };

            int eq_count = 0; // Чтоб потом проверить, что вершины идентичны
            std::vector<double> n0_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 1
            std::vector<double> n1_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 2
            std::vector<double> n2_tmp = {0, 0, 0}; // Нормаль к грани в которой вершина 3
            double a1 = 0, a2 = 0, a3 = 0;
            if (areVerticesEqual(v0, u0) || areVerticesEqual(v0, u1) || areVerticesEqual(v0, u2)) 
            {
                n0_tmp = formNormall(u0, u1, u2);
                if (normal0_count >= 1)
                    n0_tmp = alignNormall(n0_tmp, v0);
                eq_count++;
                a1 = 1.0;
            }

            if (areVerticesEqual(v1, u0) || areVerticesEqual(v1, u1) || areVerticesEqual(v1, u2)) 
            {
                n1_tmp = formNormall(u0, u1, u2);
                if (normal1_count >= 1)
                    n1_tmp = alignNormall(n1_tmp, v1);
                eq_count++;
                a2 = 1.0;
            }

            if (areVerticesEqual(v2, u0) || areVerticesEqual(v2, u1) || areVerticesEqual(v2, u2)) 
            {
                n2_tmp = formNormall(u0, u1, u2);
                if (normal2_count >= 1)
                    n2_tmp = alignNormall(n2_tmp, v2);
                eq_count++;
                a3 = 1.0;
            }

            if (eq_count == 3) continue; // Та же самая вершина

            normal0_tmp[0] += n0_tmp[0];
            normal0_tmp[1] += n0_tmp[1];
            normal0_tmp[2] += n0_tmp[2];
            normal0_count += a1;

            normal1_tmp[0] += n1_tmp[0];
            normal1_tmp[1] += n1_tmp[1];
            normal1_tmp[2] += n1_tmp[2];
            normal1_count += a2;

            normal2_tmp[0] += n2_tmp[0];
            normal2_tmp[1] += n2_tmp[1];
            normal2_tmp[2] += n2_tmp[2];
            normal2_count += a3;
        }
        
        std::vector<double> normal0 = {normal0_tmp[0] / normal0_count, normal0_tmp[1] / normal0_count, normal0_tmp[2] / normal0_count}; 

        std::vector<double> normal1 = {normal1_tmp[0] / normal1_count, normal1_tmp[1] / normal1_count, normal1_tmp[2] / normal1_count}; 

        std::vector<double> normal2 = {normal2_tmp[0] / normal2_count, normal2_tmp[1] / normal2_count, normal2_tmp[2] / normal2_count}; 
        
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color,
                                                    normal0[0], normal0[1], normal0[2],
                                                    normal0[0], normal0[1], normal0[2],
                                                    normal0[0], normal0[1], normal0[2]));
    }
    _sphere = CalculateBoundingSphere();
}


PolygonObject::PolygonObject(std::vector<Polygon>& polygons) : _sphere(CalculateBoundingSphere())
{
    for (const auto& polygon : polygons) {
        // Временные нормали и счетчики для каждой вершины
        std::vector<double> normal0_tmp = {0, 0, 0}; 
        double normal0_count = 0;
        
        std::vector<double> normal1_tmp = {0, 0, 0}; 
        double normal1_count = 0;
        
        std::vector<double> normal2_tmp = {0, 0, 0}; 
        double normal2_count = 0;

        // Поиск соседей для усреднения нормалей
        for (const auto& otherPolygon : polygons) {
            if (&polygon != &otherPolygon) {
                int eq_count = 0;  // счётчик совпадений

                // Временные нормали для соседнего полигона
                std::vector<double> n0_tmp = {0, 0, 0};
                std::vector<double> n1_tmp = {0, 0, 0};
                std::vector<double> n2_tmp = {0, 0, 0};
                double a1 = 0, a2 = 0, a3 = 0;
                if (areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n0_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal0_count >= 1)
                        n0_tmp = alignNormall(n0_tmp, {polygon.x1, polygon.y1, polygon.z1});
                    eq_count++;
                    a1 = 1.0;
                }
                
                if (areVerticesEqual({polygon.x2, polygon.y2, polygon.z2}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x2, polygon.y2, polygon.z2}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x2, polygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n1_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal1_count >= 1)
                        n1_tmp = alignNormall(n1_tmp, {polygon.x2, polygon.y2, otherPolygon.z2});
                    eq_count++;
                    a2 = 1.0;
                }
                
                if (areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n2_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal2_count >= 1)
                        n2_tmp = alignNormall(n2_tmp, {polygon.x3, polygon.y3, polygon.z3});
                    eq_count++;
                    a3 = 1.0;
                }

                if (eq_count == 3) continue;  // Пропуск совпадающего полигона

                // Увеличиваем временные нормали
                normal0_tmp[0] += n0_tmp[0];
                normal0_tmp[1] += n0_tmp[1];
                normal0_tmp[2] += n0_tmp[2];
                normal0_count += a1;

                normal1_tmp[0] += n1_tmp[0];
                normal1_tmp[1] += n1_tmp[1];
                normal1_tmp[2] += n1_tmp[2];
                normal1_count += a2;

                normal2_tmp[0] += n2_tmp[0];
                normal2_tmp[1] += n2_tmp[1];
                normal2_tmp[2] += n2_tmp[2];
                normal2_count += a3;
            }
        }

        // Усреднение нормалей
        std::vector<double> normal0 = {normal0_tmp[0] / normal0_count, normal0_tmp[1] / normal0_count, normal0_tmp[2] / normal0_count}; 
        std::vector<double> normal1 = {normal1_tmp[0] / normal1_count, normal1_tmp[1] / normal1_count, normal1_tmp[2] / normal1_count}; 
        std::vector<double> normal2 = {normal2_tmp[0] / normal2_count, normal2_tmp[1] / normal2_count, normal2_tmp[2] / normal2_count}; 

        // Добавляем полигон с рассчитанными нормалями
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color,
            normal0[0], normal0[1], normal0[2],
            normal1[0], normal1[1], normal1[2],
            normal2[0], normal2[1], normal2[2]
        ));
    }

    _sphere = CalculateBoundingSphere();
}


PolygonObject::PolygonObject(std::vector<std::vector<double>>& l) : _sphere(CalculateBoundingSphere())
{
    std::vector<Polygon> tempPolygons;

    // Создаем временные полигоны
    for (const auto& vec : l) {
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        tempPolygons.emplace_back(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color);
    }

    for (const auto& polygon : tempPolygons) {
        std::vector<double> normal0_tmp = {0, 0, 0}; 
        double normal0_count = 0;
        
        std::vector<double> normal1_tmp = {0, 0, 0}; 
        double normal1_count = 0;
        
        std::vector<double> normal2_tmp = {0, 0, 0}; 
        double normal2_count = 0;

        for (const auto& otherPolygon : tempPolygons) {
            if (&polygon != &otherPolygon) {
                int eq_count = 0;

                std::vector<double> n0_tmp = {0, 0, 0};
                std::vector<double> n1_tmp = {0, 0, 0};
                std::vector<double> n2_tmp = {0, 0, 0};
                double a1 = 0, a2 = 0, a3 = 0;
                if (areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x1, polygon.y1, polygon.z1}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n0_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal0_count >= 1)
                        n0_tmp = alignNormall(n0_tmp, {polygon.x1, polygon.y1, polygon.z1});
                    eq_count++;
                    a1 = 1.0;
                }
                
                if (areVerticesEqual({polygon.x2, polygon.y2, polygon.z2}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x2, polygon.y2, polygon.z2}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x2, polygon.y2, polygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n1_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal1_count >= 1)
                        n1_tmp = alignNormall(n1_tmp, {polygon.x2, polygon.y2, polygon.z2});
                    eq_count++;
                    a2 = 1.0;
                }
                if (areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}) ||
                    areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}) ||
                    areVerticesEqual({polygon.x3, polygon.y3, polygon.z3}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3})) 
                {
                    n2_tmp = formNormall({otherPolygon.x1, otherPolygon.y1, otherPolygon.z1}, {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2}, {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3});
                    if (normal2_count >= 1)
                        n2_tmp = alignNormall(n2_tmp, {polygon.x3, polygon.y3, polygon.z3});
                    eq_count++;
                    a3 = 1.0;
                }

                // Пропускаем полигон, совпадающий по всем вершинам
                if (eq_count == 3) continue;

                // Увеличиваем временные нормали для усреднения
                normal0_tmp[0] += n0_tmp[0];
                normal0_tmp[1] += n0_tmp[1];
                normal0_tmp[2] += n0_tmp[2];
                normal0_count += a1;

                normal1_tmp[0] += n1_tmp[0];
                normal1_tmp[1] += n1_tmp[1];
                normal1_tmp[2] += n1_tmp[2];
                normal1_count += a2;

                normal2_tmp[0] += n2_tmp[0];
                normal2_tmp[1] += n2_tmp[1];
                normal2_tmp[2] += n2_tmp[2];
                normal2_count += a3;
            }
        }

        // Усреднение временных нормалей для получения финальных нормалей вершин
        std::vector<double> normal0 = {normal0_tmp[0] / normal0_count, normal0_tmp[1] / normal0_count, normal0_tmp[2] / normal0_count}; 
        std::vector<double> normal1 = {normal1_tmp[0] / normal1_count, normal1_tmp[1] / normal1_count, normal1_tmp[2] / normal1_count}; 
        std::vector<double> normal2 = {normal2_tmp[0] / normal2_count, normal2_tmp[1] / normal2_count, normal2_tmp[2] / normal2_count}; 

        // Добавляем готовый полигон в компонентный список
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color,
            normal0[0], normal0[1], normal0[2],
            normal1[0], normal1[1], normal1[2],
            normal2[0], normal2[1], normal2[2]
        ));
    }

    // Завершаем создание объекта
    _sphere = CalculateBoundingSphere();
}


PolygonObject::~PolygonObject()
{

}
