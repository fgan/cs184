/*
 * SceneGroup.h
 *
 *  Created on: Feb 12, 2009
 *      Author: jima
 */

#ifndef SCENEGROUP_H_
#define SCENEGROUP_H_

#include <vector>
#include "SceneData.h"
#include "MeshInfo.h"
#include "algebra3.h"


using namespace std;

class SceneGroup {
public:
    string getName(); /* get the name of the group, useful for debugging */

    /* functions get objects that can exist at leaf nodes -- return false if related object is not present */
    bool computeMesh(OBJTriangleMesh* &mesh, MaterialInfo &material, int time); /* get a mesh */
    bool computeSphere(double &radius, MaterialInfo &material, int time); /* get a sphere */
    bool computeLight(LightInfo &ld, int time = 0); /* get light parameters */
    bool computeCamera(CameraInfo &frustum, int time = 0); /* get camera frustum */

    int getChildCount(); /* get the amount of instances which are in the group */
    SceneInstance *getChild(int i); /* get a child */

    virtual ~SceneGroup();

private:
    SceneGroup(); // private constructor to be called by SceneLoader only

    // private copy constructor and assignment operator to avoid copying this data
    SceneGroup(const SceneGroup&);
    SceneGroup& operator=(const SceneGroup&);

    string _name;
    vector<SceneInstance *> _children;
    ParametricSphere * _sphere;
    ParametricLight *_light;
    ParametricCamera *_camera;
    OBJTriangleMesh *_mesh;
    ParametricMaterial *_meshMaterial;

    friend class SceneLoader;
};

#endif /* SCENEGROUP_H_ */
