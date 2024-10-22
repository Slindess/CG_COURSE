//
// Created by Михаил Слиняков on 20.10.2024.
//

#ifndef CG_POLYGONDRAWADAPTER_H
#define CG_POLYGONDRAWADAPTER_H

#include "../BaseDrawAdapter.h"
#include "../../Drawer/QtDrawer.h"
#include "../../Scene/Scene.h"
#include "../../Camera/Camera.h"
#include <memory>

class PolygonDrawAdapter : public BaseDrawAdapter
{
public:
    PolygonDrawAdapter(std::shared_ptr<QtDrawer> drawer);
    void Draw(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
    ~PolygonDrawAdapter();

private:
    std::shared_ptr<QtDrawer> _drawer;

};


#endif //CG_POLYGONDRAWADAPTER_H
