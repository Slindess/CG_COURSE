#ifndef CG_MANAGER_H
#define CG_MANAGER_H

#include <memory>

#include "../Drawer/QtDrawer.h"
#include "../Scene/Scene.h"

/* ОСНОВНУЮ ЛОГИКУ БУДУ ВЕШАТЬ СЮДА */
class Manager
{
public:
    Manager();
    void SetDrawer(std::shared_ptr<QtDrawer> drawer)
    void Manage();  // ЕГО МБ БУДЕТ ПО ТАЙМЕРУ ДЕРГАТЬ QT???
    ~Manager();

private:
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<QtDrawer> _drawer;
};


#endif //CG_MANAGER_H
