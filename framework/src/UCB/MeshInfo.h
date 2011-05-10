/*
 * MeshInfo.h
 *
 * Basic mesh data
 *
 *  Created on: Feb 19, 2009
 *      Author: jima
 */

#ifndef MESH_INFO_H_
#define MESH_INFO_H_

#include "algebra3.h"
#include "UCBglobal.h"
#include <vector>

struct OBJVertex {
    vec3 pos;

    OBJVertex(vec3 pos) : pos(pos) {}
};

struct OBJTriangle {
    int ind[3];

    OBJTriangle(int i, int j, int k) {
        ind[0] = i; ind[1] = j; ind[2] = k;
    }
};

struct OBJTriangleMesh {
    std::vector<OBJTriangle*> triangles;
    std::vector<OBJVertex*> vertices;

    OBJTriangleMesh() {}
    OBJTriangleMesh(string file) {
        loadFile(file);
    }
    ~OBJTriangleMesh() { 
        clear(); 
    }

    bool loadFile(string file);
    void clear();
};

#endif // MESH_INFO_H_

