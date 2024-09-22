#ifndef CG_SCENE_H
#define CG_SCENE_H

#include <list>
#include <memory>
#include "../Objects/BaseObject.h"

class Scene
{
public:
    std::list<std::shared_ptr<BaseObject>> objects;
};


#endif //CG_SCENE_H
