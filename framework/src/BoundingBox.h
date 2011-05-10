/*
 * BoundingBox.h
 *
 *  Created on: Mar 5, 2011
 *      Author: Yu Gan
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "global.h"
#include "UCB/SceneInfo.h"
#include "Primitives.h"
#include "Viewport.h"
#include "UCB/Scene.h"
#include <stack>

/**
 * The World forms a container for lights and primitives
 * in our scene.
 */
class BoundingBox {
public:
	BoundingBox::BoundingBox();
	BoundingBox(vector<Sphere> _spheres, int axis);
    ~BoundingBox();

	// recursively tests a ray for intersection with the BoundingBox, and fills in t, n, and m with info from the surface it hit
    bool intersect(Ray & r, double & t, vec3 &n, MaterialInfo &m);

private:

    vector<Sphere> _spheres; // holds the spheres of this BB if it is a leaf
	BoundingBox *_leftChild;
	BoundingBox *_rightChild;
	vec3 upperCorner, lowerCorner;

	static const bool _DEBUG_INIT = false;
};

#endif /* BOUNDINGBOX_H_ */

