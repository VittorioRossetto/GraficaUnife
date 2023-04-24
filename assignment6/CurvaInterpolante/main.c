/*
    VITTORIO ROSSETTO
    159213
	Assignment 6 computer graphics: curva interpolante
*/
// usare export MESA_GL_VERSION_OVERRIDE=4.5 in caso di GLXBadFBConfig

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

// Inizializzo le variabili globali
static int numVal = 0, order = 4, askOrder = 1;
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// I punti di controllo finali 
static float controlPoints[4][3] =
{
    {-50.0, 10.0, 0.0}, { -37.0, -7.0, 0.0},
    {-10.0, 27.0, 0.0}, {0.0, 0.0, 0.0}
};

// I punti di controllo originali,
// poi verrano utilizzati in caso di reset
static float originalControlPoints[4][3] =
{
    {-50.0, 10.0, 0.0}, { -37.0, -7.0, 0.0},
    {-10.0, 27.0, 0.0}, {0.0, 0.0, 0.0}
};

//Struttua dati che conterrà i nuovi control point
static float nuoviControlPoints[4][3];


//matrice Minterpolante= Mb^-1 * Mbs (calcolato attraverso: https://matrixcalc.org/it/)
static float Minterpolante[4][4] =
{
    { 1.0, 0.0, 0.0, 0.0}, {-0.833, 3.0, -1.5, 0.333},
    { 0.333, -1.5, 3.0, -0.833}, {0.0, 0.0, 0.0, 1.0}
};

// Initialization routine.
void setup(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}
void NuoviControlPoints(void)
{
    // funzione per il calcolo dei nuovi control points, 
    // attraverso la matrice interpolante 
    int indx, indy, indz;
    for (indx=0; indx<4; indx++){
        for (indy=0; indy<4; indy++){
             nuoviControlPoints[indx][indy]=0;
            for (indz=0; indz<4; indz++){
         nuoviControlPoints[indx][indy] = nuoviControlPoints[indx][indy]+Minterpolante[indx][indz]*controlPoints[indz][indy];
            }
        }
    }
}

void restoreControlPoints(void)
{
    // funzione per ristabilire i valori iniziali
    int indx, indy;
    for (indx=0; indx<4; indx++)
        for (indy=0; indy<3; indy++)
            controlPoints[indx][indy] = originalControlPoints[indx][indy];
}

void writeBitmapString(void *font, char *string)
{
    // funzione per il disegno di una stringa bitmap
    char *c;

    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    if (askOrder == 1)
        // First screen.
    {
        // mostro scrtta la descrizione finchè l'utente non preme invio
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos3f(-25.0, 0.0, 0.0);
        writeBitmapString((void*)font, "Show Bezier curve of order: ");

        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(50.0, 0.0, 0.0);
        glRasterPos3f(20.0, 0.0, 0.0);
        writeBitmapString((void*)font, "4");
        glLoadIdentity();
    }
    else
        // Second screen.
    {
        int indx;
        // Calcolo i nuovi control-point
        NuoviControlPoints();
        // Metto a video i control point in blu.
        glPointSize(5.0);
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_POINTS);
        for (indx = 0; indx < order; indx++)
            glVertex3fv(controlPoints[indx]);
        glEnd();

        // Draw the control polygon in light gray.
        glColor3f(0.7, 0.7, 0.7);
        glBegin(GL_LINE_STRIP);
        for (indx = 0; indx < order; indx++)
            glVertex3fv(controlPoints[indx]);
        glEnd();

        // Specify and enable the Bezier curve.
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, order, nuoviControlPoints[0]);
        // Abilito la struttura dati
        glEnable(GL_MAP1_VERTEX_3);

        // Draw the Bezier curve by defining a sample grid and evaluating on it.
        glColor3f(1.0, 1.0, 1.0);
        glMapGrid1f(100, 0.0, 1.0);//Dico in quanti punti partizionare il mio intervallo, in questo caso 100
        glEvalMesh1(GL_LINE, 0, 100);// Definisco quali e quanti punti mettere a video, in questo caso tutti

        // Mostro in verde il punto selezionato
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3fv(controlPoints[numVal]);
        glEnd();

    }
    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        //ESC per uscire
        exit(0);
        break;
    case 13:
        askOrder = 0;
        glutPostRedisplay();
        break;
    case 127:
        askOrder = 1;
        restoreControlPoints();
        glutPostRedisplay();
        break;
    case ' ':
        if (askOrder == 0)
        {
            if (numVal < order-1) numVal++;
            else numVal = 0;
        }
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

// Funzione per definire il comportamento
// quando vengono premuti tasti non ASCII (arrow keys)
void specialKeyInput(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
    {
        if (askOrder == 1)
        {
            if (order < 6) order += 1;
        }
        else{
            controlPoints[numVal][1] += 1.0;
            nuoviControlPoints[numVal][1] += 1.0;
            }
    }
    if(key == GLUT_KEY_DOWN)
    {
        if (askOrder == 1)
        {
            if (order > 2) order -= 1;
        }
        else{
            controlPoints[numVal][1] -= 1.0;
            nuoviControlPoints[numVal][1] -= 1.0;
            }
    }
    if(key == GLUT_KEY_LEFT) {
                                controlPoints[numVal][0] -= 1.0;
                                nuoviControlPoints[numVal][0] -= 1.0;
                            }
    if(key == GLUT_KEY_RIGHT){
                                controlPoints[numVal][0] += 1.0;
                                nuoviControlPoints[numVal][0] += 1.0;
                            }
    glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Curva d'interpolazione");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
