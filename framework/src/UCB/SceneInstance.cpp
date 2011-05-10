/*
 * SceneInstance.cpp
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#include "SceneInstance.h"
#include "SceneGroup.h"

SceneInstance::SceneInstance()
{
    _name = "unassigned";
    
    _color = NULL;
    _lod = NULL;
    _child = NULL;
}

SceneInstance::~SceneInstance()
{
    delete _color;
    delete _lod;
    for (vector<Transform*>::iterator it = _transforms.begin(); it != _transforms.end(); ++it)
        delete *it;
}

SceneGroup * SceneInstance::getChild() {
    return _child;
}


string SceneInstance::getName() {
    return _name;
}

bool SceneInstance::computeColor(vec3 &color, int time)
{
    if (_color == NULL)
        return false;
    else
        color = _color->getColor(time);
    return true;
}

bool SceneInstance::computeLOD(int &lod, int time)
{
    if (_lod == NULL)
        return false;
    else
        lod = _lod->getLod(time);
    return true;
}

bool SceneInstance::computeTransform(mat4 &mat, int time)
{
    mat = identity3D();
    if (_transforms.empty())
        return false;
    else {
        for (vector<Transform*>::reverse_iterator it = _transforms.rbegin(); it != _transforms.rend(); ++it)
        {
            mat = mat * (**it).getMatrix(time);
        }
    }
    return true;
}
