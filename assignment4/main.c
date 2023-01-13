#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#include "planets/sun.h"
#include "planets/jupiter.h"
// #include "planets/mercury.h"
// #include "planets/venus.h"
// #include "planets/earth.h"
// #include "planets/mars.h"
// #include "planets/saturn.h"
// #include "planets/uranus.h"
// #include "planets/neptune.h"

const int quality = 30;

float xrot = 0.0f;
float yrot = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

float tra_x = 0.0f;
float tra_y = 0.0f;
float tra_z = -10.0f; //-10 = visao de longe

int modoLuz = 1;


GLuint carregaTextura(const char* arquivo) {
    GLuint idTexture = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTexture == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTexture;
}

void desenhaPlanetas() {
	
	glTranslatef(tra_x, tra_y, tra_z);
	glColor3f(1.0f, 1.0f, 1.0f);

	drawSun(quality);
	drawJupiter(quality);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
	0.0f, 0.0f, 10.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f);

	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(-15, 0.0f, 1.0f, 0.0f);

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
    if(modoLuz == 1){
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 10.0 };
        GLfloat light_position[] = { 0.1, 0.1, 0.1, 0 };
        glShadeModel (GL_SMOOTH);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);

        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
    }  else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
}


void moveItems() {
    moveJupiter();
	
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

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	
	glutTimerFunc(33, moveItems, 1); // 1000/33 = 30 FPS

	glutMainLoop();

	return 0;
}