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
#include <random>
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

    //std::cout << "Generated " << polygons.size() << " triangles." << "\n"; // Лог

    // Создаем PolygonObject из сгенерированных полигонов
    return std::make_shared<PolygonObject>(polygons);
}

Manager::Manager()
{
    std::cout << "Начальные настройки для генерации: \n";
    std::cout << "Тип ланшдафта: " << landscape_type << "\n";
    std::cout << "Средняя высота: " << height << "\n";
    std::cout << "Заснеженность: " << snow << "\n";
    textureFactories[1] = []() {return std::make_shared<SimpleMountainTexture>();};
    textureFactories[2] = []() {return std::make_shared<SandTexture>();};
    textureFactories[3] = []() {return std::make_shared<JungleTexture>();};

    _scene = std::make_shared<Scene>();
    double cam_screen = 40.0;
    _camera = std::make_shared<Camera>(25.0, 15.0, cam_screen - 265 + 60, 25.0, 15.0, cam_screen + 60);
    
    
    std::shared_ptr<PerlinNoiseMountainGenerator> g = std::make_shared<PerlinNoiseMountainGenerator>(10, 10, 20);
    //std::shared_ptr<DiamondSquareMountainGenerator> g = std::make_shared<DiamondSquareMountainGenerator>(65, 15);
    _landScape = g->generateMountain(landscape_type, height, snow);
    
    std::shared_ptr<ObjReader> r = std::make_shared<ObjReader>();
    _balloon = r->Read("../Models/balloon3.obj", 0);

    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_landScape));
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_balloon));
    
    std::shared_ptr<PolygonObject> oxo = std::make_shared<PolygonObject>(
        std::initializer_list<std::initializer_list<double>>{
                // Преобразование первой стороны куба
                {-100.0, 0, -100.0, 100.0, 0.0, -100.0, 0.0, 0.0, 100.0, 66, 66, 66}, // Треугольник 11
        }

    );

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
    else if (type == 4)
    {
        auto sphere = _balloon->Sphere();
        _camera->x_screen = sphere.center[0] + 50;
        _camera->y_screen = sphere.center[1] + 15;
        _camera->z_screen = sphere.center[2] - 65;

        _camera->x_view = sphere.center[0] + 50;
        _camera->y_view = sphere.center[1] + 15;
        _camera->z_view = _camera->z_screen + 265;
        std::cout << _camera->x_screen << " " << _camera->y_screen << " " << _camera->z_view << "\n";
        std::cout << sphere.center[0] << " " << sphere.center[1] << " " << sphere.center[2] << "\n";
         _scene->removeObject(_balloon);
        _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_balloon));

    }

    std::cout << "Новая позиция камеры: " << type << "\n";
}

void Manager::ResetLandScape(int type)
{
    landscape_type = type;
    _scene->removeObject(_landScape);
    std::shared_ptr<PerlinNoiseMountainGenerator> g = std::make_shared<PerlinNoiseMountainGenerator>(10, 10, 20);
    //std::shared_ptr<DiamondSquareMountainGenerator> g = std::make_shared<DiamondSquareMountainGenerator>(65, 15);
    _landScape = g->generateMountain(landscape_type, height, snow);
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_landScape));
    std::cout << "Новый тип: " << type << "\n";
}

void Manager::ResetHeight(int heightt)
{
    height = heightt;
    _scene->removeObject(_landScape);
    std::shared_ptr<PerlinNoiseMountainGenerator> g = std::make_shared<PerlinNoiseMountainGenerator>(10, 10, 20);
    //std::shared_ptr<DiamondSquareMountainGenerator> g = std::make_shared<DiamondSquareMountainGenerator>(65, 15);
    _landScape = g->generateMountain(landscape_type, height, snow);
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_landScape));
    std::cout << "Новая высота: " << heightt << "\n";
}

void Manager::Source(std::string source)
{
    _scene->removeObject(_landScape);
    std::shared_ptr<ObjReader> r = std::make_shared<ObjReader>();
    _landScape = r->Read("../Models/" + source, landscape_type);
    if (_landScape != nullptr)
        _scene->addObject(std::dynamic_pointer_cast<BaseObject>(_landScape));
}

void Manager::Snow(int snoww, int melting)
{

    snow = snoww;

    if (!melting)
    {
        for (const auto& component : _landScape->GetComponents())
        {
                auto polygon = std::dynamic_pointer_cast<Polygon>(component);
                if (!polygon) continue;

                polygon->texture = textureFactories.find(landscape_type)->second();;

        }
    }

   for (const auto& component : _landScape->GetComponents())
   {
        auto polygon = std::dynamic_pointer_cast<Polygon>(component);
        if (!polygon) continue;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(1, 10);
            int random_number = distrib(gen);
            if (melting)
            {
                if (std::dynamic_pointer_cast<SnowTexture>(polygon->texture))
                {
                    if (random_number < 10 - snow)
                    {
                        polygon->texture = textureFactories.find(landscape_type)->second();
                    }
                }
            }
            else
            {
                if (random_number > 10 - snow)
                {
                    polygon->texture = std::make_shared<SnowTexture>();
                }
            }
    }

    std::cout << "MYSNIW: " << snow; 
}

Manager::~Manager() {}