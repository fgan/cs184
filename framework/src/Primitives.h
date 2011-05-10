/*
 * Primitive.h
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 * (edited by jima)
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "global.h"
#include "UCB/SceneInfo.h"

struct Light {
	Light() {}
	Light(vec3 pos, vec3 dir, LightInfo l) : _l(l), _pos(pos), _dir(dir) {}

	inline const LightInfo& getLightInfo() { return _l; }
	inline const vec3& getPosition() { return _pos; }
	inline const vec3& getDirection() { return _dir; }

private:
	LightInfo _l;
	vec3 _pos, _dir;
};

class Sphere {
public:
    Sphere(vec4 center, double radius, MaterialInfo m, mat4 sphereToWorldMatrix);

    double intersect(Ray & ray);
    vec4 calculateNormal(vec4 & position);
	inline MaterialInfo& getMaterial() { return _m; }
	double getRefractiveIndex() {return _m.k[MAT_KTN];}
	vec4 getCenter();
	double getRadius();
	void getAxisAlignedBounds(vec3 & upperBound, vec3 & lowerBound);

private:
    vec4 _p;
    double _r;
	MaterialInfo _m;
	mat4 _worldToSphere;
	mat4 _sphereToWorld;
	mat4 _sphereNormToWorldNorm;
	vec3 _upperAxisAlignedBound;
	vec3 _lowerAxisAlignedBound;

	// these won't be needed until you want to stretch the spheres:
//    mat4 _modelToWorld;
//    mat4 _worldToModel;
};

class Cube {
public:
	Cube(vec4 center, double sideLength, MaterialInfo m, string filename = "texture.bmp");

	double intersect(Ray & ray);
	vec4 calculateNormal(vec4 & position);
	vec3 calculateColor(vec4 & position);
	inline MaterialInfo& getMaterial() { return _m; }

private:
	vec3 _upperCorner;
	vec3 _lowerCorner;
	double _sideLength;
	MaterialInfo _m;
	FIBITMAP *_bitmap;
};

#endif /* PRIMITIVE_H_ */
