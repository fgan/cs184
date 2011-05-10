/*
 * World.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 * (edited by jima)
 */

#include "World.h"

#include <limits>

World::World() {
	loadScene("scene.scd");
}

World::World(string filename) {
	loadScene(filename);
}

World::~World() {
	delete _bb;
	delete scene;
}

bool World::intersect(Ray & r, double & bestT, vec3 &outn, MaterialInfo &outm) {

	bestT = numeric_limits<double>::infinity();

	if (_DEBUG_INTERSECT1
		&& abs(r.direction()[0] - 0.146734796) < 0.05
		&& abs(r.direction()[1] + 0.146734796) < 0.05
		&& abs(r.direction()[2] + 0.978231971) < 0.05)
		int i = 0; //debug statement, stops at a ray aimed at the center of one of the spheres in threespheres.scd
	if (_DEBUG_INTERSECT2
		&& abs(r.direction()[0] + 0.114776942) < 0.02
		&& abs(r.direction()[1] - 0.0619082097) < 0.02
		&& abs(r.direction()[2] + 0.991460351) < 0.02)
		int i = 0; // debug statement, stops at a ray aimed for the left eye of the bunny in ellipsoids.scd


	if (_ASSIGNMENT <= 5 || _FINAL_PROJ) {
		// iterate through spheres to see if any of them are intersected
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
				//AS4 stuff
				//if (sphere == _spheres.begin()) {
				//	outm.k[MAT_KSM] *= ksmMod;
				//	outm.k[MAT_KSP] *= kspMod;
				//	if (outm.k[MAT_KSP] < 1.0) outm.k[MAT_KSP] = 1.0;
				//}
			}
		}

		for (vector<Cube>::iterator cube = _cubes.begin(); cube != _cubes.end(); cube++) {
			double intersect = cube->intersect(r);
			if (intersect < bestT) {
				bestT = intersect;
				vec4 pos = r.getPos(intersect);
				outn = vec3(cube->calculateNormal(pos), VW);
				outm = cube->getMaterial();
				outm.color = cube->calculateColor(pos);
			}
		}

		return bestT < numeric_limits<double>::infinity();
	}
	else
		return _bb->intersect(r, bestT, outn, outm);
}

void World::loadScene(string filename) {
	// for as4, you can optionally hard-code the scene.  For as5 and as6 it must be loaded from a file.
	
	if (_FINAL_PROJ) {
		vec4 eye(0.0, 0.0, 0, 1.0);
		vec4 LL(-1.0, -1.0, -3.0, 1.0);
		vec4 UL(-1.0, 1.0, -3.0, 1.0);
		vec4 LR(1.0, -1.0, -3.0, 1.0);
		vec4 UR(1.0, 1.0, -3.0, 1.0);

		//_view = Viewport(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
		_view = Viewport(eye, IMAGE_WIDTH, IMAGE_HEIGHT, Lens());

		_lights[LIGHT_DIRECTIONAL].push_back(
								Light(0, vec3(0.5,0.5,-0.5),
								LightInfo(LIGHT_DIRECTIONAL, vec3(.4, .8, 1.2))));
		_lights[LIGHT_POINT].push_back(
								Light(vec3(0.0,0.0,-14.0), vec3(0.5,0.5,-0.5),
								LightInfo(LIGHT_POINT, vec3(1.39, 0.2, 0.2))));

		_ambientLight = vec3(.5,.2,.2);
		/*
		_spheres.push_back(Sphere(vec4(-2.5,-1.5,-17.0,1.0), 2.0,
							MaterialInfo(vec3(.4, .5, .9), 
							.1, .5, .5, 150, 1.0)));
		_spheres.push_back(Sphere(vec4(0.0,4.0,-25.0,1.0), 2.5,
							MaterialInfo(vec3(.9, .4, .5),
							.4, .2, .5, 20, 0.0)));
		_spheres.push_back(Sphere(vec4(1.5,-1.5,-10.0,1.0), 1.0,
							MaterialInfo(vec3(.5, .9, .4),
							.5, .5, .3, 4, .5)));
		*/

		_cubes.push_back(Cube(vec4(-3.5,-3.5,-35.0,1.0), 1.5, MaterialInfo(vec3(.4, .5, .9), 
							.1, .5, .5, 150, 1.0)));

		_cubes.push_back(Cube(vec4(2.0, 1.5, -30,1.0), 1.5, MaterialInfo(vec3(.9, .4, .5),
							.4, .2, .5, 20, 0.0)));

		_cubes.push_back(Cube(vec4(-3.5,4,-120.0,1.0), 3, MaterialInfo(vec3(.5, .9, .4),
							.5, .5, .3, 4, .5)));

		_cubes.push_back(Cube(vec4(3.5,-4,-250.0,1.0), 3.5, MaterialInfo(vec3(.5, .9, .4),
							.5, .5, .3, 4, .5)));

		ksmMod = kspMod = 1.0;
	}

	else if (_ASSIGNMENT >= 5) {
		scene = new Scene(filename);

		loadInstance(scene->getRoot());

		if (_ASSIGNMENT >= 6)
			_bb = new BoundingBox(_spheres, 0);
	}
	
	if (_ASSIGNMENT <= 4) {
		//vec4 eye(0.0, 0.0, 0, 1.0);
		//vec4 LL(-1.0, -1.0, -3.0, 1.0);
		//vec4 UL(-1.0, 1.0, -3.0, 1.0);
		//vec4 LR(1.0, -1.0, -3.0, 1.0);
		//vec4 UR(1.0, 1.0, -3.0, 1.0);

		//_view = Viewport(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT);

		//_lights[LIGHT_DIRECTIONAL].push_back(
		//						Light(0, vec3(0.5,0.5,-0.5),
		//						LightInfo(LIGHT_DIRECTIONAL, vec3(.4, .8, 1.2))));
		//_lights[LIGHT_POINT].push_back(
		//						Light(vec3(0.0,0.0,-14.0), vec3(0.5,0.5,-0.5),
		//						LightInfo(LIGHT_POINT, vec3(1.39, 0.2, 0.2))));

		//_ambientLight = vec3(.5,.2,.2);

		//_spheres.push_back(Sphere(vec4(-2.5,-1.5,-17.0,1.0), 2.0,
		//					MaterialInfo(vec3(.4, .5, .9), 
		//					.1, .5, .5, 150, 1.0)));
		//_spheres.push_back(Sphere(vec4(0.0,4.0,-25.0,1.0), 2.5,
		//					MaterialInfo(vec3(.9, .4, .5),
		//					.4, .2, .5, 20, 0.0)));
		//_spheres.push_back(Sphere(vec4(1.5,-1.5,-10.0,1.0), 1.0,
		//					MaterialInfo(vec3(.5, .9, .4),
		//					.5, .5, .3, 4, .5)));

		//ksmMod = kspMod = 1.0;
	}
}

