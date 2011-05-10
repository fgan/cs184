/*
 * Viewport.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 */

#include "Viewport.h"
#include "Lens.h"

Viewport::Viewport(vec4 eye, int width, int height, Lens l) {
    _eye = eye;
	_UL = vec4(-0.18, 0.12, 1.02538, 0);
	_UR = vec4(0.18, 0.12, 1.02538, 0);
	_LL = vec4(-0.18, -0.12, 1.02538, 0);
	_LR = vec4(0.18, -0.12, 1.02538, 0);
    _pixelsWide = width;
    _pixelsHigh = height;
	_raysPerPixel = 1;
	_incPP = (int)sqrt((float)_raysPerPixel);
	_viewToWorld = identity3D();
	_lens = l;
}


void Viewport::resetSampler() {
    _i = _j = 1.0 / (_incPP + 1); //Starts off at the center of a pixel.
    _percentage = -1;
}

bool Viewport::getSample(vec2 &p, Ray &r) {
    _i += 1.0/_incPP;
    if (_i >= _pixelsWide) {
        _i = _incPP/2;
        _j += 1.0/_incPP;
    }
    if (_j >= _pixelsHigh) {
        resetSampler();
        return false;
    }

    p = vec2(_i,_j);

	// friendly progress counter
    int soFar = int( 100*((floor(_i)+1)*(floor(_j)+1) / (double) (_pixelsWide * _pixelsHigh)) );
    if (soFar >= _percentage+10) {
    	_percentage = soFar;
    	cout << soFar << "% Done." << endl;
    }

	double u = _i / (double) _pixelsWide;
    double v = _j / (double) _pixelsHigh;
    vec4 startPoint(((1-u)*((1-v)*_LL[0] + v*_UL[0]) + u*((1-v)*_LR[0] + v*_UR[0])),
				  ((1-u)*((1-v)*_LL[1] + v*_UL[1]) + u*((1-v)*_LR[1] + v*_UR[1])),
				  ((1-u)*((1-v)*_LL[2] + v*_UL[2]) + u*((1-v)*_LR[2] + v*_UR[2])),1);

	_lens.getRays(startPoint, r);

	return true;
}


