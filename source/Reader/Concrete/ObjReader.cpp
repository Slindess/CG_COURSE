#include "ObjReader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "../../Utils/Color.h"
#include "../../Texture/BaseTexture.h"
#include "../../Texture/Concrete/SimpleMountainTexture.h"
#include "../../Texture/Concrete/NoTexture.h"

std::vector<double> normalizer(const std::vector<double>& vec) {
    double length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    if (length > 0) {
        return { vec[0] / length, vec[1] / length, vec[2] / length };
    }
    return vec; // Если длина нулевая, возвращаем нулевой вектор
}

std::shared_ptr<PolygonObject> ObjReader::Read(std::string fileName)
{
    std::vector<std::vector<double>> v;
    std::vector<std::vector<double>> n;
    std::vector<std::vector<int>> colorize;
    std::vector<Polygon> polygons;
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + fileName);
    }
    
    int polygon_index = 0;
    int bad_normals = 0;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v")
        {
            std::vector<double> vertex = {0, 0, 0};
            lineStream >> vertex[0] >> vertex[1] >> vertex[2];
            v.push_back(vertex);
        } 
        else if (prefix == "colorize")
        {
            int start;
            int stop;
            int r, g, b;
            lineStream >> start >> stop >> r >> g >> b;
            std::vector<int> color = {start, stop, r, g, b};
            colorize.push_back(color);
        }
        else if (prefix == "vn")
        {
            // Чтение нормали
            std::vector<double> normal = {0, 0, 0};
            lineStream >> normal[0] >> normal[1] >> normal[2];
            n.push_back(normal);
        }
        else if (prefix == "f")
        {   
            std::vector<std::vector<double>> polygon; // Тут вершины
            std::vector<std::vector<double>> normals;  // Тут под такими же индексами нормали
            std::string vertexData;
            bool correct_polygon = true;
            int op = 0;
            for (int i = 0; i < 4; ++i) {
                if (!(lineStream >> vertexData)) {
                    break;
                    //throw std::runtime_error("Invalid face definition, not enough vertices.");
                }

                
                size_t firstSlash = vertexData.find('/');
                size_t secondSlash = vertexData.find('/', firstSlash + 1);

                if (firstSlash == std::string::npos || secondSlash == std::string::npos)
                {
                    throw std::runtime_error("Invalid face format: " + vertexData);
                }

                //std::cout << "Пихаю в stoi: {" << vertexData.substr(0, firstSlash) << "}\n";
                //std::cout << "Пихаю в stoi: {" << vertexData.substr(secondSlash + 1) << "}\n";

                auto arg1 = vertexData.substr(0, firstSlash);
                auto arg2 = vertexData.substr(secondSlash + 1);

                if (arg1.size() == 0 || arg2.size() == 0)
                {
                    correct_polygon = false;
                    break;
                }
                int vertexIndex = std::stoi(vertexData.substr(0, firstSlash)) - 1;
                int normalIndex = std::stoi(vertexData.substr(secondSlash + 1)) - 1;
            

                std::vector<double> vertex = v[vertexIndex];
                std::vector<double> normal = n[normalIndex];

                
                polygon.push_back(vertex);
                normals.push_back(normal);


            }

            if (correct_polygon)
            {
                double scale = 0.1;
                double push = 0;
                int r = 160;
                int g = 161;
                int b = 163;

                for (int i = 0; i < colorize.size(); i++)
                {
                    if (polygon_index >= colorize[i][0] && polygon_index <= colorize[i][1])
                    {
                        r = colorize[i][2];
                        g = colorize[i][3];
                        b = colorize[i][4];
                        break;
                    }
                }

                std::shared_ptr<BaseTexture> texture = std::make_shared<NoTexture>();
                
                if (!bad_normals)
                {

                    polygons.push_back(*(new Polygon(polygon[0][1] * scale, polygon[0][0]* scale, polygon[0][2]* scale + push,
                                                    polygon[1][1] * scale, polygon[1][0]* scale, polygon[1][2]* scale + push,
                                                    polygon[2][1]* scale, polygon[2][0]* scale, polygon[2][2]* scale + push,
                                                    *(new Color(r, g, b)), 
                                                    normals[0][1], normals[0][0], normals[0][2],
                                                    normals[1][1], normals[1][0], normals[1][2],
                                                    normals[2][1], normals[2][0], normals[2][2], texture)));
                    
                    if (polygons.size() > 3)
                    {
                        polygons.push_back(*(new Polygon(polygon[0][1] * scale, polygon[0][0]* scale, polygon[0][2]* scale+ push,
                                                        polygon[3][1] * scale, polygon[3][0]* scale, polygon[3][2]* scale+ push,
                                                        polygon[2][1]* scale, polygon[2][0]* scale, polygon[2][2]* scale+ push,
                                                        *(new Color(r, g, b)), 
                                                        normals[0][1], normals[0][0], normals[0][2],
                                                        normals[3][1], normals[3][0], normals[3][2],
                                                        normals[2][1], normals[2][0], normals[2][2], texture)));
                    }
                }
                else
                {

                        polygons.push_back(*(new Polygon(polygon[0][1] * scale, polygon[0][0]* scale, polygon[0][2]* scale + push,
                                                    polygon[1][1] * scale, polygon[1][0]* scale, polygon[1][2]* scale + push,
                                                    polygon[2][1]* scale, polygon[2][0]* scale, polygon[2][2]* scale + push,
                                                    *(new Color(r, g, b)), 
                                                    normals[0][1], normals[0][0], normals[0][2],
                                                    texture)));
                    if (polygon.size() > 3)
                    {
                        polygons.push_back(*(new Polygon(polygon[0][1] * scale, polygon[0][0]* scale, polygon[0][2]* scale+ push,
                                                        polygon[3][1] * scale, polygon[3][0]* scale, polygon[3][2]* scale+ push,
                                                        polygon[2][1]* scale, polygon[2][0]* scale, polygon[2][2]* scale+ push,
                                                        *(new Color(r, g, b)), 
                                                        normals[0][1], normals[0][0], normals[0][2],
                                                        texture)));
                    }
                    
                }
            }
            polygon_index++;
        }
        else if(prefix == "normals_bad")
        {
            bad_normals = 1;
        }
    }
    std::cout << polygons.size() << "\n";

    std::shared_ptr<PolygonObject> polygonObject;
    if (bad_normals)
    {
        polygonObject = std::make_shared<PolygonObject>(polygons);
    }
    else
    {
        polygonObject = std::make_shared<PolygonObject>(polygons, 543);
    }

    return polygonObject;
}