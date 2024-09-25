#include "Manager.h"

#include <memory>

#include "../Scene/Scene.h"
#include "../Objects/Concrete/CarcasObject.h"
#include "../DrawAdapter/Concrete/CarcasDrawAdapter.h"

Manager::Manager()
{
    _scene = std::make_shared<Scene>();
    _camera = std::make_shared<Camera>(0.0, 0.0, 20, 0.0, 0.0, 50.0);
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
    std::shared_ptr<CarcasDrawAdapter> adapter = std::make_shared<CarcasDrawAdapter>(_drawer);
    _drawer->setColor(0, 0, 0);
    adapter->Draw(*ox, *_camera);
}


void Manager::setInfo()
{
    /* OX */
    std::shared_ptr<CarcasObject> ox = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 20.0, 0.0, 0.0}}
    );
    std::shared_ptr<CarcasDrawAdapter> adapter = std::make_shared<CarcasDrawAdapter>(_drawer);
    _drawer->setColor(255, 0, 0);
    adapter->Draw(*ox, *_camera);

    /* OY */
    std::shared_ptr<CarcasObject> oy = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 0.0, 20.0, 0.0}}
    );
    _drawer->setColor(0, 255, 0);
    adapter->Draw(*oy, *_camera);

    /* OZ */
    std::shared_ptr<CarcasObject> oz = std::make_shared<CarcasObject>(
            std::initializer_list<std::initializer_list<double>>{{0.0, 0.0, 0.0, 0.0, 0.0, 20.0}}
    );
    _drawer->setColor(0, 0, 255);
    adapter->Draw(*oz, *_camera);

}

Manager::~Manager() {}