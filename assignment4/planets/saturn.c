#include "saturn.h"

GLUquadricObj *obj_saturn = NULL;
GLuint idTexturesaturn;

const float saturn_ray = 0.4;
const float saturn_distance = 7.5;
const float saturn_veloc = 100;//0 a 1000

float saturn_ang = 0.0;
float saturn_rotate = 0.0;
float saturn_x = 0.0;
float saturn_y = 0.0;

void initSaturn() {
    obj_saturn = gluNewQuadric();
    gluQuadricDrawStyle(obj_saturn, GLU_FILL);
    gluQuadricTexture(obj_saturn, GL_TRUE);
    gluQuadricNormals(obj_saturn, GLU_SMOOTH);
}

void drawSaturn(int quality) {
	glPushMatrix();
		glTranslatef(saturn_x, 0, saturn_y);
		glRotatef(saturn_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(0.6f, 0.6f, 0.6f);
		gluSphere(obj_saturn, saturn_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Saturn";
	glRasterPos3f(saturn_x + saturn_ray, 0, saturn_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveSaturn() {
	saturn_ang -= saturn_veloc/10000;
	if(saturn_ang < -360) {
		saturn_ang = 0;
	}
	saturn_x = saturn_distance * cos(saturn_ang);
	saturn_y = saturn_distance * sin(saturn_ang);
}