#include "Manager.h"

#include <memory>

#include "../Scene/Scene.h"
#include "../Objects/Concrete/CarcasObject.h"
#include "../Objects/Concrete/PolygonObject.h"
#include "../DrawAdapter/Concrete/CarcasDrawAdapter.h"
#include "../DrawAdapter/Concrete/PolygonDrawAdapter.h"
#include "../Generators/MountainGenerator.h"
#include "../Generators/DiamondSquareMountainGenerator.h"
#include "../Reader/Concrete/ObjReader.h"
#include <cmath>
#include <iostream>

std::shared_ptr<PolygonObject> generateFloor(double startX, double endX, double startZ, double endZ, double step) {
    std::vector<std::vector<double>> polygons;

    bool isWhite = true;

    for (double x = startX; x < endX; x += step)
    {
        for (double z = startZ; z < endZ; z += step)
        {
            Color color = isWhite ? Color(143, 188, 143) : Color(202, 227, 202); // Чередуем цвет
            //Color color = isWhite ? Color(250, 157, 244) : Color(227, 9, 212);
            // Два треугольника для клетки
            polygons.push_back({x, 0.0, z, x + step, 0.0, z, x, 0.0, z + step, color.r, color.g, color.b, 0, 1, 0});
            polygons.push_back({x + step, 0.0, z, x + step, 0.0, z + step, x, 0.0, z + step, color.r, color.g, color.b, 0, 1, 0});

            isWhite = !isWhite; // Меняем цвет для следующей клетки
        }
        if ((int)((endX - startX) / step) % 2 == 0)
        {
            isWhite = !isWhite; // Меняем цвет в каждом новом ряду, если четное число клеток
        }
    }

    std::cout << "Generated " << polygons.size() << " triangles." << "\n"; // Лог

    // Создаем PolygonObject из сгенерированных полигонов
    return std::make_shared<PolygonObject>(polygons);
}


// Функция для вычисления нормали к треугольнику
std::vector<double> calculateNormal(const std::vector<double>& v0, const std::vector<double>& v1, const std::vector<double>& v2) {
    double nx = (v1[1] - v0[1]) * (v2[2] - v0[2]) - (v1[2] - v0[2]) * (v2[1] - v0[1]);
    double ny = (v1[2] - v0[2]) * (v2[0] - v0[0]) - (v1[0] - v0[0]) * (v2[2] - v0[2]);
    double nz = (v1[0] - v0[0]) * (v2[1] - v0[1]) - (v1[1] - v0[1]) * (v2[0] - v0[0]);
    double length = -1; //std::sqrt(nx * nx + ny * ny + nz * nz);
    return {nx / length, ny / length, nz / length};
}

std::shared_ptr<PolygonObject> generateSphere(double radius) {
    std::vector<std::vector<double>> polygons;
    double segments = 100;
    double rings = 20;
    double pi = 3.14159265358979323846;
    double ofx = 10;
    double ofy = 20;
    double ofz = 20;

    // Перебираем кольца (latitude)
    for (int i = 0; i < rings; ++i) {
        double theta1 = pi * i / rings; // Текущий угол кольца
        double theta2 = pi * (i + 1) / rings; // Следующий угол кольца

        // Координаты по высоте (y) для текущего и следующего колец
        double y1 = radius * cos(theta1);
        double y2 = radius * cos(theta2);

        // Радиусы текущего и следующего колец
        double radius1 = radius * sin(theta1);
        double radius2 = radius * sin(theta2);

        // Перебираем сегменты для каждого кольца (longitude)
        for (int j = 0; j < segments; ++j) {
            double phi1 = 2 * pi * j / segments; // Текущий угол сегмента
            double phi2 = 2 * pi * (j + 1) / segments; // Следующий угол сегмента

            // Вычисляем координаты четырех вершин
            std::vector<double> v0 = { radius1 * cos(phi1) + ofx, y1 + ofy, radius1 * sin(phi1) + ofz };
            std::vector<double> v1 = { radius1 * cos(phi2) + ofx, y1 + ofy, radius1 * sin(phi2) + ofz };
            std::vector<double> v2 = { radius2 * cos(phi1) + ofx, y2 + ofy, radius2 * sin(phi1) + ofz };
            std::vector<double> v3 = { radius2 * cos(phi2) + ofx, y2 + ofy, radius2 * sin(phi2) + ofz };

            // Вычисляем нормали для каждого треугольника
            auto normal1 = calculateNormal(v0, v2, v1);
            auto normal2 = calculateNormal(v1, v2, v3);

            Color color(235, 111, 77); // Цвет полигона

            // Добавляем полигоны с нормалями
            polygons.push_back({v0[0], v0[1], v0[2], v2[0], v2[1], v2[2], v1[0], v1[1], v1[2], color.r, color.g, color.b,
                                normal1[0], normal1[1], normal1[2]});
            polygons.push_back({v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2], color.r, color.g, color.b,
                                normal2[0], normal2[1], normal2[2]});
        }
    }

    std::cout << "Generated " << polygons.size() << " triangles for the sphere." << "\n"; // Лог

    return std::make_shared<PolygonObject>(polygons);
}



