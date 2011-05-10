/*
 * Film.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: njoubert
 *  (Edited by jima)
 */

#include "Film.h"

Film::Film(int w, int h, bool enableMultisample) {
	_multisampleEnabled = enableMultisample;
	_w = w;
	_h = h;
	_pixels = new GLfloat[w*h*3];
	_samples = new GLfloat[w*h];
	clear();
}

Film::~Film() {
	delete [] _pixels;
	delete [] _samples;
}

void Film::expose(const vec3 &c, const vec2 &p) {
	int pos = absolutePosition(floor(p[0]),floor(p[1]));
	_pixels[pos*3] += float(c[0]);
	_pixels[pos*3+1] += float(c[1]);
	_pixels[pos*3+2] += float(c[2]);
	_samples[pos] += 1.0f;
	
}

void Film::clear() {
	memset(_pixels, 0, sizeof(GL_FLOAT)*_w*_h*3);
	memset(_samples, 0, sizeof(GL_FLOAT)*_w*_h);
}

void Film::show() {
	if (_multisampleEnabled) { // divide by the sample count
		for (int i = 0; i < _w*_h; i++) {
			if (_samples[i] > 0) {
				_pixels[i*3] /= _samples[i];
				_pixels[i*3+1] /= _samples[i];
				_pixels[i*3+2] /= _samples[i];
				_samples[i] = 1;
			}
		}
	}
	glDrawPixels(_w, _h, GL_RGB, GL_FLOAT, _pixels);
}


void Film::saveAsBMP(std::string filename) {
	if (_multisampleEnabled) { // divide by the sample count
		for (int i = 0; i < _w*_h; i++) {
			if (_samples[i] > 0) {
				_pixels[i*3] /= _samples[i];
				_pixels[i*3+1] /= _samples[i];
				_pixels[i*3+2] /= _samples[i];
				_samples[i] = 1;
			}
		}
	}

    std::fstream fp_out;

    int filesize = 54 + 3 * _w * _h;// + _h*((4-(_w*3)%4)%4); //Account for padding
    unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54,
            0, 0, 0 };
    unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            0, 24, 0 };
    char bmppad[3] = { 0, 0, 0 };

    bmpfileheader[2] = (unsigned char) (filesize);
    bmpfileheader[3] = (unsigned char) (filesize >> 8);
    bmpfileheader[4] = (unsigned char) (filesize >> 16);
    bmpfileheader[5] = (unsigned char) (filesize >> 24);

    bmpinfoheader[4] = (unsigned char) (_w);
    bmpinfoheader[5] = (unsigned char) (_w >> 8);
    bmpinfoheader[6] = (unsigned char) (_w >> 16);
    bmpinfoheader[7] = (unsigned char) (_w >> 24);
    bmpinfoheader[8] = (unsigned char) (_h);
    bmpinfoheader[9] = (unsigned char) (_h >> 8);
    bmpinfoheader[10] = (unsigned char) (_h >> 16);
    bmpinfoheader[11] = (unsigned char) (_h >> 24);

    fp_out.open(filename.c_str(), std::ios::out | std::ios::binary);
    fp_out.write((char*) &bmpfileheader, 14);
    fp_out.write((char*) &bmpinfoheader, 40);
	for (int i = 0; i < _w*_h; i++) {
		unsigned char r = (unsigned char) MAX(0,MIN(255,(_pixels[i*3+0] * 255.0f)));
		unsigned char g = (unsigned char) MAX(0,MIN(255,(_pixels[i*3+1] * 255.0f)));
		unsigned char b = (unsigned char) MAX(0,MIN(255,(_pixels[i*3+2] * 255.0f)));
        fp_out.write((char*)&b, 1);
        fp_out.write((char*)&g, 1);
        fp_out.write((char*)&r, 1);
    }
	fp_out.write(bmppad, (4 - (_w * 3) % 4) % 4); //pad it like hellz!
    fp_out.close();
}


void Film::saveFrame(string imgOutDir, string prefix) {
	static int frameCount = 0;
    frameCount++;
	std::stringstream filename(stringstream::in | stringstream::out);
    filename << imgOutDir << prefix;
    //filename << std::setfill('0') << setw(6) << _frameCount << ".bmp";
    filename << frameCount << ".bmp";
    cout << "Save frame " << frameCount << endl;

    saveAsBMP(filename.str());
}

