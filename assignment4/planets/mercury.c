#include "mercury.h"

GLUquadricObj *obj_mercury = NULL;
GLuint idTexturemercury;

const float mercury_ray = 0.2;
const float mercury_distance = 3;
const float mercury_veloc = 470;//0 a 1000

float mercury_ang = 0.0;
float mercury_rotate = 0.0;
float mercury_x = 0.0;
float mercury_y = 0.0;

void initMercury() {
    obj_mercury = gluNewQuadric();
    gluQuadricDrawStyle(obj_mercury, GLU_FILL);
    gluQuadricTexture(obj_mercury, GL_TRUE);
    gluQuadricNormals(obj_mercury, GLU_SMOOTH);
}

void drawMercury(int quality) {
	glPushMatrix();
		glTranslatef(mercury_x, 0, mercury_y);
		glRotatef(mercury_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(0.1f, 0.0f, 0.0f);
		gluSphere(obj_mercury, mercury_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Mercury";
	glRasterPos3f(mercury_x + mercury_ray, 0, mercury_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveMercury() {
	mercury_ang -= mercury_veloc/10000;
	if(mercury_ang < -360) {
		mercury_ang = 0;
	}
	mercury_x = mercury_distance * cos(mercury_ang);
	mercury_y = mercury_distance * sin(mercury_ang);
}