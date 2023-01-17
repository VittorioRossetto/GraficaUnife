#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#include "planets/sun.h"
#include "planets/jupiter.h"
#include "planets/mercury.h"
#include "planets/venus.h"
#include "planets/earth.h"
#include "planets/mars.h"
#include "planets/saturn.h"
#include "planets/uranus.h"
#include "planets/neptune.h"

const int quality = 30;

float xrot = 0.0f;
float yrot = 0.0f;

int modoLuz = 1;
float zCam = 0.001;
float yCam = 10.0;

void desenhaPlanetas() {
	
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	drawSun(quality);
	drawJupiter(quality);
	drawMars(quality);
	drawMercury(quality);
	drawNeptune(quality);
	drawSaturn(quality);
	drawUranus(quality);
	drawVenus(quality);
	drawEarth(quality);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
	0.0f, 25.0f, zCam,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	// glRotatef(90, 1.0f, 0.0f, 0.0f);
	// glRotatef(-15, 0.0f, 1.0f, 0.0f);

	desenhaPlanetas();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(70.0f, 1.0f * w / h, 1.0f, 100 * 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void configuraLuz(){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 10.0 };
        GLfloat light_position[] = { 0,0,0 };
		GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0};
		glClearColor(0.0, 0.0, 0.0, 0.0);
        
        glShadeModel (GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
}


void moveItems() {
    moveJupiter();
	moveMars();
	moveMercury();
	moveNeptune();
	moveSaturn();
	moveUranus();
	moveVenus();
	moveEarth();

	if(zCam < 25)
		zCam = zCam + 0.1;
	
	
	configuraLuz();
	glutPostRedisplay();
	glutTimerFunc(33, moveItems, 1);
	
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 600);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("Solar System");
	
	initSun();
	initJupiter();
	initMars();
	initMercury();
	initNeptune();
	initSaturn();
	initUranus();
	initVenus();
	initEarth();

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	
	glutTimerFunc(33, moveItems, 1); // 1000/33 = 30 FPS

	glutMainLoop();

	return 0;
}