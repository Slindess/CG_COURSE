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

bool isNeighbor(const std::vector<double>& v0, const std::vector<double>& v1, const std::vector<double>& v2, 
                const std::vector<double>& n0, const std::vector<double>& n1, const std::vector<double>& n2,
                int &vertexCount) 
{
    int sharedVertices = 0;
    vertexCount = 0;  // Да, это костыль. (1+2)=3 - смежна с 12, (1+3)=5 - смежна с 13...

    // Проверяем совпадения каждой вершины
    if (areVerticesEqual(v0, n0) || areVerticesEqual(v0, n1) || areVerticesEqual(v0, n2)) {++sharedVertices; vertexCount += 1;}
    if (areVerticesEqual(v1, n0) || areVerticesEqual(v1, n1) || areVerticesEqual(v1, n2)) {++sharedVertices; vertexCount += 2;}
    if (areVerticesEqual(v2, n0) || areVerticesEqual(v2, n1) || areVerticesEqual(v2, n2)) {++sharedVertices; vertexCount += 3;}

    // Проверка, если это тот же самый полигон (все три вершины совпадают)
    if (sharedVertices == 3) return false;

    // Если есть ровно две совпадающие вершины, полигоны считаются соседями
    return sharedVertices == 2;
}

std::optional<std::vector<double>> getNonMatchingVertex(const std::vector<double>& v0, const std::vector<double>& v1, const std::vector<double>& v2,
                                                        const std::vector<double>& n0, const std::vector<double>& n1, const std::vector<double>& n2) 
{
    // Проверяем каждую вершину второго треугольника, если она не совпадает ни с одной вершиной первого
    if (!areVerticesEqual(n0, v0) && !areVerticesEqual(n0, v1) && !areVerticesEqual(n0, v2)) {
        return n0; // Возвращаем несовпадающую вершину
    }
    if (!areVerticesEqual(n1, v0) && !areVerticesEqual(n1, v1) && !areVerticesEqual(n1, v2)) {
        return n1;
    }
    if (!areVerticesEqual(n2, v0) && !areVerticesEqual(n2, v1) && !areVerticesEqual(n2, v2)) {
        return n2;
    }

    // Если все вершины совпадают, возвращаем пустое значение
    return std::nullopt;
}

PolygonObject::PolygonObject(std::initializer_list<std::initializer_list<double>> l) : _sphere(CalculateBoundingSphere())
{
    for (auto e : l)
    {
        std::vector<double> vec(e);
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        std::vector<double> neighbors; // ДВА СОСЕДА - 9 элементов (по 3 координаты у каждой)
        std::vector<int> neighborsOrder;
        // Поиск соседей
        for (auto n : l)
        {
            std::vector<double> nec(n);
            std::vector<double> n0 = { nec[1], nec[0], nec[2] };
            std::vector<double> n1 = { nec[4], nec[3], nec[5] };
            std::vector<double> n2 = { nec[7], nec[6], nec[8] };

            int vertexCount = 0;
            if (isNeighbor(v0, v1, v2, n0, n1, n2, vertexCount))
            {
                auto nonMatchingVertex = getNonMatchingVertex(v0, v1, v2, n0, n1, n2);
                if (nonMatchingVertex.has_value()) {
                    neighbors.insert(neighbors.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                    neighborsOrder.push_back(vertexCount);
                }
            }
        }
        // Вот тут провести сортировку соседей, чтоб сначала был у кого neighborsOrder самый меньший
        // Создаем пары для сортировки
        std::vector<std::pair<int, std::vector<double>> > pairedNeighbors;
        for (size_t i = 0; i < neighbors.size() / 3; ++i) {
            pairedNeighbors.emplace_back(neighborsOrder[i], std::vector<double>(neighbors.begin() + i * 3, neighbors.begin() + (i + 1) * 3));
        }

        // Сортируем по порядковым номерам соседей
        std::sort(pairedNeighbors.begin(), pairedNeighbors.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        // Обновляем neighbors в правильном порядке
        neighbors.clear();
        for (const auto& pair : pairedNeighbors) {
            neighbors.insert(neighbors.end(), pair.second.begin(), pair.second.end());
        }

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color,
                                                        neighbors[0], neighbors[1], neighbors[2],
                                                        neighbors[3], neighbors[4], neighbors[5],
                                                        neighbors[6], neighbors[7], neighbors[8]));
    }

    _sphere = CalculateBoundingSphere();
}

