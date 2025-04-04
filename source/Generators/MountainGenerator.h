#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <random>
#include "../Objects/Concrete/PolygonObject.h" // Подключаем ваш класс PolygonObject
#include "../Utils/Color.h"
#include "../Texture/BaseTexture.h"
#include "../Texture/Concrete/SimpleMountainTexture.h"
#include "../Texture/Concrete/SnowTexture.h"
#include "../Texture/Concrete/SandTexture.h"
#include "../Texture/Concrete/JungleTexture.h"
#include <map>
#include <functional>

std::vector<double> calculateNormall(const std::vector<double>& v0, const std::vector<double>& v1, const std::vector<double>& v2) {
    double nx = (v1[1] - v0[1]) * (v2[2] - v0[2]) - (v1[2] - v0[2]) * (v2[1] - v0[1]);
    double ny = (v1[2] - v0[2]) * (v2[0] - v0[0]) - (v1[0] - v0[0]) * (v2[2] - v0[2]);
    double nz = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
    double length = -1; //std::sqrt(nx * nx + ny * ny + nz * nz);
    return {nx / length, ny / length, nz / length};
}

class PerlinNoiseMountainGenerator {
public:
    PerlinNoiseMountainGenerator(int gridSize, int noiseSize, double scale)
        : _gridSize(gridSize), _noiseSize(noiseSize), _scale(scale) {
        //initializeGradientGrid();
        island();

        textureFactories[1] = []() {return std::make_shared<SimpleMountainTexture>();};
        textureFactories[2] = []() {return std::make_shared<SandTexture>();};
        textureFactories[3] = []() {return std::make_shared<JungleTexture>();};
    }

