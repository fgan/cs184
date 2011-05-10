/*
 * Primitive.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Primitives.h"

#include <limits>


Sphere::Sphere(vec4 center, double radius, MaterialInfo m, mat4 sphereToWorldMatrix) {
    _p = center;
	_p[3] = 1.0;
    _r = radius;
	_m = m;
	_sphereToWorld = sphereToWorldMatrix;
	_worldToSphere = sphereToWorldMatrix.inverse();
	_sphereNormToWorldNorm = sphereToWorldMatrix.inverse().transpose();

	// calculate axis aligned bounds once
	vec4 v1 = _sphereToWorld * vec4(_r, _r, _r, 1.0);
	vec4 v2 = _sphereToWorld * vec4(_r, _r, -1*_r, 1.0);
	vec4 v3 = _sphereToWorld * vec4(_r, -1*_r, _r, 1.0);
	vec4 v4 = _sphereToWorld * vec4(_r, -1*_r, -1*_r, 1.0);
	vec4 v5 = _sphereToWorld * vec4(-1*_r, _r, _r, 1.0);
	vec4 v6 = _sphereToWorld * vec4(-1*_r, _r, -1*_r, 1.0);
	vec4 v7 = _sphereToWorld * vec4(-1*_r, -1*_r, _r, 1.0);
	vec4 v8 = _sphereToWorld * vec4(-1*_r, -1*_r, -1*_r, 1.0);
	_upperAxisAlignedBound[0] = max(v1[0],max(v2[0],max(v3[0],max(v4[0],max(v5[0],max(v6[0],max(v7[0],v8[0])))))));
	_upperAxisAlignedBound[1] = max(v1[1],max(v2[1],max(v3[1],max(v4[1],max(v5[1],max(v6[1],max(v7[1],v8[1])))))));
	_upperAxisAlignedBound[2] = max(v1[2],max(v2[2],max(v3[2],max(v4[2],max(v5[2],max(v6[2],max(v7[2],v8[2])))))));
	_lowerAxisAlignedBound[0] = min(v1[0],min(v2[0],min(v3[0],min(v4[0],min(v5[0],min(v6[0],min(v7[0],v8[0])))))));
	_lowerAxisAlignedBound[1] = min(v1[1],min(v2[1],min(v3[1],min(v4[1],min(v5[1],min(v6[1],min(v7[1],v8[1])))))));
	_lowerAxisAlignedBound[2] = min(v1[2],min(v2[2],min(v3[2],min(v4[2],min(v5[2],min(v6[2],min(v7[2],v8[2])))))));
}

//Checks for intersection with the given ray. Ray is in world coordinates.
double Sphere::intersect(Ray & ray) {
	vec4 start = ray.start();
	vec4 dir = ray.direction();
	start = _worldToSphere * start;
	dir = _worldToSphere * dir;
	vec3 e = vec3(start, VW);
	vec3 d = vec3(dir, VW);
	vec3 diff = e - vec3(_p, VW);
	double discriminant = (d*diff)*(d*diff) - d.length2()*(diff.length2()-_r*_r);
	if (discriminant < 0.0)
		return numeric_limits<double>::infinity();
	else if (discriminant == 0.0)
		return d*diff*(-1.0)/d.length2();
	else { // discriminant > 0.0
		double time1 = (d*diff*(-1.0) - sqrt(discriminant))/d.length2();
		double time2 = (d*diff*(-1.0) + sqrt(discriminant))/d.length2();
		if (time1 < ray.getMinT() && time2 < ray.getMinT())
			return numeric_limits<double>::infinity();
		else if (time1 < ray.getMinT())
			return time2;
		else if (time2 < ray.getMinT())
			return time1;
		else {
			return MIN(time1, time2);
		}
	}
}

//Calculates the normal for the given position on this sphere, in world coordinates. Parameter position should be given in world coordinates as well. Returned vec4 is NOT normalized, nor is the w value set.
vec4 Sphere::calculateNormal(vec4 & position) {
	position = _worldToSphere * position;
	vec4 normal = position - _p;
	normal = _sphereNormToWorldNorm * normal;
	return normal;
}

vec4 Sphere::getCenter() {
	vec4 worldCenter = _sphereToWorld * _p;
	return worldCenter;
}

double Sphere::getRadius() {
	return _r;
}

void Sphere::getAxisAlignedBounds(vec3 & upperBound, vec3 & lowerBound) {
	upperBound = _upperAxisAlignedBound;
	lowerBound = _lowerAxisAlignedBound;
}

bool loadBitmap(string filename, FIBITMAP* &bitmap) {
    // get the file format
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(filename.c_str());
    if (format == FIF_UNKNOWN)
        return false;

    // load the image
    bitmap = FreeImage_Load(format, filename.c_str());
    if (!bitmap)
        return false;

    return true;
}

Cube::Cube(vec4 center, double sideLength, MaterialInfo m, string filename) {
	_upperCorner = vec3(center, 3) + vec3(sideLength / 2);
	_lowerCorner = vec3(center, 3) - vec3(sideLength / 2);
	_m = m;
	_sideLength = sideLength;

	//_bitmap = FreeImage_Load(FIF_BMP, filename
	loadBitmap(filename, _bitmap);

}

double Cube::intersect(Ray & r) {
	double enterX, enterY, enterZ, exitX, exitY, exitZ;

	// find x-coordinates
	double x1 = (_lowerCorner[0] - r.start()[0])/r.direction()[0];
	double x2 = (_upperCorner[0] - r.start()[0])/r.direction()[0];
	if (x1 <= 0 && x2 <= 0) return numeric_limits<double>::infinity();
	else if (min(x1, x2) <= 0 && max(x1, x2) > 0) enterX = 0.0, exitX = max(x1, x2);
	else enterX = min(x1, x2), exitX = max(x1, x2);
	// find y-coordinates
	double y1 = (_lowerCorner[1] - r.start()[1])/r.direction()[1];
	double y2 = (_upperCorner[1] - r.start()[1])/r.direction()[1];
	if (y1 <= 0 && y2 <= 0) return numeric_limits<double>::infinity();
	else if (min(y1, y2) <= 0 && max(y1, y2) > 0) enterY = 0.0, exitY = max(y1, y2);
	else enterY = min(y1, y2), exitY = max(y1, y2);
	// find z-coordinates
	double z1 = (_lowerCorner[2] - r.start()[2])/r.direction()[2];
	double z2 = (_upperCorner[2] - r.start()[2])/r.direction()[2];
	if (z1 <= 0 && z2 <= 0) return numeric_limits<double>::infinity();
	else if (min(z1, z2) <= 0 && max(z1, z2) > 0) enterZ = 0.0, exitZ = max(z1, z2);
	else enterZ = min(z1, z2), exitZ = max(z1, z2);

	// if overlap, return false
	if (min(min(exitX, exitY), exitZ) <= max(max(enterX, enterY), enterZ))
		return numeric_limits<double>::infinity();
	
	return max(max(enterX, enterY), enterZ);
}

vec4 Cube::calculateNormal(vec4 & position) {
	double dx1 = abs(_upperCorner[0] - position[0]);
	double dx2 = abs(_lowerCorner[0] - position[0]);
	double dy1 = abs(_upperCorner[1] - position[1]);
	double dy2 = abs(_lowerCorner[1] - position[1]);
	double dz1 = abs(_upperCorner[2] - position[2]);
	double dz2 = abs(_lowerCorner[2] - position[2]);
	double plane = min(dx1, min(dx2, min(dy1, min(dy2, min(dz1, dz2)))));
	if (plane == dx1) return vec4 (1, 0, 0, 1);
	if (plane == dx2) return vec4 (-1, 0, 0, 1);
	if (plane == dy1) return vec4 (0, 1, 0, 1);
	if (plane == dy2) return vec4 (0, -1, 0, 1);
	if (plane == dz1) return vec4 (0, 0, 1, 1);
	return vec4 (0, 0, -1, 1);
}

vec3 Cube::calculateColor(vec4 & position) {
	//return vec4(0);
	int width = FreeImage_GetLine(_bitmap);

	double dx1 = abs(_upperCorner[0] - position[0]);
	double dx2 = abs(_lowerCorner[0] - position[0]);
	double dy1 = abs(_upperCorner[1] - position[1]);
	double dy2 = abs(_lowerCorner[1] - position[1]);
	double dz1 = abs(_upperCorner[2] - position[2]);
	double dz2 = abs(_lowerCorner[2] - position[2]);
	double plane = min(dx1, min(dx2, min(dy1, min(dy2, min(dz1, dz2)))));

	vec3 rel_to_lower_corner = (vec3(position) - _lowerCorner) / _sideLength * 512;
	

	vec3 rel_to_upper_corner = (_upperCorner - vec3(position)) / _sideLength * 512;

	RGBQUAD color;

	if (plane == dx2) {//left face
		FreeImage_GetPixelColor(_bitmap, (int) rel_to_lower_corner[0], (int) rel_to_lower_corner[1], &color);
	}
	if (plane == dy2) {//right face
		FreeImage_GetPixelColor(_bitmap, (int)rel_to_lower_corner[0], (int)(abs(512-rel_to_lower_corner[1])), &color);
	}

	if (plane == dx1) {//right face
		FreeImage_GetPixelColor(_bitmap, (int)rel_to_upper_corner[0], (int)(abs(512-rel_to_upper_corner[1])), &color);
	}
	if (plane == dz2) {//top and back face
		FreeImage_GetPixelColor(_bitmap, (int) (abs(512-rel_to_lower_corner[0])), (int) rel_to_lower_corner[1], &color);
	}
	if (plane == dz1) {//front face
		FreeImage_GetPixelColor(_bitmap, (int) (abs(512-rel_to_upper_corner[0])), (int) (abs(512-rel_to_upper_corner[1])), &color);
	}

	FreeImage_GetPixelColor(_bitmap, (int) (abs(512-rel_to_upper_corner[0])), (int) rel_to_upper_corner[1], &color);

	return vec3(color.rgbRed/255.0,  color.rgbGreen/255.0, color.rgbBlue/255.0);
}