/*
PolygonObject::PolygonObject(std::vector<std::initializer_list<double>>& l) : _sphere(CalculateBoundingSphere())
{
    for (const auto& e : l)
    {
        std::vector<double> vec(e);

        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        std::vector<double> neighbors; // ДВА СОСЕДА - 9 элементов (по 3 координаты у каждой)
        // Поиск соседей
        for (auto n : l)
        {
            std::vector<double> nec(n);
            std::vector<double> n0 = { nec[1], nec[0], nec[2] };
            std::vector<double> n1 = { nec[4], nec[3], nec[5] };
            std::vector<double> n2 = { nec[7], nec[6], nec[8] };

            if (isNeighbor(v0, v1, v2, n0, n1, n2))
            {
                auto nonMatchingVertex = getNonMatchingVertex(v0, v1, v2, n0, n1, n2);
                if (nonMatchingVertex.has_value()) {
                    neighbors.insert(neighbors.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                }
            }
        }

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2],
                                                        v1[0], v1[1], v1[2],
                                                        v2[0], v2[1], v2[2],
                                                        color, neighbors[0], neighbors[1], neighbors[2],
                                                        neighbors[3], neighbors[4], neighbors[5],
                                                        neighbors[6], neighbors[7], neighbors[8]));


    }
    _sphere = CalculateBoundingSphere();
}
*/
PolygonObject::PolygonObject(std::vector<std::initializer_list<double>>& l) : _sphere(CalculateBoundingSphere())
{
    for (const auto& e : l)
    {
        std::vector<double> vec(e);

        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        std::vector<double> neighbors;
        std::vector<int> neighborsOrder;

        // Поиск соседей
        for (const auto& n : l)
        {
            std::vector<double> nec(n);
            std::vector<double> n0 = { nec[1], nec[0], nec[2] };
            std::vector<double> n1 = { nec[4], nec[3], nec[5] };
            std::vector<double> n2 = { nec[7], nec[6], nec[8] };

            int vertexCount = 0;
            if (isNeighbor(v0, v1, v2, n0, n1, n2, vertexCount))
            {
                auto nonMatchingVertex = getNonMatchingVertex(v0, v1, v2, n0, n1, n2);
                if (nonMatchingVertex.has_value()) {
                    neighbors.insert(neighbors.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                    neighborsOrder.push_back(vertexCount);
                }
            }
        }

        // Сортируем соседи
        std::vector<std::pair<int, std::vector<double>>> pairedNeighbors;
        for (size_t i = 0; i < neighbors.size() / 3; ++i) {
            pairedNeighbors.emplace_back(neighborsOrder[i], std::vector<double>(neighbors.begin() + i * 3, neighbors.begin() + (i + 1) * 3));
        }
        std::sort(pairedNeighbors.begin(), pairedNeighbors.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        neighbors.clear();
        for (const auto& pair : pairedNeighbors) {
            neighbors.insert(neighbors.end(), pair.second.begin(), pair.second.end());
        }

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(v0[0], v0[1], v0[2], 
                                                        v1[0], v1[1], v1[2], 
                                                        v2[0], v2[1], v2[2], 
                                                        color, neighbors[0], neighbors[1], neighbors[2], 
                                                        neighbors[3], neighbors[4], neighbors[5], 
                                                        neighbors[6], neighbors[7], neighbors[8]));
    }
    _sphere = CalculateBoundingSphere();
}

/*
PolygonObject::PolygonObject(std::vector<Polygon>& polygons) : _sphere(CalculateBoundingSphere())
{
    for (const auto& polygon : polygons) {
        std::vector<double> neighbours;

        // Поиск соседей
        for (const auto& otherPolygon : polygons) {
            if (&polygon != &otherPolygon && isNeighbor(
                {polygon.x1, polygon.y1, polygon.z1},
                {polygon.x2, polygon.y2, polygon.z2},
                {polygon.x3, polygon.y3, polygon.z3},
                {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
            )) {
                // Найти несовпадающую вершину и добавить ее в соседей
                auto nonMatchingVertex = getNonMatchingVertex(
                    {polygon.x1, polygon.y1, polygon.z1},
                    {polygon.x2, polygon.y2, polygon.z2},
                    {polygon.x3, polygon.y3, polygon.z3},
                    {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                    {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                    {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
                );

                if (nonMatchingVertex.has_value()) {
                    neighbours.insert(neighbours.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                }
            }
        }

        // Используем первую несовпадающую вершину как соседа
        double nx = neighbours.size() >= 3 ? neighbours[0] : polygon.x1;
        double ny = neighbours.size() >= 3 ? neighbours[1] : polygon.y1;
        double nz = neighbours.size() >= 3 ? neighbours[2] : polygon.z1;

        // Добавляем треугольник в компоненты, передавая координаты соседей
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color, nx, ny, nz, nx, ny, nz, nx, ny, nz // Замените на реальных соседей
        ));
    }
    _sphere = CalculateBoundingSphere();
}
*/
PolygonObject::PolygonObject(std::vector<Polygon>& polygons) : _sphere(CalculateBoundingSphere())
{
    for (const auto& polygon : polygons) {
        std::vector<double> neighbours;
        std::vector<int> neighborsOrder;

        // Поиск соседей
        for (const auto& otherPolygon : polygons) {
            int vertexCount = 0; // Объявляем переменную vertexCount
            if (&polygon != &otherPolygon && isNeighbor(
                {polygon.x1, polygon.y1, polygon.z1},
                {polygon.x2, polygon.y2, polygon.z2},
                {polygon.x3, polygon.y3, polygon.z3},
                {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3},
                vertexCount // Передаем vertexCount
            )) {
                auto nonMatchingVertex = getNonMatchingVertex(
                    {polygon.x1, polygon.y1, polygon.z1},
                    {polygon.x2, polygon.y2, polygon.z2},
                    {polygon.x3, polygon.y3, polygon.z3},
                    {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                    {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                    {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
                );

                if (nonMatchingVertex.has_value()) {
                    neighbours.insert(neighbours.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                    neighborsOrder.push_back(vertexCount); // Используйте vertexCount для сортировки
                }
            }
        }

        // Сортируем соседи
        std::vector<std::pair<int, std::vector<double>>> pairedNeighbors;
        for (size_t i = 0; i < neighbours.size() / 3; ++i) {
            pairedNeighbors.emplace_back(neighborsOrder[i], std::vector<double>(neighbours.begin() + i * 3, neighbours.begin() + (i + 1) * 3));
        }
        std::sort(pairedNeighbors.begin(), pairedNeighbors.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        neighbours.clear();
        for (const auto& pair : pairedNeighbors) {
            neighbours.insert(neighbours.end(), pair.second.begin(), pair.second.end());
        }

        // Используем первую несовпадающую вершину как соседа
        double nx = neighbours.size() >= 3 ? neighbours[0] : polygon.x1;
        double ny = neighbours.size() >= 3 ? neighbours[1] : polygon.y1;
        double nz = neighbours.size() >= 3 ? neighbours[2] : polygon.z1;

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color, nx, ny, nz, nx, ny, nz, nx, ny, nz // Замените на реальных соседей
        ));
    }
    _sphere = CalculateBoundingSphere();
}

/*
PolygonObject::PolygonObject(std::vector<std::vector<double>>& l) : _sphere(CalculateBoundingSphere())
{
    std::vector<Polygon> tempPolygons; // Временное хранилище для полигонов, чтобы искать соседей

    // Первое проход для создания полигонов и их хранения
    for (const auto& vec : l) {
        // Извлекаем координаты и цвет
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        // Сохраним полигон во временное хранилище
        tempPolygons.emplace_back(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color);
    }

    // Второе проход для добавления полигонов с соседями
    for (const auto& polygon : tempPolygons) {
        std::vector<double> neighbours;

        // Поиск соседей
        for (const auto& otherPolygon : tempPolygons) {
            if (&polygon != &otherPolygon && isNeighbor(
                {polygon.x1, polygon.y1, polygon.z1},
                {polygon.x2, polygon.y2, polygon.z2},
                {polygon.x3, polygon.y3, polygon.z3},
                {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
            )) {
                // Найти несовпадающую вершину и добавить ее в соседей
                auto nonMatchingVertex = getNonMatchingVertex(
                    {polygon.x1, polygon.y1, polygon.z1},
                    {polygon.x2, polygon.y2, polygon.z2},
                    {polygon.x3, polygon.y3, polygon.z3},
                    {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                    {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                    {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
                );

                if (nonMatchingVertex.has_value()) {
                    neighbours.insert(neighbours.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                }
            }
        }

        // Используем первую несовпадающую вершину как соседа
        double nx = neighbours.size() >= 3 ? neighbours[0] : polygon.x1;
        double ny = neighbours.size() >= 3 ? neighbours[1] : polygon.y1;
        double nz = neighbours.size() >= 3 ? neighbours[2] : polygon.z1;

        // Добавляем треугольник в компоненты, передавая координаты соседей
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color, nx, ny, nz, nx, ny, nz, nx, ny, nz // Замените на реальных соседей
        ));
    }

    _sphere = CalculateBoundingSphere();
}
*/
PolygonObject::PolygonObject(std::vector<std::vector<double>>& l) : _sphere(CalculateBoundingSphere())
{
    std::vector<Polygon> tempPolygons;

    // Первое проход для создания полигонов и их хранения
    for (const auto& vec : l) {
        std::vector<double> v0 = { vec[1], vec[0], vec[2] };
        std::vector<double> v1 = { vec[4], vec[3], vec[5] };
        std::vector<double> v2 = { vec[7], vec[6], vec[8] };
        Color color(vec[9], vec[10], vec[11]);

        tempPolygons.emplace_back(v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], color);
    }

    // Второе проход для добавления полигонов с соседями
    for (const auto& polygon : tempPolygons) {
        std::vector<double> neighbours;
        std::vector<int> neighborsOrder;

        for (const auto& otherPolygon : tempPolygons) {
            int vertexCount = 0; // Объявляем переменную vertexCount
            if (&polygon != &otherPolygon && isNeighbor(
                {polygon.x1, polygon.y1, polygon.z1},
                {polygon.x2, polygon.y2, polygon.z2},
                {polygon.x3, polygon.y3, polygon.z3},
                {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3},
                vertexCount // Передаем vertexCount
            )) {
                auto nonMatchingVertex = getNonMatchingVertex(
                    {polygon.x1, polygon.y1, polygon.z1},
                    {polygon.x2, polygon.y2, polygon.z2},
                    {polygon.x3, polygon.y3, polygon.z3},
                    {otherPolygon.x1, otherPolygon.y1, otherPolygon.z1},
                    {otherPolygon.x2, otherPolygon.y2, otherPolygon.z2},
                    {otherPolygon.x3, otherPolygon.y3, otherPolygon.z3}
                );

                if (nonMatchingVertex.has_value()) {
                    neighbours.insert(neighbours.end(), nonMatchingVertex->begin(), nonMatchingVertex->end());
                    neighborsOrder.push_back(vertexCount); // Используйте vertexCount для сортировки
                }
            }
        }

        // Сортируем соседи
        std::vector<std::pair<int, std::vector<double>>> pairedNeighbors;
        for (size_t i = 0; i < neighbours.size() / 3; ++i) {
            pairedNeighbors.emplace_back(neighborsOrder[i], std::vector<double>(neighbours.begin() + i * 3, neighbours.begin() + (i + 1) * 3));
        }
        std::sort(pairedNeighbors.begin(), pairedNeighbors.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        neighbours.clear();
        for (const auto& pair : pairedNeighbors) {
            neighbours.insert(neighbours.end(), pair.second.begin(), pair.second.end());
        }

        // Используем первую несовпадающую вершину как соседа
        double nx = neighbours.size() >= 3 ? neighbours[0] : polygon.x1;
        double ny = neighbours.size() >= 3 ? neighbours[1] : polygon.y1;
        double nz = neighbours.size() >= 3 ? neighbours[2] : polygon.z1;

        // Добавляем треугольник в компоненты
        _components.push_back(std::make_shared<Polygon>(
            polygon.x1, polygon.y1, polygon.z1,
            polygon.x2, polygon.y2, polygon.z2,
            polygon.x3, polygon.y3, polygon.z3,
            polygon.color, nx, ny, nz, nx, ny, nz, nx, ny, nz // Замените на реальных соседей
        ));
    }
    _sphere = CalculateBoundingSphere();
}


PolygonObject::~PolygonObject()
{

}
