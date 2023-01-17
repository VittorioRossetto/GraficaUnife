#include "mars.h"

GLUquadricObj *obj_mars = NULL;
GLuint idTexturemars;

const float mars_ray = 0.15;
const float mars_distance = 9;
const float mars_veloc = 240;//0 a 1000

float mars_ang = 0.0;
float mars_rotate = 0.0;
float mars_x = 0.0;
float mars_y = 0.0;

void initMars() {
    obj_mars = gluNewQuadric();
    gluQuadricDrawStyle(obj_mars, GLU_FILL);
    gluQuadricTexture(obj_mars, GL_TRUE);
    gluQuadricNormals(obj_mars, GLU_SMOOTH);
}

void drawMars(int quality) {
	glPushMatrix();
		glTranslatef(mars_x, 0, mars_y);
		glRotatef(mars_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(1.0f, 0.0f, 0.0f);
		gluSphere(obj_mars, mars_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Mars";
	glRasterPos3f(mars_x + mars_ray, 0, mars_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveMars() {
	mars_ang -= mars_veloc/10000;
	if(mars_ang < -360) {
		mars_ang = 0;
	}
	mars_x = mars_distance * cos(mars_ang);
	mars_y = mars_distance * sin(mars_ang);
	
}