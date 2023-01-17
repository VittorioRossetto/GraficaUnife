#include "jupiter.h"

GLUquadricObj *obj_jupiter = NULL;
GLuint idTextureJupiter;

const float jupiter_ray = 1.4;
const float jupiter_distance = 11.5;
const float jupiter_veloc = 130;//0 a 1000

float jupiter_ang = 0.0;
float jupiter_rotate = 0.0;
float jupiter_x = 0.0;
float jupiter_y = 0.0;

void initJupiter() {
    obj_jupiter = gluNewQuadric();
    gluQuadricDrawStyle(obj_jupiter, GLU_FILL);
    gluQuadricTexture(obj_jupiter, GL_TRUE);
    gluQuadricNormals(obj_jupiter, GLU_SMOOTH);
    ;
}

void drawJupiter(int quality) {
	glPushMatrix();
		glTranslatef(jupiter_x, 0, jupiter_y);
		glRotatef(jupiter_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor4f(1.0f, 0.5f, 0.0f, 0.0f);
		gluSphere(obj_jupiter, jupiter_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Jupiter";
	glRasterPos3f(jupiter_x + jupiter_ray, 0, jupiter_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveJupiter() {
	jupiter_ang -= jupiter_veloc/10000;
	if(jupiter_ang < -360) {
		jupiter_ang = 0;
	}
	jupiter_x = jupiter_distance * cos(jupiter_ang);
	jupiter_y = jupiter_distance * sin(jupiter_ang);
}