    std::shared_ptr<PolygonObject> generateMountain(int type, int avgheight, int snow) {
        std::vector<Polygon> polygons;
        double rs = 3;
        std::vector<std::vector<double>> heights(_noiseSize * 10 / rs, std::vector<double>(_noiseSize * 10 /rs));

            // Генерация карты шума
        for (double i = 0; i < _noiseSize - 1; i += 0.1 * rs) {
            for (double j = 0; j < _noiseSize - 1; j += 0.1 * rs) {
                double x = i;
                double y = j;
            
                double height = (generateNoise(x, y) + 1) * avgheight; // Смещаем и масштабируем высоту
                //std::cout << i / 0.1 << " " << j / 0.1 << " " << height << "\n";
                // Сохраняем высоту в двумерном массиве
                heights[i / (0.1 * rs)][j / (0.1 * rs)] = height; // Используем i и j для индексации
            }
        }

        double maxHeight = 0.0; // Переменная для хранения максимальной высоты

        // Сначала находим максимальную высоту
        for (const auto& row : heights) {
            for (double height : row) {
                if (height > maxHeight) {
                    maxHeight = height;
                }
            }
        }

        // Перебор высот для создания полигонов
        for (int i = 11; i < heights.size() - 11; i++) {
            for (int j = 11; j < heights[i].size() - 11; j++) {
                double x1 = (double)i * 0.1 * rs;
                double y1 = (double)j * 0.1 * rs;
                double h1 = heights[i][j];           // Вершина 1
                //std::cout << i << " " << j << " " << h1 << "\n";
                double h2 = heights[i][j + 1];       // Вершина 2
                double h3 = heights[i + 1][j];       // Вершина 3
                double h4 = heights[i + 1][j + 1];   // Вершина 4

                if (h1 <= 0 || h2 <= 0 || h3 <= 0 || h4 <= 0) continue;
                double x2 = (double)i * 0.1 * rs;
                double y2 = (double)(j + 1) * 0.1 * rs;
                double x3 = (double)(i + 1) * 0.1 * rs;
                double y3 = (double)j * 0.1 * rs;
                double x4 = (double)(i + 1) * 0.1 * rs;
                double y4 = (double)(j + 1)* 0.1 * rs;

                double scaleX = 100;
                double scaleY = 100;

                double offsetX = 500;
                double offsetY = 500;
                double offsetH = avgheight * 1.3888888;

                x1 *= scaleX;
                y1 *= scaleY;
                x2 *= scaleX;
                y2 *= scaleY;
                x3 *= scaleX;
                y3 *= scaleY;
                x4 *= scaleX;
                y4 *= scaleY;
                x1 -= offsetX;
                y1 -= offsetY;
                x2 -= offsetX;
                y2 -= offsetY;
                x3 -= offsetX;
                y3 -= offsetY;
                x4 -= offsetX;
                y4 -= offsetY;
                //double x2 = x1 + 0.7;
                //double y2 = y1 + 0.7;
                Color mount = {160, 161, 163};
                //std::shared_ptr<BaseTexture> texture = std::make_shared<SimpleMountainTexture>();
                std::shared_ptr<BaseTexture> texture = textureFactories.find(type)->second();

                /*
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(1, 10);
                int random_number = distrib(gen);
                if (random_number > 10 - snow)
                {
                    texture = std::make_shared<SnowTexture>();
                }*/
                /*
                if (h1 > (9 / 10.0) * maxHeight || h3 > (9 / 10.0) * maxHeight || h3 > (9 / 10.0) * maxHeight || h4 > (9 / 10.0) * maxHeight )
                {
                    texture = std::make_shared<SnowTexture>();
                }*/
                //Color snow = {160, 161, 163};
                Color snow = {255, 255, 255};
                //if (h1 < 25) continue;
                Color result = (h1 > (9 / 10.0) * maxHeight || h3 > (9 / 10.0) * maxHeight || h3 > (9 / 10.0) * maxHeight || h4 > (9 / 10.0) * maxHeight ) ? snow : mount;
                
                
                h1 = std::max(h1, h1) - offsetH;
                h2 = std::max(h2, h2) - offsetH;
                h3 = std::max(h3, h3) - offsetH;
                h4 = std::max(h4, h4) - offsetH;

                std::vector n1 = calculateNormall({x3, y3, h3}, {x1, y1, h1}, {x2, y2, h2});
                std::vector n2 = calculateNormall({x2, y2, h2}, {x4, y4, h4}, {x3, y3, h3});
                polygons.emplace_back(h1, y1, x1, h2, y2, x2, h3, y3, x3, result, n1[0], n1[1], n1[2], texture);
                polygons.emplace_back(h4, y4, x4, h2, y2, x2, h3, y3, x3, result, n2[0], n2[1], n2[2], texture);
                //polygons.emplace_back(h1, y1, x1, h2, y1, x2, h3, y1, x1, result);
                //polygons.emplace_back(h2, y1, x2, h4, y2, x2, h3, y2, x1, result);


            }
        }
        //auto p = polygons[0];
        //std::cout << p.texture->GetUrl() << "\n\n\n\n\n";
        return std::make_shared<PolygonObject>(polygons);
    }

private:
    int _gridSize, _noiseSize;
    double _scale;
    std::vector<std::vector<std::pair<double, double>>> _gradientGrid;

