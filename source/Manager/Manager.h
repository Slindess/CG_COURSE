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
#include <map>
#include <functional>
#include "../Texture/BaseTexture.h"
#include "../Texture/Concrete/SimpleMountainTexture.h"
#include "../Texture/Concrete/SnowTexture.h"
#include "../Texture/Concrete/SandTexture.h"
#include "../Texture/Concrete/JungleTexture.h"

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
    void ResetLandScape(int type); 
    void ResetHeight(int heightt);
    void Source(std::string source);
    void Snow(int snoww, int melting = 0);
    void Manage();  // ЕГО МБ БУДЕТ ПО ТАЙМЕРУ ДЕРГАТЬ QT???
    ~Manager();

protected:


private:
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<QtDrawer> _drawer;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<PolygonObject> _landScape; // Пришлось пойти на это, потому что нужно менять ландшафт
    std::shared_ptr<PolygonObject> _balloon; // Пришлось пойти на это, потому что шар привязан к камере
    std::map<int, std::function<std::shared_ptr<BaseTexture>()>> textureFactories;
    //std::shared_ptr<SolutionImpl<BaseObject, BaseDrawAdapter>> _solution;

    int landscape_type = 1;
    int height = 180;
    int snow = 0;
    void setInfo(bool on);
};


#endif //CG_MANAGER_H
