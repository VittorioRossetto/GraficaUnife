/*
	Per evitare ripetizioni solo questo programma 
	per pianeta è stato commentato,
*/

#include "earth.h"

GLUquadricObj *obj_earth = NULL;

const float earth_ray = 0.3;  //raggio del pianeta
const float earth_distance = 7; // distanza dal sole
const float earth_veloc = 290; //velocità a cui si muove

// definiamo angolo di rotazione e di rivoluzione, x e y del pianeta
float earth_ang = 0.0;
float earth_rotate = 0.0;
float earth_x = 0.0;
float earth_y = 0.0;

void initEarth() {
	//funzione per inizializzare la terra 
    obj_earth = gluNewQuadric();
    gluQuadricDrawStyle(obj_earth, GLU_FILL);
    gluQuadricTexture(obj_earth, GL_TRUE);
    gluQuadricNormals(obj_earth, GLU_SMOOTH);
}

void drawEarth(int quality) {
	// poniamo la sfera
	// nella posizione definita in base alla distanza dal sole la velocità 
	// di rotazione
	glPushMatrix();
		glTranslatef(earth_x, 0, earth_y);
		glRotatef(earth_rotate,0, 1, 0);
		glRotatef(-90,1, 0, 0);
		glColor3f(0.0f, 0.5f, 0.5f);
		gluSphere(obj_earth, earth_ray, quality, quality);
	glPopMatrix();

	//Andiamo a scrivere il nome del pianeta
	//lo poniamo in bianco accanto al pianeta
	glColor3f(1.0, 1.0, 1.0);
	char name[] = "Earth";
	glRasterPos3f(earth_x + earth_ray, 0, earth_y + .1);
	int len = (int)strlen(name);
	for(int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, name[i]);
	}
}

void moveEarth() {
	// modifichiamo le coordinate della terra al passare del tempo,
	// in base alla velocità, per farlo calcoliamo l'angolo
	// e da lì ricaviamo x e y del pianeta
	earth_ang -= earth_veloc/10000;
	if(earth_ang < -360) {
		earth_ang = 0;
	}
	earth_x = earth_distance * cos(earth_ang);
	earth_y = earth_distance * sin(earth_ang);
	
	
}