/*
 * World.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 * (edited by jima)
 */

#include "BoundingBox.h"
#include <algorithm>
#include <limits>

bool sortSphereX(Sphere &sph1, Sphere &sph2) {
	double coord1 = sph1.getCenter()[0];
	double coord2 = sph2.getCenter()[0];
	return (coord1 < coord2);
}

bool sortSphereY(Sphere &sph1, Sphere &sph2) {
	double coord1 = sph1.getCenter()[1];
	double coord2 = sph2.getCenter()[1];
	return (coord1 < coord2);
}

bool sortSphereZ(Sphere &sph1, Sphere &sph2) {
	double coord1 = sph1.getCenter()[2];
	double coord2 = sph2.getCenter()[2];
	return (coord1 < coord2);
}

BoundingBox::BoundingBox() {
	_leftChild = NULL;
	_rightChild = NULL;
}

BoundingBox::BoundingBox(vector<Sphere> sph, int axis) {

	upperCorner = vec3(-1*numeric_limits<double>::infinity());
	lowerCorner = vec3(numeric_limits<double>::infinity());

	// calculate axis-aligned bounds
	for (vector<Sphere>::iterator it = sph.begin(); it != sph.end(); it++) {
		vec4 center = it->getCenter();
		vec3 sphereUpperCorner, sphereLowerCorner;
		it->getAxisAlignedBounds(sphereUpperCorner, sphereLowerCorner);
		if (lowerCorner[0] > sphereLowerCorner[0]) lowerCorner[0] = sphereLowerCorner[0];
		if (lowerCorner[1] > sphereLowerCorner[1]) lowerCorner[1] = sphereLowerCorner[1];
		if (lowerCorner[2] > sphereLowerCorner[2]) lowerCorner[2] = sphereLowerCorner[2];
		if (upperCorner[0] < sphereUpperCorner[0]) upperCorner[0] = sphereUpperCorner[0];
		if (upperCorner[1] < sphereUpperCorner[1]) upperCorner[1] = sphereUpperCorner[1];
		if (upperCorner[2] < sphereUpperCorner[2]) upperCorner[2] = sphereUpperCorner[2];
	}

	// set up child bounding boxes or set this up as a leaf node
	if (sph.size() == 1) {
		_spheres.push_back(sph[0]);
		_rightChild = NULL;
		_leftChild = NULL;
		if (_DEBUG_INIT) {
			vec4 center = sph[0].getCenter();
			cout << "reached leaf with center " << center[0] << "," << center[1] << "," << center[2];
			cout << "; radius " << sph[0].getRadius() << endl;
		}
	}
	else {
		if (axis == 0)
			sort(sph.begin(), sph.end(), sortSphereX);
		else if (axis == 1)
			sort(sph.begin(), sph.end(), sortSphereY);
		else
			sort(sph.begin(), sph.end(), sortSphereZ);
		int cutoff = sph.size()/2;
		vector<Sphere> leftSpheres (sph.begin(), sph.begin() + cutoff);
		vector<Sphere> rightSpheres (sph.begin() + cutoff, sph.end());
		_leftChild = new BoundingBox(leftSpheres, (axis+1)%3);
		_rightChild = new BoundingBox(rightSpheres, (axis+1)%3);
	}
	if (_DEBUG_INIT) {
		cout << "BoundingBox created with lower corner " << lowerCorner[0] << "," << lowerCorner[1] << "," << lowerCorner[2];
		cout << " and upper corner " << upperCorner[0] << "," << upperCorner[1] << "," << upperCorner[2] << endl;
	}
}

BoundingBox::~BoundingBox() {
	delete _leftChild;
	delete _rightChild;
}

bool BoundingBox::intersect(Ray & r, double & bestT, vec3 &outn, MaterialInfo &outm) {
	double enterX, enterY, enterZ, exitX, exitY, exitZ;

	// handle leaf
	if (_spheres.size() > 0) {
		for (vector<Sphere>::iterator sphere = _spheres.begin(); sphere != _spheres.end(); sphere++) {
			double intersect = sphere->intersect(r);
			if (intersect < bestT) {
				//cout << "intersect found" << endl;
				bestT = intersect;
				//cout << intersect << " ";
				vec4 pos = r.getPos(intersect);
				//cout << pos[0] << "," << pos[1] << "," << pos[2] << " ";
				outn = vec3(sphere->calculateNormal(pos), VW);
				//cout << outn[0] << "," << outn[1] << "," << outn[2] << endl;
				outm = sphere->getMaterial();
			}
		}
		return bestT < numeric_limits<double>::infinity();
	}

	// find x-coordinates
	double x1 = (lowerCorner[0] - r.start()[0])/r.direction()[0];
	double x2 = (upperCorner[0] - r.start()[0])/r.direction()[0];
	if (x1 <= 0 && x2 <= 0) return false;
	else if (min(x1, x2) <= 0 && max(x1, x2) > 0) enterX = 0.0, exitX = max(x1, x2);
	else enterX = min(x1, x2), exitX = max(x1, x2);
	// find y-coordinates
	double y1 = (lowerCorner[1] - r.start()[1])/r.direction()[1];
	double y2 = (upperCorner[1] - r.start()[1])/r.direction()[1];
	if (y1 <= 0 && y2 <= 0) return false;
	else if (min(y1, y2) <= 0 && max(y1, y2) > 0) enterY = 0.0, exitY = max(y1, y2);
	else enterY = min(y1, y2), exitY = max(y1, y2);
	// find z-coordinates
	double z1 = (lowerCorner[2] - r.start()[2])/r.direction()[2];
	double z2 = (upperCorner[2] - r.start()[2])/r.direction()[2];
	if (z1 <= 0 && z2 <= 0) return false;
	else if (min(z1, z2) <= 0 && max(z1, z2) > 0) enterZ = 0.0, exitZ = max(z1, z2);
	else enterZ = min(z1, z2), exitZ = max(z1, z2);

	// if overlap, return false
	if (min(min(exitX, exitY), exitZ) <= max(max(enterX, enterY), enterZ))
		return false;
	
	bool bFound = _leftChild->intersect(r, bestT, outn, outm);
	bFound = _rightChild->intersect(r, bestT, outn, outm) || bFound;
	return bFound;
}

