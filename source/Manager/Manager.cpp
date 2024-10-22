#include "Manager.h"

#include <memory>

#include "../Scene/Scene.h"
#include "../Objects/Concrete/CarcasObject.h"
#include "../Objects/Concrete/PolygonObject.h"
#include "../DrawAdapter/Concrete/CarcasDrawAdapter.h"
#include "../DrawAdapter/Concrete/PolygonDrawAdapter.h"
#include <iostream>

std::shared_ptr<PolygonObject> generateFloor(double startX, double endX, double startZ, double endZ, double step) {
    std::vector<std::vector<double>> polygons;

    bool isWhite = true;

    for (double x = startX; x < endX; x += step) {
        for (double z = startZ; z < endZ; z += step) {
            Color color = isWhite ? Color(235, 152, 52) : Color(52, 76, 235); // Чередуем цвет

            // Два треугольника для клетки
            polygons.push_back({x, 0.0, z, x + step, 0.0, z, x, 0.0, z + step, color.r, color.g, color.b});
            polygons.push_back({x + step, 0.0, z, x + step, 0.0, z + step, x, 0.0, z + step, color.r, color.g, color.b});

            isWhite = !isWhite; // Меняем цвет для следующей клетки
        }
        if ((int)((endX - startX) / step) % 2 == 0) {
            isWhite = !isWhite; // Меняем цвет в каждом новом ряду, если четное число клеток
        }
    }

    std::cout << "Generated " << polygons.size() << " triangles." << "\n"; // Лог

    // Создаем PolygonObject из сгенерированных полигонов
    return std::make_shared<PolygonObject>(polygons);
}



Manager::Manager()
{
    _scene = std::make_shared<Scene>();
    _camera = std::make_shared<Camera>(15.0, 16.0, -15.0, 15.0, 16.0, 15.0);
    /*
    _solution = std::make_shared<SolutionImpl<BaseObject, BaseDrawAdapter>>(
            std::initializer_list<std::pair<BaseObject, BaseDrawAdapter>>{
                    {CarcasObject, CarcasDrawAdapter}
            }
    );
     */
}

void Manager::SetDrawer(std::shared_ptr<QtDrawer> drawer)
{
    _drawer = drawer;
}

void Manager::CamPlus()
{
    _camera->z_screen -= 2;
    _camera->z_view -= 2;
}

void Manager::CamMinus()
{
    _camera->z_screen += 2;
    _camera->z_view += 2;
}

void Manager::CamUp()
{
    _camera->y_screen += 2;
    _camera->y_view += 2;
}

void Manager::CamDown()
{
    _camera->y_screen -= 2;
    _camera->y_view -= 2;
}

void Manager::CamLeft()
{
    _camera->x_screen -= 2;
    _camera->x_view -= 2;
}

void Manager::CamRight()
{
    _camera->x_screen += 2;
    _camera->x_view += 2;
}

void Manager::CamPitchUp()
{
    _camera->pitch += 0.1;
}

void Manager::CamPitchDown()
{
    _camera->pitch -= 0.1;
}

void Manager::CamYawLeft()
{
    _camera->yaw -= 0.1;
}

void Manager::CamYawRight()
{
    _camera->yaw += 0.1;
}

void Manager::Manage()
{
    _scene->ClearScene();
    setInfo();

    std::shared_ptr<PolygonObject> ox = std::make_shared<PolygonObject>(
            std::initializer_list<std::initializer_list<double>>{
                    // Преобразование первой стороны куба
                    {0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0, 10.0, 255, 0, 0}, // Треугольник 11
                    {10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0, 255, 0, 0},
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 255, 0, 0}, // Треугольник 1
                    {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 0.0, 255, 0, 0}, // Треугольник 2
                    // Преобразование второй стороны куба
                    {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0.0, 255, 0, 0}, // Треугольник 3
                    {0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 10.0, 10.0, 255, 0, 0}, // Треугольник 4
                    // Преобразование третьей стороны куба
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 255, 255, 0}, // Треугольник 5
                    {10.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0.0, 0.0, 10.0, 255, 0, 0}, // Треугольник 6
                    // Преобразование четвертой стороны куба
                    {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0, 255, 0, 0}, // Треугольник 7
                    {10.0, 10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0, 255, 0, 0}, // Треугольник 8
                    // Преобразование пятой стороны куба
                    {0.0, 10.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 10.0, 255, 0, 0}, // Треугольник 9
                    {10.0, 10.0, 0.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0, 255, 0, 0}, // Треугольник 10
            }

    );
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(ox));


    /*
    std::shared_ptr<PolygonObject> floor = std::make_shared<PolygonObject>(
            std::initializer_list<std::initializer_list<double>>{
                    // Пример сетки 4x4
                    // Первая клетка: треугольники белого цвета
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 255, 255, 255}, // Треугольник 1
                    {10.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0.0, 0.0, 10.0, 255, 255, 255}, // Треугольник 2

                    // Вторая клетка: треугольники синего цвета
                    {10.0, 0.0, 0.0, 20.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0, 0, 255}, // Треугольник 3
                    {20.0, 0.0, 0.0, 20.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0, 0, 255}, // Треугольник 4


                    // Третья клетка: белые треугольники
                    {0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 0.0, 20.0, 255, 255, 255}, // Треугольник 5
                    {10.0, 0.0, 10.0, 10.0, 0.0, 20.0, 0.0, 0.0, 20.0, 255, 255, 255}, // Треугольник 6

                    // Четвертая клетка: синие треугольники
                    {10.0, 0.0, 10.0, 20.0, 0.0, 10.0, 10.0, 0.0, 20.0, 0, 0, 255}, // Треугольник 7
                    {20.0, 0.0, 10.0, 20.0, 0.0, 20.0, 10.0, 0.0, 20.0, 0, 0, 255}, // Треугольник 8


            }
    );

    // Добавляем объект пола на сцену
        _scene->addObject(std::dynamic_pointer_cast<BaseObject>(floor));
    */

    std::shared_ptr<PolygonObject> floor = generateFloor(-100.0, 100.0, -30.0, 30.0, 10.0);
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(floor));

    std::shared_ptr<PolygonDrawAdapter> adapter = std::make_shared<PolygonDrawAdapter>(_drawer);
    adapter->Draw(_scene, _camera);
}


void Manager::setInfo()
{
    /*
    std::shared_ptr<CarcasObject> ox = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 20.0, 0.0, 0.0}}
    );
    std::shared_ptr<CarcasDrawAdapter> adapter = std::make_shared<CarcasDrawAdapter>(_drawer);
    _drawer->setColor(255, 0, 0);
    adapter->Draw(*ox, *_camera);

    std::shared_ptr<CarcasObject> oy = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 0.0, 20.0, 0.0}}
    );
    _drawer->setColor(0, 255, 0);
    adapter->Draw(*oy, *_camera);

    std::shared_ptr<CarcasObject> oz = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 0.0, 0.0, 20.0}}
    );
    _drawer->setColor(0, 0, 255);
    adapter->Draw(*oz, *_camera);
    */
}

Manager::~Manager() {}