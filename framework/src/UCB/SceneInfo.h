/*
 * SceneInfo.h
 *
 * These basic holder structs are used to return data to you
 *
 *  Created on: Feb 19, 2009
 *      Author: jima
 */

#ifndef SCENE_INFO_H_
#define SCENE_INFO_H_

#include "algebra3.h"

enum {
    MAT_KA = 0,     //ambient
    MAT_KD,     //diffuse reflection
    MAT_KS,     //specular reflection
    MAT_KSP,    //specular reflection phong exponent, the "roughness"
    MAT_KSM,    //metalicity,
    MAT_KT,     //transparency for refracted rays
    MAT_KTN,    //refractive index of the material
    MAT_NUM_COEFFICIENTS
}; // phong coefficients

enum { 
    FRUS_LEFT, FRUS_RIGHT, FRUS_BOTTOM, FRUS_TOP, FRUS_NEAR, FRUS_FAR
}; // frustum sides

enum {
    LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT, LIGHT_AMBIENT
}; // light types


struct MaterialInfo {
    double k[MAT_NUM_COEFFICIENTS]; //Storage for the material coefficients
    vec3 color;

    MaterialInfo() {
		color = vec3(0);
		for (int i = 0; i < MAT_NUM_COEFFICIENTS; i++) k[i] = 0;
	}
    MaterialInfo(vec3 color, const double ka, const double kd, const double ks, 
			const double ksp, const double ksm, const double kt = 0, const double ktn = 1)
        : color(color)
    {
        k[MAT_KA] = ka;
        k[MAT_KD] = kd;
        k[MAT_KS] = ks;
        k[MAT_KSP] = ksp;
        k[MAT_KSM] = ksm;
        k[MAT_KT] = kt;
        k[MAT_KTN] = ktn;
    }
};


struct LightInfo {
    int type;
    vec3 color;
    double falloff;
    double angularFalloff;
    double deadDistance;

    LightInfo() {}
    LightInfo(int type, vec3 color, double falloff = 2, double angularFalloff = .1, double deadDistance = .1)
        : type(type), color(color), falloff(falloff), angularFalloff(angularFalloff), deadDistance(deadDistance)
    {}
};

struct CameraInfo {
    int perspective;
    double sides[6];

    CameraInfo() {}
    CameraInfo(int perspective, double l, double r, double b, double t, double n=1, double f=100)
        : perspective(perspective)
    {
        sides[FRUS_LEFT] = l;
        sides[FRUS_RIGHT] = r;
        sides[FRUS_BOTTOM] = b;
        sides[FRUS_TOP] = t;
        sides[FRUS_NEAR] = n;
        sides[FRUS_FAR] = f;
    }
};


#endif /* SCENE_INFO_H_ */

