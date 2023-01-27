#ifndef SPHERE
#define SPHERE

	#include <GL/glew.h>
	#include <math.h>
	#include <GL/glut.h>
	#include <unistd.h>
	#include <string.h>

    void addVertices(float x, float y, float z);
	void addNormals(float nx, float ny, float nz);
	void addTexCoords(float s, float t);
	void initSphere();

#endif