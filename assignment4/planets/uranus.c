#include "uranus.h"

GLUquadricObj *obj_uranus = NULL;
GLuint idTextureuranus;

const float uranus_ray = 0.5;
const float uranus_distance = 17.5;
const float uranus_veloc = 50;//0 a 1000

float uranus_ang = 0.0;
float uranus_rotate = 0.0;
float uranus_x = 0.0;
float uranus_y = 0.0;

void initUranus() {
    obj_uranus = gluNewQuadric();
    gluQuadricDrawStyle(obj_uranus, GLU_FILL);
    gluQuadricTexture(obj_uranus, GL_TRUE);
    gluQuadricNormals(obj_uranus, GLU_SMOOTH);
}

void drawUranus(int quality) {
	glPushMatrix();
		glTranslatef(uranus_x, 0, uranus_y);
		glRotatef(uranus_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		gluSphere(obj_uranus, uranus_ray, quality, quality);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Uranus";
	glRasterPos3f(uranus_x + uranus_ray, 0, uranus_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveUranus() {
	uranus_ang -= uranus_veloc/10000;
	if(uranus_ang < -360) {
		uranus_ang = 0;
	}
	uranus_x = uranus_distance * cos(uranus_ang);
	uranus_y = uranus_distance * sin(uranus_ang);
	
}