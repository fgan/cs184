#include "main.h"
#include <limits>
#include <cmath>

using namespace std;

//****************************************************
// Global Variables
//****************************************************
World * world;
Film * film;
int frameCount = 0;
bool isRightMouseDown = false;
bool isDisplayCalculated = false;
bool _HANDLE_SHADOW = true;
bool _HANDLE_REFLECTION = true;
int _MAX_DEPTH = 4;
bool _LAZY_MODE = false;


// use this to multiply colors:
inline vec3 pairwiseMult(const vec3 &a, const vec3 &b) {
	return vec3(a[0]*b[0], a[1]*b[1], a[2]*b[2]);
}

//-------------------------------------------------------------------------------
// Here you raycast a single ray, calculating the color of this ray.
vec3 raycast(Ray & ray, int depth) {
	vec3 retColor(0,0,0);
	vec3 d = vec3(ray.direction().normalize(),VW);

	double t; vec3 n; MaterialInfo m;
	if (world->intersect(ray, t, n, m)) {
		if (_LAZY_MODE) // debugging and testing statement
			return vec3(1.0);
		retColor += pairwiseMult(m.color, world->getAmbientLight()) * m.k[MAT_KA];

		n.normalize();
		vec3 v = vec3(ray.start(), VW) - vec3(ray.getPos(t), VW);
		v.normalize();
		vec3 specHighlightColor = m.k[MAT_KSM]*m.color + (1-m.k[MAT_KSM])*vec3(1.0, 1.0, 1.0);
		
		// Handles Direct Light
		for (int lighttype = 0; lighttype < 3; lighttype++) {
			for (vector<Light>::iterator light = world->getLightsBeginIterator(lighttype); light != world->getLightsEndIterator(lighttype); light++) {
				LightInfo li = light->getLightInfo();

				if (_HANDLE_SHADOW) {
					// calculates whether or not this light hits the point (SHADOWS)
					vec3 endPos;
					if (li.type == LIGHT_DIRECTIONAL) {
						endPos = vec3(ray.getPos(t), VW) - light->getDirection();
					} else if (li.type == LIGHT_POINT || li.type == LIGHT_SPOT) {
						endPos = light->getPosition();
					}
					Ray incidentRay = Ray(ray.getPos(t), vec4(endPos, 1.0), 0.01); // better to hard code minTime?
					double intersectTime; vec3 garbageNormal; MaterialInfo garbageMaterial;
					if (world->intersect(incidentRay, intersectTime, garbageNormal, garbageMaterial) && (li.type == LIGHT_DIRECTIONAL || intersectTime < 1.0)) {
						// cout << "shadow detected" << endl;
						continue;
					}
				}

				vec3 l, lightColor = li.color;
				double lightDistance;
				// calculates l, the unit vector pointing towards the light source
				if (li.type == LIGHT_DIRECTIONAL) {
					l = -1*light->getDirection();
				}
				else if (li.type == LIGHT_POINT || li.type == LIGHT_SPOT) {
					Ray incidentRay = Ray(ray.getPos(t), vec4(light->getPosition(), 1.0), ray.getMinT()); // may want to hard code MinT
					l = light->getPosition() - vec3(ray.getPos(t), VW);
					lightDistance = l.length();
				}
				l.normalize();
				// calculates color from diffuse reflection
				double diffuseFactor = MAX(0.0, n*l);
				vec3 diffuseColor = pairwiseMult(m.k[MAT_KD]*diffuseFactor*lightColor, m.color);

				// calculates r, the unit vector in the direction of maximum specular direction
				vec3 r = 2*(n*l)*n - l;
				// calculates color from specular reflection
				double reflectFactor = MAX(0.0, v*r);
				vec3 specColor = pairwiseMult(m.k[MAT_KS]*pow(reflectFactor, m.k[MAT_KSP])*lightColor, specHighlightColor);

				// upkeeping
				if (li.type == LIGHT_POINT || li.type == LIGHT_SPOT) {
					double distanceFactor = pow(li.deadDistance + lightDistance, li.falloff);
					diffuseColor /= distanceFactor;
					specColor /= distanceFactor;
					if (li.type == LIGHT_SPOT) {
						vec3 d = light->getDirection();
						d.normalize();
						double angleFactor = pow((-1 * l) * d, li.angularFalloff);
						diffuseColor *= angleFactor;
						specColor *= angleFactor;
					}
				}
				
				// adds colors to color of point
				retColor += diffuseColor;
				retColor += specColor;
			}
		}

		if (_HANDLE_REFLECTION && depth < _MAX_DEPTH) {
			vec4 rayDir = ray.direction();
			rayDir[3] = 1;
			vec3 rayNorm = vec3(rayDir);
			rayNorm *= -1;
			rayNorm.normalize();
			vec3 reflectDir = 2*(n*rayNorm)*n - rayNorm;
			vec4 reflectDir4 = vec4(reflectDir);
			reflectDir4[3] = 0;
			Ray reflectRay = Ray(ray.getPos(t), ray.getPos(t) + 10*reflectDir4, ray.getMinT());
			retColor += raycast(reflectRay, depth + 1);
		}
		
	}
	
    return retColor * pow(m.k[MAT_KS], depth);
}

