#include <memory>

#include "CarcasDrawAdapter.h"
#include "../../Drawer/QtDrawer.h"

CarcasDrawAdapter::CarcasDrawAdapter(std::shared_ptr<QtDrawer> drawer) : _drawer(drawer)
{}

void CarcasDrawAdapter::Draw(CarcasObject &object)
{

}


CarcasDrawAdapter::~CarcasDrawAdapter(){}

