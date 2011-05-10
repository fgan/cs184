#ifndef LENS_H_
#define LENS_H_

#include "global.h"
#include "Primitives.h"

class Lens {
public:
	Lens();
	//Lens(string filename);
	~Lens() {};
	void initializeLens(string filename="sample");
	//void addLens(Sphere s1, Sphere s2);
	bool raytrace(Ray & r, Ray & exitRay);
	bool traceFirstHalf(Ray & r, Ray & exitRay, vec2 & offset);

	Ray getRays(vec4 & point, Ray & r);

private:
	bool refractRay(Ray & r, vec3 normal, double n1, double n2, vec4 pos);
	vector<Sphere> _lens; //every 2 spheres represent the surfaces of each lens
	double _apertureRadius; //radius of aperture
	//vector<double> _thickness; //each element represents either the thickness of the lens or the space in between lens
	static const bool DEBUG_FLAG = false;
};
#endif