Manager::Manager()
{
    std::cout << "НАЧАЛИ: \n";
    _scene = std::make_shared<Scene>();
    double cam_screen = 40.0;
    _camera = std::make_shared<Camera>(25.0, 15.0, cam_screen - 265 + 60, 25.0, 15.0, cam_screen + 60);
    
    
    std::shared_ptr<PerlinNoiseMountainGenerator> g = std::make_shared<PerlinNoiseMountainGenerator>(10, 10, 20);
    //std::shared_ptr<DiamondSquareMountainGenerator> g = std::make_shared<DiamondSquareMountainGenerator>(65, 15);
    std::shared_ptr<PolygonObject> mountain = g->generateMountain();
    
    std::shared_ptr<ObjReader> r = std::make_shared<ObjReader>();
    _balloon = r->Read("../Models/balloon3.obj");

    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(mountain));
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_balloon));
    
    std::shared_ptr<PolygonObject> oxo = std::make_shared<PolygonObject>(
        std::initializer_list<std::initializer_list<double>>{
                // Преобразование первой стороны куба
                {-100.0, 0, -100.0, 100.0, 0.0, -100.0, 0.0, 0.0, 100.0, 66, 66, 66}, // Треугольник 11
        }

    );
    //_scene->addObject(std::dynamic_pointer_cast<BaseObject>(oxo));

    //std::shared_ptr<PolygonObject> floor = generateSphere(5);
    //_scene->addObject(std::dynamic_pointer_cast<BaseObject>(floor));

   setInfo(true);
}

void Manager::SetDrawer(std::shared_ptr<QtDrawer> drawer)
{
    _drawer = drawer;
}

void Manager::CamPlus()
{
    _camera->z_screen -= 20;
    _camera->z_view -= 20;
    
    _balloon->Move(0, 0, -20);
}

void Manager::CamMinus()
{
    _camera->z_screen += 10;
    _camera->z_view += 10;

    _balloon->Move(0, 0, 10);
}

void Manager::CamUp()
{
    _camera->x_screen += 10;
    _camera->x_view += 10;

    _balloon->Move(10, 0, 0);
}

void Manager::CamDown()
{
    _camera->x_screen -= 20;
    _camera->x_view -= 20;

    _balloon->Move(-20, 0, 0);
}

void Manager::CamLeft()
{
    _camera->y_screen -= 20;
    _camera->y_view -= 20;

    _balloon->Move(0, -20, 0);
}

void Manager::CamRight()
{
    _camera->y_screen += 20;
    _camera->y_view += 20;

    _balloon->Move(0, 20, 0);
}

void Manager::CamPitchUp()
{
    _camera->pitch += 0.5;
}

void Manager::CamPitchDown()
{
    _camera->pitch -= 0.5;
}

void Manager::CamYawLeft()
{
    _camera->yaw -= 0.5;
}

