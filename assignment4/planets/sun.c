#include "sun.h"

GLUquadricObj *obj_sun = NULL;

const float sun_ray = 1.5;
const float sun_distance = 0;

float sun_ang = 0.0f;
float sun_x = 0.0;
float sun_y = 0.0;

void initSun() {
    obj_sun = gluNewQuadric();
    gluQuadricDrawStyle(obj_sun, GLU_FILL);
    gluQuadricTexture(obj_sun, GL_TRUE);
    gluQuadricNormals(obj_sun, GLU_SMOOTH);
}

void drawSun(int quality) {
	glPushMatrix();
		glTranslatef(sun_x, 0, sun_y);
		glRotatef(sun_ang,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(1.0f, 1.0f, 0.0f);
		gluSphere(obj_sun, sun_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Sun";
	glRasterPos3f(sun_x + sun_ray, 0, sun_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}