void World::loadInstance(SceneInstance *si) {

	// Compute transform information. Should add a mat4 even if there is no transform.
	mat4 mTransform;
	if (!si->computeTransform(mTransform)) {
		mTransform = identity3D(); // if there is no transform, push an identity matrix.
	}
	if (!transformStack.empty() && transformStack.top() != identity3D()) {
		mat4 mPreviousTransform = transformStack.top();
		mTransform =  mPreviousTransform * mTransform;
	}
	transformStack.push(mTransform);
	
	SceneGroup *sg = si->getChild();
	
	// Get camera info, if any.
	CameraInfo camInfo;
	if (sg->computeCamera(camInfo)) {
		vec4 eye(0.0, 0.0, 0.0, 1.0);
		double left = camInfo.sides[FRUS_LEFT];
		double right = camInfo.sides[FRUS_RIGHT];
		double top = camInfo.sides[FRUS_TOP];
		double bottom = camInfo.sides[FRUS_BOTTOM];
		double depth = -1*camInfo.sides[FRUS_NEAR];
		vec4 LL(left, bottom, depth, 1.0);
		vec4 UL(left, top, depth, 1.0);
		vec4 LR(right, bottom, depth, 1.0);
		vec4 UR(right, top, depth, 1.0);
		eye = mTransform * eye;
		LL = mTransform * LL;
		UL = mTransform * UL;
		LR = mTransform * LR;
		UR = mTransform * UR;
		//_view = Viewport(eye, LL, UL, LR, UR, IMAGE_WIDTH, IMAGE_HEIGHT, camInfo.perspective);
		_view = Viewport(eye, IMAGE_WIDTH, IMAGE_HEIGHT, Lens());
	}

	// Compute light info, if any.
	LightInfo li;
	if (sg->computeLight(li)) {
		vec3 direction, position;
		switch (li.type) {
			case LIGHT_DIRECTIONAL:
			case LIGHT_POINT:
			case LIGHT_SPOT:
				direction = vec3(-1*mTransform[0][2],-1*mTransform[1][2],-1*mTransform[2][2]);
				position = mTransform * vec3(0.0);
				_lights[li.type].push_back(Light(position, direction, li));
				break;
			case LIGHT_AMBIENT:
				_ambientLight = li.color;
				break;
		}	
	}

	// Computes sphere info, if any.
	double radius; MaterialInfo matInfo;
	if (sg->computeSphere(radius, matInfo, 0)) {
		_spheres.push_back(Sphere(vec4(0.0), radius, matInfo, mTransform));	
	}

	// Recursively parse scene.
	for (int i = 0; i < sg->getChildCount(); i++) {
		loadInstance(sg->getChild(i));
	}

	// Pops the transformation matrix of this SceneGroup. This function should have added this matrix at the beginning.
	transformStack.pop();
}

void World::as4Edit(double xcoord, double ycoord) {
	double width = (double)(this->_view.getW());
	double height = (double)(this->_view.getH());
	ksmMod = xcoord/width;
	kspMod = (height - ycoord)/height;
}



