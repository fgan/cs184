/*
 * Viewport.h
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "global.h"
#include "Lens.h"

/**
 * Viewport holds all the information about our camera.
 * This includes how to sample across the viewport. This code should not
 * need to change.
 */
class Viewport {
public:
	Viewport() {}
    //Viewport(vec4 eye, vec4 LL, vec4 UL, vec4 LR, vec4 UR, int width, int height, int perspective);
	Viewport::Viewport(vec4 eye, int width, int height, Lens l);
	~Viewport() {}

    /**
     * This will reset the sampler back to its initial state.
     */
    void resetSampler();
    /**
     * This will get the next sample of the viewport.
     * It provides coordinates of the pixel to sample, and a ray through that pixel
	 * Returns false after returning all samples on the screen
     * */
    bool getSample(vec2 &p, Ray &r);
	bool getSamples(vec2 &p, vector<Ray> &r);

	inline int getW() { return _pixelsWide; }
	inline int getH() { return _pixelsHigh; }
	inline void setRaysPerPixel(int rpp) { _raysPerPixel = rpp; }

	// if the viewport is transformed in space, use this to transform rays out
	inline const mat4& getViewToWorld() { return _viewToWorld; }
	inline void setViewToWorld(mat4 xform) { _viewToWorld = xform; }

private:
    vec4 _eye;
    vec4 _LL;
    vec4 _UL;
    vec4 _LR;
    vec4 _UR;

    int _pixelsWide;
    int _pixelsHigh;
    double _i;
    double _j;

	int _perspective;

    int _percentage;

	int _raysPerPixel;
	int _incPP;
	mat4 _viewToWorld;

	static const int _RPP = 25; // rays per pixel

	Lens _lens;
};

#endif /* VIEWPORT_H_ */

