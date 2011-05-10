/*
 * Film.h
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *	Edited by jima
 */

#ifndef FILM_H_
#define FILM_H_

#include "global.h"

class Film {
public:
    Film(int w, int h, bool enableMultisample = true);
    ~Film();

    void expose(const vec3 &c, const vec2 &p); // add a sample to the screen
	void show(); // show the film on the screen
	void clear(); // clear the film

	// toggle whether we draw multiple samples per pixel (just enables the divided-by-_samples line)
	void allowMultisample(bool yes = true) { _multisampleEnabled = yes; }
	
	 /* This routine is an adaptation of the saveBMP routine written by
     ** SuperJer
     ** Email = superjer@superjer.com
     ** Web   = http://www.superjer.com/
     */
    void saveAsBMP(std::string filename);
	void saveFrame(string imgOutDir = "./", string prefix = "screenshot");

private:
    int _w;
    int _h;
	GLfloat* _pixels; // This will be a linear array of pixels, row by row from top to bottom
	GLfloat* _samples; // Sample counts, for multisampling
	bool _multisampleEnabled;

	/**
     * Return a flat array index for an x,y pixel location
     */
    inline int absolutePosition(int x, int y) {
        if (x < 0 || x >= (_w) || y < 0 || y >= (_h)) {
            UCBPrint("Image", "Tried to get a pixel outside of valid range!");
            exit(1);
        }
        return x + (y * _w);
    }
    
};

#endif /* FILM_H_ */

