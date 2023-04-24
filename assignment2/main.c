/*
    Assignment 2: Graph
    Vittorio Rossetto 159213

    Displaying a graph showing fps variation on number of displayed lines
    variation
*/

#include <GL/glut.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 900
#define DLENGTH 11

int fx[DLENGTH] = {1700, 300, 180, 175, 150, 130, 105, 95, 90, 80, 75};
//         x         1    10   20   30   40   50   60  70  80  90  100

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    float fxN[DLENGTH], iN, iSN;
    char y[5], x[5], zero = '0';
    int len, lastWritten;

    //modifichiamo i dati di partenza inserendoli in fxN per poterli utilizzare come coordinate
    for(int i = 0; i < DLENGTH; i++) {
        fxN[i] = .5 * (float)fx[i] / WIDTH;
    }

    //disegnamo le assi cartesiane in bianco
    glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(-1, 0);
            glVertex2f(1, 0);
        glEnd();
    glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2f(-0.9, 1);
            glVertex2f(-0.9, -1);
        glEnd();

    //scriviamo lo zero all'origine degli assi
    glColor3f(1, 1, 1);
    glRasterPos2f(-0.93,-0.03);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, zero);
    
    //calcoliamo un'unità così da distribuire adeguatamente i dati
    float xUnit = 1.9 / DLENGTH;

    for(int i = 0; i < DLENGTH; i++) {

        //tracciamo una linea tra un dato e il successivo
        if(i< DLENGTH - 1) {
            glBegin(GL_LINES);
                glColor3f(0.0, 1, 0.0);
                //essendo il primo dato relativo a 1 e non 0 modifico opportunamente la coordinata 
                if(i == 0)
                    iN = -0.9 + xUnit * .1;
                else
                    iN = -0.9 + xUnit * (float)i;
                glVertex2f(iN, fxN[i]);
                iSN = -0.9 + xUnit * ((float)i + 1);
                glVertex2f(iSN, fxN[i+1]);
            glEnd();
        }
        
        //scriviamo le y dei nostri dati sull'asse (evitando eventuali sovrapposizioni)
        if(i == 0 || abs(fx[i] - lastWritten) > 40) {
            sprintf(y, "%d", fx[i]);
            glColor3f(1, 1, 1);
            glRasterPos2f(-0.97,fxN[i]);
            len = (int)strlen(y);
            for(int j = 0; j < len; j++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, y[j]);
            }
            lastWritten = fx[i];
        }
        
        //scriviamo le x dei dati sull'asse
        if(i != 0)
            sprintf(x, "%d", i*10);
        else
            sprintf(x, "1"); //scriviamo per il primo dato
        glColor3f(1, 1, 1);
        if(i == DLENGTH -1)
            iN = -0.9 + xUnit * (float)i;
        glRasterPos2f(iN, -0.03);
        len = (int)strlen(x);
        for(int j = 0; j < len; j++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, x[j]);
        }
    
    }
    glEnd();
    glFlush();
}

// initialization function
void init (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(WIDTH, WIDTH);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("GRAPH   X = n_lines  Y = FPS");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}