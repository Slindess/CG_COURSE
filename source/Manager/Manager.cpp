#include "Manager.h"

#include <memory>

#include "../Scene/Scene.h"
#include "../Objects/Concrete/CarcasObject.h"
#include "../Objects/Concrete/PolygonObject.h"
#include "../DrawAdapter/Concrete/CarcasDrawAdapter.h"
#include "../DrawAdapter/Concrete/PolygonDrawAdapter.h"

Manager::Manager()
{
    _scene = std::make_shared<Scene>();
    _camera = std::make_shared<Camera>(0.0, 0.0, 0.0, 0.0, 0.0, 10.0);
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
    setInfo();

    /*
    std::shared_ptr<CarcasObject> ox = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 10.0, 0.0, 0.0},
                                                                 {0.0, 0.0, 0.0, 0.0, 10.0, 0.0},
                                                                 {0.0, 0.0, 0.0, 0.0, 0.0, 10.0},
                                                                 {10.0, 0.0, 0.0, 10.0, 10.0, 0.0},
                                                                 {10.0, 10.0, 0.0, 0.0, 10.0, 0.0},
                                                                 {0.0, 10.0, 0.0, 0.0, 10.0, 10.0},
                                                                 {0.0, 10.0, 10.0, 0.0, 0.0, 10.0},
                                                                 {0.0, 0.0, 10.0, 10.0, 0.0, 10.0},
                                                                 {10.0, 0.0, 10.0, 10.0, 0.0, 0.0},
                                                                 {10.0, 0.0, 10.0, 10.0, 10.0, 10.0},
                                                                 {10.0, 10.0, 10.0, 10.0, 10.0, 0.0},
                                                                 {10.0, 10.0, 10.0, 0.0, 10.0, 10.0}}
    );
     */
    /*
    std::shared_ptr<PolygonObject> ox = std::make_shared<PolygonObject>(
        std::initializer_list<std::initializer_list<double>>{
                // Преобразование первой стороны куба
                {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0}, // Треугольник 1
                {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 0.0}, // Треугольник 2
                // Преобразование второй стороны куба
                {0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0.0}, // Треугольник 3
                {0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 10.0, 10.0}, // Треугольник 4
                // Преобразование третьей стороны куба
                {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0}, // Треугольник 5
                {10.0, 0.0, 0.0, 10.0, 0.0, 10.0, 0.0, 0.0, 10.0}, // Треугольник 6
                // Преобразование четвертой стороны куба
                {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0}, // Треугольник 7
                {10.0, 10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0}, // Треугольник 8
                // Преобразование пятой стороны куба
                {0.0, 10.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 10.0}, // Треугольник 9
                {10.0, 10.0, 0.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0}, // Треугольник 10
                // Преобразование шестой стороны куба
                {0.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 10.0, 10.0}, // Треугольник 11
                {10.0, 0.0, 10.0, 10.0, 10.0, 10.0, 0.0, 10.0, 10.0}  // Треугольник 12
        }
    );
     */
    std::shared_ptr<PolygonObject> ox = std::make_shared<PolygonObject>(
            std::initializer_list<std::initializer_list<double>>{
                    // Преобразование первой стороны куба
                    {0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 10.0, 0.0},
                    {10.0, 0.0, 0.0, 10.0, 10.0, 0.0, 0.0, 10.0, 0.0}  // Рисует грань абсолютно зеркально и не там
                    //{10.0, 0.0, 0.0,  0.0, 10.0, 0.0, 10.0, 10.0, 0.0}, // Рисует нормально, хотя это та же грань, что и выше, где поменяны две вершины местами
                    //{10.0, 0.0, 10.0,  0.0, 10.0, 10.0, 10.0, 10.0, 10.0}
            }
    );
    _scene->addObject(std::dynamic_pointer_cast<BaseObject>(ox));
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