/*
 * Scene.h
 *
 * The main scene class -- constructor calls the scene loader, getRoot gives a starting point for scene traversal
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "SceneInstance.h"
#include "SceneGroup.h"

using namespace std;

class SceneLoader;

class Scene {
public:
    Scene(string filename); // Load a scene with the provided file name
    ~Scene();

    SceneInstance *getRoot() { return _root; } // Get the starting point for a scene traversal

    void PrintScene(); // for debug purposes -- display the loaded structure

private:
    friend class SceneLoader;
    SceneInstance *_root; // The starting point for traversing the scene DAG
    SceneLoader *_loader; // Loader handles the file reading bits

    // private copy constructor and assignment operator to avoid copying this data
    Scene(const Scene&);
    Scene& operator=(const Scene&);
};

#endif /* SCENE_H_ */
