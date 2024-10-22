#ifndef CG_SCENE_H
#define CG_SCENE_H

#include <list>
#include <memory>
#include "../Objects/BaseObject.h"

class Scene
{
public:
    std::list<std::shared_ptr<BaseObject>> objects;
    void addObject(const std::shared_ptr<BaseObject>& object);
    void removeObject(const std::shared_ptr<BaseObject>& object);
    size_t getObjectCount() const;
    void ClearScene();
};


#endif //CG_SCENE_H
