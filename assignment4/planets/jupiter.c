#include "jupiter.h"

GLUquadricObj *obj_jupiter = NULL;
GLuint idTextureJupiter;

const float jupiter_tam = 1.0;
const float jupiter_raio = 9;
const float jupiter_veloc = 140;//0 a 1000

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

void drawJupiter(int qualidade) {
	glPushMatrix();
		glTranslatef(jupiter_x, 0, jupiter_y);
		glRotatef(jupiter_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		gluSphere(obj_jupiter, jupiter_tam, qualidade, qualidade);
	glPopMatrix();
}

void moveJupiter() {
	jupiter_ang -= jupiter_veloc/10000;
	if(jupiter_ang < -360) {
		jupiter_ang = 0;
	}
	jupiter_x = jupiter_raio * cos(jupiter_ang);
	jupiter_y = jupiter_raio * sin(jupiter_ang);
	
	//rotation
	jupiter_rotate += 10.0f;
	if(jupiter_rotate > 360.0f) {
		jupiter_rotate = 0.0f;
	}
}