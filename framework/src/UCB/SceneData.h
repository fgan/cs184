/*
 * SceneData.h
 *
 *  These classes are used internally to wrap ParametricValues
 *  The Scene 'Group' and 'Instance' classes will not return them to users of the class.
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef SCENEDATA_H_
#define SCENEDATA_H_

#include "ParametricValue.h"
#include "SceneInfo.h"
#include <vector>

class Transform {
    friend class SceneLoader;

public:
    virtual mat4 getMatrix(int time) = 0; // pure virtual function
    virtual ~Transform() {}
};

class GeneralTransform : public Transform {
    vector<ParametricValue*> matrix;
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        mat4 out = 0;
        if (matrix.size() == 9) // 2d matrix
        {
            out[2][2] = 1;
            for (int i = 0; i < 3; i++)
            {
                int i_ind = i < 2 ? i : 3;

                for (int j = 0; j < 3; j++)
                {
                    int j_ind = j < 2 ? j : 3;

                    out[i_ind][j_ind] = matrix[i * 3 + j]->getValue(time);
                }
            }
        } else if (matrix.size() == 16) {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    out[i][j] = matrix[i*4 + j]->getValue(time);
                }
            }
        } else {
            out = identity3D();
        }
        return out;
    }

    ~GeneralTransform()
    {
        for (vector<ParametricValue*>::iterator it = matrix.begin(); it != matrix.end(); ++it)
        {
            delete *it;
        }
    }

};

class Translate : public Transform {
    ParametricValue *translate[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 tr;
        tr[0] = translate[0]->getValue(time);
        tr[1] = translate[1]->getValue(time);
        if (translate[2] == NULL)
            tr[2] = 0;
        else
            tr[2] = translate[2]->getValue(time);
        return translation3D(tr);
    }

    Translate() {
        for (int i = 0; i < 3; i++)
            translate[i] = NULL;
    }

    ~Translate() {
        for (int i = 0; i < 3; i++)
            delete translate[i];
    }
};

class Scale : public Transform {
    ParametricValue *scale[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 sc;
        sc[0] = scale[0]->getValue(time);
        sc[1] = scale[1]->getValue(time);
        if (scale[2] == NULL)
            sc[2] = 1;
        else
            sc[2] = scale[2]->getValue(time);
        return scaling3D(sc);
    }

    Scale() {
        for (int i = 0; i < 3; i++)
            scale[i] = NULL;
    }

    ~Scale() {
        for (int i = 0; i < 3; i++)
            delete scale[i];
    }
};

class Rotate : public Transform {
    ParametricValue *angle;
    ParametricValue *axis[3];
    friend class SceneLoader;

public:
    mat4 getMatrix(int time) {
        vec3 ax;
        if (axis[2] == NULL) // 2D
        {
            ax = vec3(0,0,1);
        } else {
            ax = vec3(axis[0]->getValue(time),
                            axis[1]->getValue(time),
                                axis[2]->getValue(time));
        }
        return rotation3D(ax, angle->getValue(time));
    };

    Rotate()
    {
        angle = NULL;
        for (int i = 0; i < 3; i++)
            axis[i] = NULL;
    }

    ~Rotate() {
        delete angle;
        for (int i = 0; i < 3; i++)
            delete axis[i];
    }
};

class ParametricColor {
    ParametricValue *_color[3];
    friend class SceneLoader;

public:
    vec3 getColor(int time) {
        return vec3(_color[0]->getValue(time),
                _color[1]->getValue(time),
                    _color[2]->getValue(time));
    }

    ParametricColor()
    {
        for (int i = 0; i < 3; i++)
            _color[i] = NULL;
    }

    ~ParametricColor() {
        for (int i = 0; i < 3; i++)
            delete _color[i];
    }
};

class LOD {
    ParametricValue *_level;
    friend class SceneLoader;

public:
    int getLod(int time) { return int(_level->getValue(time)); }

    LOD() : _level(NULL) {}

    ~LOD() {
        delete _level;
    }
};

class ParametricMaterial {
    ParametricColor *_vec3;
    ParametricValue *_coefficients[7];
    friend class SceneLoader;

public:
    MaterialInfo getMaterial(int time) { return MaterialInfo(_vec3->getColor(time),
                                                     _coefficients[0]->getValue(time),
                                                     _coefficients[1]->getValue(time),
                                                     _coefficients[2]->getValue(time),
                                                     _coefficients[3]->getValue(time),
                                                     _coefficients[4]->getValue(time),
                                                     _coefficients[5]->getValue(time),
                                                     _coefficients[6]->getValue(time)); }
    ParametricMaterial() : _vec3(NULL) {
        for (int i = 0; i < 7; i++)
            _coefficients[i] = 0;
    }

    ~ParametricMaterial() {
        delete _vec3;
        for (int i = 0; i < 7; i++)
            delete _coefficients[i];
    }
};


class ParametricSphere {
    ParametricValue *_radius;
	ParametricMaterial *_material;
    friend class SceneLoader;

public:
    double getRadius(int time) { return _radius->getValue(time); }
	MaterialInfo getMaterial(int time) { return _material->getMaterial(time); }

    ParametricSphere() : _radius(NULL), _material(NULL) {}

    ~ParametricSphere() {
        delete _radius;
    }
};


class ParametricCamera {
    ParametricValue *_perspective;
    ParametricValue *_frustum[6];
    friend class SceneLoader;

public:
    CameraInfo getCamera(int time) { return CameraInfo((int)_perspective->getValue(time),
                                                  _frustum[0]->getValue(time),
                                                  _frustum[1]->getValue(time),
                                                  _frustum[2]->getValue(time),
                                                  _frustum[3]->getValue(time),
                                                  _frustum[4]->getValue(time),
                                                  _frustum[5]->getValue(time)); }

	ParametricCamera() : _perspective(NULL) {
		for (int i = 0; i < 6; i++) {
			_frustum[i] = NULL;
		}
	}

	~ParametricCamera() {
		delete _perspective;
		for (int i = 0; i < 6; i++) {
			delete _frustum[i];
		}
	}
};

class ParametricLight {
    ParametricValue *_type;
    ParametricColor *_color;
    ParametricValue *_falloff;
    ParametricValue *_angularFalloff;
    ParametricValue *_deadDistance;
    friend class SceneLoader;

public:
    ParametricLight() : _type(NULL), _color(NULL), _falloff(NULL), _angularFalloff(NULL), _deadDistance(NULL) {}
    ~ParametricLight() {
        delete _type;
        delete _color;
        delete _falloff;
        delete _angularFalloff;
        delete _deadDistance;
    }

    LightInfo getLight(int time) {
        return LightInfo((int)_type->getValue(time), 
                _color->getColor(time), _falloff->getValue(time), 
                _angularFalloff->getValue(time), _deadDistance->getValue(time));
    }
};


#endif /* SCENEDATA_H_ */

