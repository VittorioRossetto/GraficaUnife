#include "earth.h"

GLUquadricObj *obj_earth = NULL;
GLuint idTextureearth;

const float earth_ray = 0.5;
const float earth_distance = 4;
const float earth_veloc = 200;//0 a 1000

float earth_ang = 0.0;
float earth_rotate = 0.0;
float earth_x = 0.0;
float earth_y = 0.0;

void initEarth() {
    obj_earth = gluNewQuadric();
    gluQuadricDrawStyle(obj_earth, GLU_FILL);
    gluQuadricTexture(obj_earth, GL_TRUE);
    gluQuadricNormals(obj_earth, GLU_SMOOTH);
}

void drawEarth(int quality) {
	glPushMatrix();
		glTranslatef(earth_x, 0, earth_y);
		glRotatef(earth_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(0.0f, 0.5f, 0.5f);
		gluSphere(obj_earth, earth_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Earth";
	glRasterPos3f(earth_x + earth_ray, 0, earth_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveEarth() {
	earth_ang -= earth_veloc/10000;
	if(earth_ang < -360) {
		earth_ang = 0;
	}
	earth_x = earth_distance * cos(earth_ang);
	earth_y = earth_distance * sin(earth_ang);
	
	
}