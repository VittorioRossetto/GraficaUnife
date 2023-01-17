#include "sun.h"

GLUquadricObj *obj_sol = NULL;

const float sol_ray = 1;
const float sol_distance = 0;

float sol_ang = 0.0f;
float sol_x = 0.0;
float sol_y = 0.0;

void initSun() {
    obj_sol = gluNewQuadric();
    gluQuadricDrawStyle(obj_sol, GLU_FILL);
    gluQuadricTexture(obj_sol, GL_TRUE);
    gluQuadricNormals(obj_sol, GLU_SMOOTH);
}

void drawSun(int quality) {
	glPushMatrix();
		glTranslatef(sol_x, 0, sol_y);
		glRotatef(sol_ang,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor4f(1.0f, 1.0f, 0.0f, 0.0f);
		gluSphere(obj_sol, sol_ray, quality, quality);
	glPopMatrix();
}