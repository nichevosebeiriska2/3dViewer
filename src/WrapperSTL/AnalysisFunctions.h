#ifndef ANALYSIS_FUNCTIONS_H
#define ANALYSIS_FUNCTIONS_H

#include "WrapperSTL.h"

using UINT = unsigned int;


bool CheckIsManifold(ObjectSTL* object);
std::shared_ptr<ObjectSTL> GetBoundingBox(ObjectSTL* object);
QVector3D GetMiddlePoint(ObjectSTL* object);


#endif