//
// Created by Михаил Слиняков on 22.09.2024.
//

#include "Scene.h"

void Scene::addObject(const std::shared_ptr<BaseObject>& object)
{
    objects.push_back(object);
}

void Scene::removeObject(const std::shared_ptr<BaseObject>& object)
{
    objects.remove(object);
}

size_t Scene::getObjectCount() const
{
    return objects.size();
}