void Manager::CamYawRight()
{
    _camera->yaw += 0.5;
}

void Manager::Manage()
{
    //_scene->ClearScene();
    
    std::shared_ptr<PolygonDrawAdapter> adapter = std::make_shared<PolygonDrawAdapter>(_drawer);
    adapter->Draw(_scene, _camera); 
}


void Manager::setInfo(bool on)
{
    if (!on) return;

    std::shared_ptr<PolygonObject> cube = std::make_shared<PolygonObject>(
            std::initializer_list<std::initializer_list<double>>{
                    // Преобразование первой стороны куба
                    {10.0, 0.0, 10.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 245, 188, 103, 0, 0, 1}, // aТреугольник 11
                    {10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0, 245, 188, 103, 0, 0, 1}, // a
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 245, 188, 103, 0, 0, -1}, // Треугольник 1
                    {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 0.0, 245, 188, 103, 0, 0, -1}, //b Треугольник 2
                    // Преобразование второй стороны куба
                    {0.0, 0.0, 10.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 245, 188, 103, -1, 0, 0}, // Треугольник 3
                    {0.0, 10.0, 0.0, 0.0, 0.0, 10.0,0.0, 10.0, 10.0, 245, 188, 103, -1, 0, 0}, // aТреугольник 4
                    // Преобразование третьей стороны куба
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 245, 188, 103, 0, -1, 0}, // Треугольник 5
                    {10.0, 0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 245, 188, 103, 0, -1, 0}, // abТреугольник 6
                    // Преобразование четвертой стороны куба
                    {10.0, 10.0, 0.0, 10.0, 0.0, 0.0, 10.0, 0.0, 10.0, 245, 188, 103, 1, 0, 0}, //b Треугольник 7
                    {10.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0, 245, 188, 103, 1, 0, 0}, //b Треугольник 8
                    // Преобразование пятой стороны куба
                    {0.0, 10.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 10.0, 245, 188, 103, 0, 1, 0}, // Треугольник 9
                    {10.0, 10.0, 0.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0, 245, 188, 103, 0, 1, 0}, // Треугольник 10
            }

    );
    //_scene->addObject(std::dynamic_pointer_cast<BaseObject>(cube));

    std::shared_ptr<PolygonObject> floor = generateFloor(-100.0, 100.0, -30.0, 30.0, 10.0);
    //_scene->addObject(std::dynamic_pointer_cast<BaseObject>(floor));
}

void Manager::SetCameraPosition(int type)
{
    if (type == 1)
    {
        auto sphere = _balloon->Sphere();
        
        
        _camera->x_screen = sphere.center[0] - 67;
        _camera->y_screen = sphere.center[1] + 15;
        _camera->z_screen = sphere.center[2] - 165;

        _camera->x_view = sphere.center[0] - 67;
        _camera->y_view = sphere.center[1] + 15;
        _camera->z_view = _camera->z_screen + 265;
        std::cout << _camera->x_screen << " " << _camera->y_screen << " " << _camera->z_view << "\n";
        std::cout << sphere.center[0] << " " << sphere.center[1] << " " << sphere.center[2] << "\n";
         _scene->removeObject(_balloon);
        _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_balloon));

    }
    else if (type == 2)
    {
        _scene->removeObject(_balloon);
    }
    else if (type == 3)
    {
        auto sphere = _balloon->Sphere();
        _camera->x_screen = sphere.center[0] - 75;
        _camera->y_screen = sphere.center[1];
        _camera->z_screen = sphere.center[2] - 263;

        _camera->x_view = sphere.center[0] - 75;
        _camera->y_view = sphere.center[1];
        _camera->z_view = _camera->z_screen + 265;
        std::cout << _camera->x_screen << " " << _camera->y_screen << " " << _camera->z_view << "\n";
        std::cout << sphere.center[0] << " " << sphere.center[1] << " " << sphere.center[2] << "\n";
        _scene->removeObject(_balloon);
        _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_balloon));

    }

}

Manager::~Manager() {}