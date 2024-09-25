#ifndef CG_CARCASDRAWADAPTER_H
#define CG_CARCASDRAWADAPTER_H

#include <memory>

#include "../BaseDrawAdapter.h"
#include "../../Drawer/QtDrawer.h"
#include "../../Objects/BaseObject.h"
#include "../../Camera/Camera.h"

/* ПО УМОЛЧАНИЮ ЭТО ДЛЯ РИСОВАНИЯ ВСПОМОГАТЕЛЬНЫХ ЭЛЕМЕНТОВ */
/* Поэтому он принимает просто объекты */
class CarcasDrawAdapter : public BaseDrawAdapter
{
public:
    CarcasDrawAdapter(std::shared_ptr<QtDrawer> drawer);
    void Draw(BaseObject &object, Camera &camera);
    ~CarcasDrawAdapter();

private:
    std::shared_ptr<QtDrawer> _drawer;
};


#endif //CG_CARCASDRAWADAPTER_H
