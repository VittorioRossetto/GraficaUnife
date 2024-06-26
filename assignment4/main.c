/*
	Assignment 4: Solar System
    Vittorio Rossetto 159213

	Recreating the solar system using opengl
*/

// Standard includes
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// Including other files
#include "planets/sun.h"
#include "planets/jupiter.h"
#include "planets/mercury.h"
#include "planets/venus.h"
#include "planets/earth.h"
#include "planets/mars.h"
#include "planets/saturn.h"
#include "planets/uranus.h"
#include "planets/neptune.h"

// definiamo le variabili globali
const int quality = 30;

int modoLuz = 1;
float zCam = 0.001;
float yCam = 10.0;

void drawPlanets() {
	// funzione per disegnare tutti i pianeti,
	// viene chiamata ogni funzione dalle singole headers
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f); // sfondo nero

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
	
	// deiniamo la matrice per la visuale
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
	0.0f, 25.0f, zCam,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	drawPlanets();
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


void setLight(){
	// funzione per settare la luce 
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
	// funzione per inizializzare il movimento dei pianeti e della scena,
	// chiamiamo le singole funzioni per i pianeti dagli headers
    moveJupiter();
	moveMars();
	moveMercury();
	moveNeptune();
	moveSaturn();
	moveUranus();
	moveVenus();
	moveEarth();

	// modifico la posizione della camera per simulare il movimento della sonda
	if(zCam < 30)
		zCam = zCam + 0.15;
	
	setLight();
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
	
	//Inizializiamo ogni pianeta
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