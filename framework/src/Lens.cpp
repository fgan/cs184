#include "Lens.h"
#include <limits>

Lens::Lens() {

	initializeLens();
	//_apertureRadius = .342;

}

void Lens::initializeLens(string filename) {

	//half of lens farthest from film
	Sphere lens1(vec4(0,0,.2518,1), .5895, MaterialInfo(0,0,0,0,1,1.67),identity3D()); //lens surface nearest object space
	Sphere lens2(vec4(0,0,1.4341,1), 1.6966, MaterialInfo(0,0,0,0,1,1),identity3D());
	Sphere lens3(vec4(0,0,.1254,1), .3855, MaterialInfo(0,0,0,0,1,1.67),identity3D());
	Sphere lens4(vec4(0,0,.6358,1), .8154, MaterialInfo(0,0,0,0,1,1.699),identity3D());
	Sphere lens5(vec4(0,0,.1409,1), .255, MaterialInfo(0,0,0,0,1,1),identity3D());

	//half of lens nearest to film
	Sphere lens6(vec4(0,0,-.1999,1), .2899, MaterialInfo(0,0,0,0,1,1.603),identity3D());
	Sphere lens7(vec4(0,0,.929,1), .8154, MaterialInfo(0,0,0,0,1,1.658),identity3D());
	Sphere lens8(vec4(0,0,-.1728,1), .4077, MaterialInfo(0,0,0,0,1,1),identity3D());
	Sphere lens9(vec4(0,0,8.98,1), 8.74130, MaterialInfo(0,0,0,0,1,1.717),identity3D());
	Sphere lens10(vec4(0,0,-.4915,1), .7946, MaterialInfo(0,0,0,0,1,1),identity3D()); //lens surface farthest object space

	_lens.push_back(lens10); //push in reverse order to get order right
	_lens.push_back(lens9);
	_lens.push_back(lens8);
	_lens.push_back(lens7);
	_lens.push_back(lens6);
	_lens.push_back(lens5);
	_lens.push_back(lens4);
	_lens.push_back(lens3);
	_lens.push_back(lens2);
	_lens.push_back(lens1);
}

bool Lens::traceFirstHalf(Ray & r, Ray & exitRay, vec2 & offset) {

	vector<Sphere>::iterator lens = _lens.begin();
	Ray traceray = r;
	double oldIndex = 1.0;
	bool isSuccess = true;

	for(int i=0; i<5; i++) {

		double intersect = lens->intersect(traceray);
		if(intersect < numeric_limits<double>::infinity()) {
			vec4 pos = traceray.getPos(intersect);
			vec3 normal = vec3(lens->calculateNormal(pos), VW);
			double newIndex = lens->getRefractiveIndex();
			if(!refractRay(traceray, normal, oldIndex, newIndex, pos)) {
				isSuccess = false;
			}
			lens++;
		}
		else {
			cout << "failed to hit lens" << endl;
			return false;
		}
	}

	double currentZPos = traceray.getPos(0)[VZ];
	double t = traceray.getPos(0)[VZ] / traceray.direction()[VZ];

	vec4 exitRayPosition(traceray.getPos(-t));

	double xoffset = exitRayPosition[VX];
	double yoffset = exitRayPosition[VY];

//	if(xoffset * xoffset + yoffset * yoffset <= _apertureRadius * _apertureRadius)
	if(xoffset * xoffset + yoffset * yoffset > .342 * .342) {
		isSuccess = false;
	}
	offset = vec2(xoffset,yoffset);
	
	exitRay = Ray(exitRayPosition, traceray.direction(), 0);

	return isSuccess;
}

bool Lens::refractRay(Ray & r, vec3 normal, double n1, double n2, vec4 pos) {

	normal.normalize();
	vec3 ray = -1 * vec3(r.direction(), VW);
	ray.normalize();

	double sin1 = 1 - (abs(ray*normal));

	double sin2 = sin1*n1/n2;

	if(sin2 > 1 || sin2 < -1)
		return false;

	else {
		double cos1 = ray*normal;
		double cos2 = sqrt(1-(pow(n1/n2,2)*(1-pow(cos1,2))));
		vec4 newDir;
		if(ray * normal > 0)
			newDir = (n1/n2)*-1*ray + ((n1/n2)*cos1 - cos2)*normal;
		else
			newDir = (n1/n2)*-1*ray + ((n1/n2)*cos1 + cos2)*normal;

		r = Ray(pos,newDir,0);

		return true;
	}

}