//-------------------------------------------------------------------------------
/// The display function
void display() {

	//Clear Buffers
    glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);					// indicate we are specifying camera transformations
	glLoadIdentity();							// make sure transformation is "zero'd"

	if (!isDisplayCalculated) {
		// Calls the raycast method on each pixel
		// sampled using the Viewport and Ray classes.
		// and stores the result using the Film class
		film->clear();
		Viewport &view = world->getViewport();
		view.resetSampler();
		vec2 point; Ray ray;
		while(view.getSample(point, ray)) {
			ray.transform(view.getViewToWorld());
			vec3 c = raycast(ray, 0);
			film->expose(c, point);
		}
		isDisplayCalculated = true;
	}
	film->show();

	//Now that we've drawn on the buffer, swap the drawing buffer and the displaying buffer.
	glutSwapBuffers();
}


//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized.
/// This gives you the opportunity to set up all the relevant transforms.
///
void reshape(int w, int h) {
	//Set up the viewport to ignore any size change.
	Viewport &view = world->getViewport();
	glViewport(0,0,view.getW(),view.getH());

	//Set up the PROJECTION transformationstack to be a simple orthographic [-1, +1] projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, view.getW(), 0, view.getH(), 1, -1);

	//Set the MODELVIEW transformation stack to the identity.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//-------------------------------------------------------------------------------
/// Called to handle keyboard events.
void myKeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
        case 's':
            film->saveFrame("./", "raycaster"); // image saving has been moved to the film class
            break;
		case 27:			// Escape key
			exit(0);
			break;
	}
}

void myMouseFunc(int button, int state, int x, int y) {
	/* as4 code */
	/*if ( button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN ) {
		world->as4Edit((double) x, (double) y);
		isRightMouseDown = true;
		glutPostRedisplay();
	}
	else if ( button==GLUT_RIGHT_BUTTON && state==GLUT_UP ) {
		isRightMouseDown = false;
	}*/
}

void myMotionFunc(int x, int y) {
    /* as4 code */
	/*if (isRightMouseDown) {
		world->as4Edit((double) x, (double) y);
		glutPostRedisplay();
	}*/
}

//-------------------------------------------------------------------------------
/// Initialize the environment
int main(int argc,char** argv) {

	//Initialize OpenGL
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

	// Load the world
	world = new World(argv[1]);
	// Allocate film for rendering
	film = new Film(IMAGE_WIDTH, IMAGE_HEIGHT);

	//Create OpenGL Window
	glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CS184 Raycaster");

	//Register event handlers with OpenGL.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutMotionFunc(myMotionFunc);
	glutPassiveMotionFunc(myMotionFunc);

	//And Go!
	glutMainLoop();
}