    // Функция для инициализации сетки градиентов с радиальным затуханием для острова
    void initializeGradientGrid() {
        _gradientGrid.resize(_gridSize, std::vector<std::pair<double, double>>(_gridSize));

        int centerX = _gridSize / 2;
        int centerY = _gridSize / 2;
        double maxDistance = std::sqrt(centerX * centerX + centerY * centerY);

        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dis(0, 2 * M_PI);

        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                double distance = std::sqrt(std::pow(centerX - i, 2) + std::pow(centerY - j, 2));
                double influence = (maxDistance - distance) / maxDistance; // Радиальное затухание

                double angle = dis(gen);
                _gradientGrid[i][j] = {std::cos(angle) * influence, std::sin(angle) * influence};
            }
        }
        /*
        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
            
                std::cout << "Grid[" << i << "][" << j << "] = ("
                          << _gradientGrid[i][j].first << ", "
                          << _gradientGrid[i][j].second << ")\n";
            }
            std::cout << "\n";
        }
        */
    }

    void island() {
        _gradientGrid.resize(_gridSize, std::vector<std::pair<double, double>>(_gridSize));

        int centerX = _gridSize / 2;
        int centerY = _gridSize / 2;
        double maxDistance = std::sqrt(centerX * centerX + centerY * centerY);

        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dis(0, 2 * M_PI);

        for (int i = 0; i < _gridSize; ++i) {
            for (int j = 0; j < _gridSize; ++j) {
                double distance = std::sqrt(std::pow(centerX - i, 2) + std::pow(centerY - j, 2));
                double influence = (maxDistance - distance) / maxDistance;

                double angle = dis(gen);
                _gradientGrid[i][j] = {std::cos(angle) * influence, std::sin(angle) * influence};
            }
        }
    }

    void island2() {
    _gradientGrid.resize(_gridSize, std::vector<std::pair<double, double>>(_gridSize));

    int centerX = _gridSize / 2;
    int centerY = _gridSize / 2;

    // Максимальная высота пика
    double maxHeight = 3.0;
    
    for (int i = 0; i < _gridSize; ++i) {
        for (int j = 0; j < _gridSize; ++j) {
            // Вычисляем расстояние от центра
            double distance = std::sqrt(std::pow(centerX - i, 2) + std::pow(centerY - j, 2));
            
            // Высота, уменьшающаяся линейно с расстоянием
            double height = maxHeight * (1.0 - distance / (std::sqrt(2) * centerX)); // Высота уменьшается по диагонали
            
            // Ограничиваем высоту, чтобы она не была отрицательной
            /*if (height < 0) {
                height = 0;
            }*/
            
            // Присваиваем градиент (в данном случае можно задать фиксированный градиент для пиковой высоты)
            double angle = (height > 0) ? M_PI / 4 : 0; // Угол градиента
            _gradientGrid[i][j] = {std::cos(angle) * height, std::sin(angle) * height};
        }
    }
}

    double fade(double t) {
        return 6 * std::pow(t, 5) - 15 * std::pow(t, 4) + 10 * std::pow(t, 3);
    }


    double dotProduct(std::pair<double, double>& grad, double x, double y) {
        return grad.first * x + grad.second * y;
    }

    double dotProduct(const std::pair<double, double>& gradient, const std::pair<double, double>& offset) {
        return gradient.first * offset.first + gradient.second * offset.second;
    }

    double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    // Генерация шума Перлина для заданных координат с использованием интерполяции
    double generateNoise(double x, double y)
    {
        //std::cout << x << " " << y << "\n";
        // Узлы сетки вокруг точки (x, y)
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        // Градиенты в узлах сетки
        auto grad1 = _gradientGrid[y0][x0]; // Левый верхний узел
        auto grad2 = _gradientGrid[y1][x0]; // Левый нижний узел
        auto grad3 = _gradientGrid[y0][x1]; // Правый верхний узел
        auto grad4 = _gradientGrid[y1][x1]; // Правый нижний узел

        // Дробные части по x и y (смещения)
        double dx = x - x0;
        double dy = y - y0;

        // Смещения от узлов к точке (x, y)
        auto offset1 = std::make_pair(dx, dy);
        auto offset2 = std::make_pair(dx, dy - 1);
        auto offset3 = std::make_pair(dx - 1, dy);
        auto offset4 = std::make_pair(dx - 1, dy - 1);

        // Шумы от каждого узла
        double noise1 = dotProduct(grad1, offset1); // Шум от левого верхнего узла
        double noise2 = dotProduct(grad2, offset2); // Шум от левого нижнего узла
        double noise3 = dotProduct(grad3, offset3); // Шум от правого верхнего узла
        double noise4 = dotProduct(grad4, offset4); // Шум от правого нижнего узла
        //std::cout << noise1 << " " << noise2 << " " << noise3 << " " << noise4 << "\n";
        // Применение функции сглаживания
        double u = fade(dx);
        double v = fade(dy);

        // Интерполяция по оси x
        double interpX1 = lerp(noise1, noise3, u); // Между левым верхним и правым верхним
        double interpX2 = lerp(noise2, noise4, u); // Между левым нижним и правым нижним

        // Финальная интерполяция по оси y
        return lerp(interpX1, interpX2, v);
    }

    std::map<int, std::function<std::shared_ptr<BaseTexture>()>> textureFactories;
};

