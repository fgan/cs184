/*
 * SceneInstance.h
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#ifndef SCENEINSTANCE_H_
#define SCENEINSTANCE_H_

#include "SceneData.h"
#include <vector>

using namespace std;

class SceneInstance {
public:
    string getName(); /* get the instance's name; useful for debugging */

    bool computeColor(vec3 &color, int time = 0); /* get the instance's color; returns false if no color specified */
    bool computeTransform(mat4 &mat, int time = 0); /* get the instances's transform; returns false if no transform specified */
    bool computeLOD(int &lod, int time = 0); /* get the instances's LOD, returns false if no LOD specified */

    class SceneGroup *getChild(); /* get the group which is a child of this instance */

    virtual ~SceneInstance();

private:
    SceneInstance(); // private constructor to be called by SceneLoader only

    // private copy constructor and assignment operator to avoid copying this data
    SceneInstance(const SceneInstance&);
    SceneInstance& operator=(const SceneInstance&);

    string _name;

    vector<Transform *> _transforms;
    ParametricColor * _color;
    LOD * _lod;

    SceneGroup * _child;

    friend class SceneLoader;
};

#endif /* SCENEINSTANCE_H_ */
