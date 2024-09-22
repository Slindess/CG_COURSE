#ifndef CG_BASEOBJECT_H
#define CG_BASEOBJECT_H

#include "../Components/BaseComponent.h"
#include <list>
#include <memory>

class BaseObject
{
public:
    BaseObject() {};
    std::list<std::shared_ptr<BaseComponent>> GetComponents() { return _components; };
    virtual ~BaseObject() = 0;

protected:
    std::list<std::shared_ptr<BaseComponent>> _components;
};


#endif //CG_BASEOBJECT_H