bool Lens::raytrace(Ray & r, Ray & exitRay) {


	//assumes parameter ray can go through aperture

	Ray tempRay = r;
	double oldIndex = 1.0;
	for(vector<Sphere>::iterator lens = _lens.begin(); lens != _lens.end(); lens++) {

		double intersect = lens->intersect(tempRay);
		if(intersect < numeric_limits<double>::infinity()) {
			vec4 pos = tempRay.getPos(intersect);
			vec3 normal = vec3(lens->calculateNormal(pos), VW);
			double newIndex = lens->getRefractiveIndex();
			if(refractRay(tempRay, normal, oldIndex, newIndex, pos)) {
				oldIndex = newIndex;
			}

			else
				return false;
			}

		else
		return false;

	}

	exitRay = tempRay;

	return true;
}


Ray Lens::getRays(vec4 & point, Ray & r) {
	double epsilon = 0.01;
	bool isFirstCheck = true;
	vec2 nextDistance;
	vec4 nextOffset = vec4(0.0, 0.0, 0.0, 0.0);
	Ray nextRay = Ray(point, nextOffset, 0.001);
	Ray upperRay, lowerRay;
	vec2 upperDistance, lowerDistance;
	vec4 upperRayOffset, lowerRayOffset;

	traceFirstHalf(nextRay, r, nextDistance);
	if (nextDistance.length() < epsilon) {
		raytrace(nextRay, r);
		return r;
	}

	upperRay = nextRay;
	upperRayOffset = nextOffset;
	upperDistance = nextDistance;
	vec4 deltaOffset = vec4(upperDistance[0], upperDistance[1], 0.0, 0.0);
	int counter = 0;

	while (true) {
		if (DEBUG_FLAG) {
			counter++;
			cout << counter << endl;
		}

		nextOffset = nextOffset - deltaOffset;
		nextRay = Ray(point, nextOffset, 0.001);
		traceFirstHalf(nextRay, r, nextDistance);
		if (nextDistance.length() < epsilon) {
			raytrace(nextRay, r);
			return r;
		}

		vec4 sumDistance = nextDistance + upperDistance;
		if (sumDistance.length() > nextDistance.length() && sumDistance.length() > upperDistance.length()) {
			// still on the same side
			if (nextDistance.length() > upperDistance.length()) {
				// this should be an error
				cout << "error in getRays algorithm" << endl;
			} else {
				upperRay = nextRay;
				upperRayOffset = nextOffset;
				upperDistance = nextDistance;
			}
		} else {
			// on other side
			lowerRay = nextRay;
			lowerRayOffset = nextOffset;
			lowerDistance = nextDistance;
			break;
		}
	}

	while (true) {
		double pUpper = lowerDistance.length() / (upperDistance.length() + lowerDistance.length());
		nextOffset = pUpper * upperRayOffset + (1.0 - pUpper) * lowerRayOffset;
		nextRay = Ray(point, nextOffset, 0.001);
		traceFirstHalf(nextRay, r, nextDistance);
		if (nextDistance.length() < epsilon) {
			raytrace(nextRay, r);
			return r;
		}

		vec4 sumUpper = nextDistance + upperDistance;
		if (sumUpper.length() > nextDistance.length() && sumUpper.length() > upperDistance.length()) {
			if (nextDistance.length() > upperDistance.length()) {
				cout << "error in search algorithm of getRays" << endl;
			}
			upperRay = nextRay;
			upperDistance = nextDistance;
			upperRayOffset = nextOffset;
		} else {
			if (nextDistance.length() > lowerDistance.length()) {
				cout << "error in search algorithm of getRays" << endl;
			}
			lowerRay = nextRay;
			lowerDistance = nextDistance;
			lowerRayOffset = nextOffset;
		}
	}
}

