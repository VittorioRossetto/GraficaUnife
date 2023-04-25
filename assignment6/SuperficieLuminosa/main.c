/*
    Vittorio Rossetto 159213
	Assignment 6 computer graphics: Superfice di bezier
*/

// usare export MESA_GL_VERSION_OVERRIDE=4.5 in caso di GLXBadFBConfig

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// Begin globals.
// Initial control points.
static float controlPoints[6][4][3] =
{
    {{-3.0, 0.0, 5.0}, {-0.25, 0.0, 5.0}, {0.25, 0.0, 5.0}, {3.0, 0.0, 5.0}},
    {{-3.0, 0.0, 3.0}, {-0.25, 0.0, 3.0}, {0.25, 0.0, 3.0}, {3.0, 0.0, 3.0}},
    {{-3.0, 0.0, 1.0}, {-0.25, 0.0, 1.0}, {0.25, 0.0, 1.0}, {3.0, 0.0, 1.0}},
    {{-3.0, 0.0, -1.0}, {-0.25, 0.0, -1.0}, {0.25, 0.0, -1.0}, {3.0, 0.0, -1.0}},
    {{-3.0, 0.0, -3.0}, {-0.25, 0.0, -3.0}, {0.25, 0.0, -3.0}, {3.0, 0.0, -3.0}},
    {{-3.0, 0.0, -5.0}, {-0.25, 0.0, -5.0}, {0.25, 0.0, -5.0}, {3.0, 0.0, -5.0}},
};

static float Xangle = 30.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate canoe.
static int rowCount = 0, columnCount = 0; // Indexes of selected control point.
// End globals.

// Proprieà materiale usato
GLfloat matAmbAndDifs[] = {1.0, 0.5, 1.0, 1.0};
GLfloat matSpecs[] = {0.0, 0.5, 0.0, 0.5};
GLfloat matShines[] = {3.0};

// Definisco la mia luce e la posizione nella scena.
void initlights(void)
{
    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat mat_specular[] = {0.0, 1.0, 0.0, 1.0};
    GLfloat position[] = {-10.0, 20.0, 5.0, 1.0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
}

// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{
    char *c;

    for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// Drawing routine.
void drawScene(void)
{
    int indx, indy;

   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   // Rotate scene.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   glPointSize(5.0);

   // Draw green control points.
   glColor3f(0.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (indx = 0; indx < 6; indx++)
         for (indy = 0; indy < 4; indy++)
            glVertex3fv(controlPoints[indx][indy]);
   glEnd();

   // Draw the control polyhedron
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glColor3f(0.0, 0.0, 0.0);
   for (indx = 0; indx < 5; indx++)
   {
      glBegin(GL_QUAD_STRIP);
      for (indy = 0; indy < 4; indy++)
	  {
         glVertex3fv(controlPoints[indx][indy]);
		 glVertex3fv(controlPoints[indx+1][indy]);
	  }
      glEnd();
   }

   // Draw red selected control point.
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_POINTS);
      glVertex3fv(controlPoints[rowCount][columnCount]);
   glEnd();

   // Specify and enable the Bezier surface.
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 6, controlPoints[0][0]);
   glEnable(GL_MAP2_VERTEX_3);

   // Assegno un materiale alla superficie e la disegno
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDifs);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecs);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShines);

   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);//partiziono gli intervalli in 20 punti
   glEvalMesh2(GL_FILL, 0, 20, 0, 20);// Metto tutti i punti a video


   // Draw the co-ordinate axes.
   glLineWidth(2.0);
   glColor3f(0.0, 0.0, 1.0);
   glBegin(GL_LINES);
	   glVertex3f(-6.0, 0.0, 0.0);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, 0.25);
       glVertex3f(6.0, 0.0, 0.0);
       glVertex3f(5.75, 0.0, -0.25);
       glVertex3f(6.0, 0.0, 0.0);

	   glVertex3f(0.0, -6.0, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);
       glVertex3f(-0.25, 5.75, 0.0);
       glVertex3f(0.0, 6.0, 0.0);

	   glVertex3f(0.0, 0.0, -6.0);
       glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
	   glVertex3f(-0.25, 0.0, 5.75);
	   glVertex3f(0.0, 0.0, 6.0);
   glEnd();
   glLineWidth(1.0);

   // Label the co-ordinate axes.
   glPushMatrix();
   glTranslatef(6.1, 0.0, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "X");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 6.1, 0.0);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "Y");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, 0.0, 6.1);
   glScalef(0.005, 0.005, 0.005);
   writeStrokeString(GLUT_STROKE_ROMAN, "Z");
   glPopMatrix();


  glutSwapBuffers();

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        exit(0);
        break;
    case 'x':
        Xangle += 5.0;
        if (Xangle > 360.0) Xangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'X':
        Xangle -= 5.0;
        if (Xangle < 0.0) Xangle += 360.0;
        glutPostRedisplay();
        break;
    case 'y':
        Yangle += 5.0;
        if (Yangle > 360.0) Yangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Y':
        Yangle -= 5.0;
        if (Yangle < 0.0) Yangle += 360.0;
        glutPostRedisplay();
        break;
    case 'z':
        Zangle += 5.0;
        if (Zangle > 360.0) Zangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Z':
        Zangle -= 5.0;
        if (Zangle < 0.0) Zangle += 360.0;
        glutPostRedisplay();
        break;
    case 9:
    {
        if (rowCount < 5) rowCount++;
        else rowCount = 0;
    }
    glutPostRedisplay();
    break;
    case ' ':
    {
        if (columnCount < 3) columnCount++;
        else columnCount = 0;
    }
    glutPostRedisplay();
    break;
    default:
        break;
    }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT) controlPoints[rowCount][columnCount][0] -= 0.1;
    if (key == GLUT_KEY_RIGHT) controlPoints[rowCount][columnCount][0] += 0.1;
    if (key == GLUT_KEY_DOWN) controlPoints[rowCount][columnCount][1] -= 0.1;
    if (key == GLUT_KEY_UP) controlPoints[rowCount][columnCount][1] += 0.1;
    if (key == GLUT_KEY_PAGE_DOWN) controlPoints[rowCount][columnCount][2] -= 0.1;
    if (key == GLUT_KEY_PAGE_UP) controlPoints[rowCount][columnCount][2] += 0.1;
    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    printf("Interaction:\n");
    printf("Press space and tab to select a control point.\n");
    printf("Press the right/left arrow keys to move the control point up/down the x-axis.\n");
    printf("Press the up/down arrow keys to move the control point up/down the y-axis.\n");
    printf("Press the page up/down keys to move the control point up/down the z-axis.\n");
    printf("Press the x, X, y, Y, z, Z keys to rotate the viewpoint.\n");
}

// Main routine.
int main(int argc, char **argv)
{
    printInteraction();
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("bezierSurface.cpp");
    initlights();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();

    return 0;
}