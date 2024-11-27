//
// Created by Михаил Слиняков on 27.11.2024.
//

#ifndef CG_DIAMONDSQUAREMOUNTAINGENERATOR_H
#define CG_DIAMONDSQUAREMOUNTAINGENERATOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <random>
#include "../Objects/Concrete/PolygonObject.h"
#include "../Utils/Color.h"
#include "../Texture/BaseTexture.h"
#include "../Texture/Concrete/SimpleMountainTexture.h"
#include "../Texture/Concrete/SnowTexture.h"

std::vector<double> calculateNormalll(const std::vector<double>& v0, const std::vector<double>& v1, const std::vector<double>& v2) {
    double nx = (v1[1] - v0[1]) * (v2[2] - v0[2]) - (v1[2] - v0[2]) * (v2[1] - v0[1]);
    double ny = (v1[2] - v0[2]) * (v2[0] - v0[0]) - (v1[0] - v0[0]) * (v2[2] - v0[2]);
    double nz = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
    double length = -1; //std::sqrt(nx * nx + ny * ny + nz * nz);
    return {nx / length, ny / length, nz / length};
}

class DiamondSquareMountainGenerator {
public:
    DiamondSquareMountainGenerator(int size, double roughness)
            : _size(size), _roughness(roughness) {
        _grid.resize(size, std::vector<double>(size, 0.0));
        generateDiamondSquare();
    }

    std::shared_ptr<PolygonObject> generateMountain() {
        std::vector<Polygon> polygons;
        double maxHeight = 0.0;

        // Нахождение максимальной высоты
        for (const auto& row : _grid) {
            for (double height : row) {
                if (height > maxHeight) {
                    maxHeight = height;
                }
            }
        }

        // Генерация полигонов
        for (int i = 0; i < _size - 1; ++i) {
            for (int j = 0; j < _size - 1; ++j) {
                double x1 = i * _scale;
                double y1 = j * _scale;
                double x2 = (i + 1) * _scale;
                double y2 = j * _scale;
                double x3 = i * _scale;
                double y3 = (j + 1) * _scale;
                double x4 = (i + 1) * _scale;
                double y4 = (j + 1) * _scale;

                double h1 = _grid[i][j];
                double h2 = _grid[i + 1][j];
                double h3 = _grid[i][j + 1];
                double h4 = _grid[i + 1][j + 1];

                // Определение текстуры
                Color mount = {160, 161, 163};
                Color snow = {255, 255, 255};
                Color result = mount;

                std::shared_ptr<BaseTexture> texture = std::make_shared<SimpleMountainTexture>();
                std::vector<double> n1 = calculateNormalll({h3, y3, x3}, {h1, y1, x1}, {h2, y2, x2});
                std::vector<double> n2 = calculateNormall({h4, y4, x4}, {h3, y3, x3}, {h2, y2, x2});
                std::cout << h1 << " " << y1 << " " << x1 << "\n";
                std::cout << h2 << " " << y2 << " " << x2 << "\n";
                std::cout << h3 << " " << y3 << " " << x3 << "\n";
                std::cout << "Normal: " << n1[0] << " " << n1[1] << " " << n1[2] << "\n";
                polygons.emplace_back(h1, y1, x1, h2, y2, x2, h3, y3, x3, result, n1[0], n1[1], n1[2], texture);
                polygons.emplace_back(h4, y4, x4, h2, y2, x2, h3, y3, x3, result, n2[0], n2[1], n2[2], texture);
            }
        }

        return std::make_shared<PolygonObject>(polygons);
    }

private:
    int _size;
    double _roughness;
    double _scale = 1;
    std::vector<std::vector<double>> _grid;

    void initializeGrid() {
        _grid.resize(_size * 2); // Устанавливаем количество строк
        for (int i = 0; i < _size * 2; ++i) {
            _grid[i].resize(_size * 2); // Устанавливаем количество столбцов для каждой строки
        }
    }

    void initializePeaks() {
        static std::mt19937 gen(std::random_device{}());
        int peaksCount = 3; // Количество вершин
        std::uniform_int_distribution<> peakDist(0, _size - 1);

        for (int i = 0; i < peaksCount; ++i) {
            int x = peakDist(gen);
            int y = peakDist(gen);
            _grid[x][y] = randomHeight() * 4; // Высота пиков больше остальных
        }
    }

    void applyErosion() {
        std::vector<std::vector<double>> newGrid = _grid;

        for (int x = 1; x < _size - 1; ++x) {
            for (int y = 1; y < _size - 1; ++y) {
                double avg = (_grid[x - 1][y] +
                              _grid[x + 1][y] +
                              _grid[x][y - 1] +
                              _grid[x][y + 1]) / 4.0;
                newGrid[x][y] = (_grid[x][y] + avg) / 2.0; // Сглаживание
            }
        }
        _grid = newGrid;
    }

    void enhancePeaks(double factor = 1.5) {
        for (int x = 0; x < _size; ++x) {
            for (int y = 0; y < _size; ++y) {
                if (_grid[x][y] > 0.75 * _roughness) { // Только для высоких точек
                    _grid[x][y] *= factor;
                }
            }
        }
    }

    void generateDiamondSquare() {
        // Инициализация размеров сетки
        initializeGrid();
        initializePeaks();
        int step = _size - 1;
        double scale = _roughness;

        // Инициализация углов
        _grid[0][0] = randomHeight();
        _grid[0][_size - 1] = randomHeight();
        _grid[_size - 1][0] = randomHeight();
        _grid[_size - 1][_size - 1] = randomHeight();

        // Алгоритм
        while (step > 1) {
            int halfStep = step / 2;

            // Diamond step
            for (int x = 0; x < _size - 1; x += step) {
                for (int y = 0; y < _size - 1; y += step) {
                    double avg = (_grid[x][y] +
                                  _grid[x + step][y] +
                                  _grid[x][y + step] +
                                  _grid[x + step][y + step]) /
                                 4.0;
                    _grid[x + halfStep][y + halfStep] = avg + randomHeight(scale);
                }
            }

            // Square step
            for (int x = 0; x < _size; x += halfStep) {
                for (int y = (x + halfStep) % step; y < _size; y += step) {
                    double avg = 0.0;
                    int count = 0;

                    if (x >= halfStep) {
                        avg += _grid[x - halfStep][y];
                        ++count;
                    }
                    if (x + halfStep < _size) {
                        avg += _grid[x + halfStep][y];
                        ++count;
                    }
                    if (y >= halfStep) {
                        avg += _grid[x][y - halfStep];
                        ++count;
                    }
                    if (y + halfStep < _size) {
                        avg += _grid[x][y + halfStep];
                        ++count;
                    }

                    _grid[x][y] = avg / count + randomHeight(scale);
                }
            }

            step /= 2;
            scale /= 2.0;
        }
        enhancePeaks();
        applyErosion();
    }

    double randomHeight(double scale = 1.0) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dis(-scale, scale);
        return dis(gen);
    }
};


#endif //CG_DIAMONDSQUAREMOUNTAINGENERATOR_H
