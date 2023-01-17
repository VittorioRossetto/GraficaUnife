#include "neptune.h"

GLUquadricObj *obj_neptune = NULL;
GLuint idTextureneptune;

const float neptune_ray = 0.5;
const float neptune_distance = 20;
const float neptune_veloc = 50;//0 a 1000

float neptune_ang = 0.0;
float neptune_rotate = 0.0;
float neptune_x = 0.0;
float neptune_y = 0.0;

void initNeptune() {
    obj_neptune = gluNewQuadric();
    gluQuadricDrawStyle(obj_neptune, GLU_FILL);
    gluQuadricTexture(obj_neptune, GL_TRUE);
    gluQuadricNormals(obj_neptune, GLU_SMOOTH);
}

void drawNeptune(int quality) {
    
	glPushMatrix();
		glTranslatef(neptune_x, 0, neptune_y);
		glRotatef(neptune_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
        glColor3f(0.5f, 1.0f, 1.0f);
		gluSphere(obj_neptune, neptune_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Neptune";
	glRasterPos3f(neptune_x + neptune_ray, 0, neptune_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveNeptune() {
	neptune_ang -= neptune_veloc/10000;
	if(neptune_ang < -360) {
		neptune_ang = 0;
	}
	neptune_x = neptune_distance * cos(neptune_ang);
	neptune_y = neptune_distance * sin(neptune_ang);
}