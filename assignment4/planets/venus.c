#include "venus.h"

GLUquadricObj *obj_venus = NULL;
GLuint idTexturevenus;

const float venus_ray = 0.3;
const float venus_distance = 5;
const float venus_veloc = 350;//0 a 1000

float venus_ang = 0.0;
float venus_rotate = 0.0;
float venus_x = 0.0;
float venus_y = 0.0;

void initVenus() {
    obj_venus = gluNewQuadric();
    gluQuadricDrawStyle(obj_venus, GLU_FILL);
    gluQuadricTexture(obj_venus, GL_TRUE);
    gluQuadricNormals(obj_venus, GLU_SMOOTH);
}

void drawVenus(int quality) {
	glPushMatrix();
		glTranslatef(venus_x, 0, venus_y);
		glRotatef(venus_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(0.0f, 0.2f, 0.0f);
		gluSphere(obj_venus, venus_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Venus";
	glRasterPos3f(venus_x + venus_ray, 0, venus_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveVenus() {
	venus_ang -= venus_veloc/10000;
	if(venus_ang < -360) {
		venus_ang = 0;
	}
	venus_x = venus_distance * cos(venus_ang);
	venus_y = venus_distance * sin(venus_ang);
	
}