#ifndef CG_MANAGER_H
#define CG_MANAGER_H

#include <memory>

#include "../Drawer/QtDrawer.h"
#include "../Scene/Scene.h"
#include "../Utils/SolutionImpl.h"
#include "../Objects/BaseObject.h"
#include "../Objects/Concrete/PolygonObject.h"
#include "../DrawAdapter/BaseDrawAdapter.h"
#include "../Camera/Camera.h"

/* ОСНОВНУЮ ЛОГИКУ БУДУ ВЕШАТЬ СЮДА */
class Manager
{
public:
    Manager();
    void SetDrawer(std::shared_ptr<QtDrawer> drawer);
    void CamPlus();
    void CamMinus();
    void CamUp();
    void CamDown();
    void CamLeft();
    void CamRight();
    void CamPitchUp();
    void CamPitchDown();
    void CamYawLeft();
    void CamYawRight();
    void SetCameraPosition(int type);  // Ну уже совсем минус-вайб
    void Manage();  // ЕГО МБ БУДЕТ ПО ТАЙМЕРУ ДЕРГАТЬ QT???
    ~Manager();

protected:


private:
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<QtDrawer> _drawer;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<PolygonObject> _balloon; // Пришлось пойти на это, потому что шар привязан к камере
    //std::shared_ptr<SolutionImpl<BaseObject, BaseDrawAdapter>> _solution;

    void setInfo(bool on);
};


#endif //CG_MANAGER_H
