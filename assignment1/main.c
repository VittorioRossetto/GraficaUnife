/*
 * This is a simple program that draws NLINES lines and computes
 * FPS (frames per second) by means of a circular buffer
 * Antonino Casile
 * 
 * Vittorio Rossetto
 * Class: Computer Graphics
 */

#include <GL/glut.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

// dimensione circular buffer
#define NELS    10

// al variare di NLINES variano il numero di linne a mettere a video.
#define NLINES  10000

// circular buffer used to compute frame rate
float circularBuffer[NELS];
int firstInd = 0, nEls = 0;

// function to get the number of elapsed ticks
uint32_t getTick()
{
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime( CLOCK_REALTIME, &ts );
    theTick  = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
    return theTick;
}

// Function to compute real modulus and NOT simply the remainder as % does
int modulo(int a, int b) {
    const int result = a % b;
    return result >= 0 ? result : result + b;
}

// Compute sum of the elements in the circular buffer
float sumCircularBuffer()
{
    int ind;
    float sum = 0;

    if (nEls > 0) {
        for (ind=1; ind<=nEls; ind++) {
            sum = sum + circularBuffer[modulo(firstInd-ind, NELS)];
        }
    }

    return sum;
}

// accumulate buffer and update window title
void computeAndShowFrameRate(void)
{
    static float lastTime = 0.0f;
    static unsigned int frameCount = 0;
    char windowTitle[100];
    float sumFPS;

    float currentTime = (float)getTick() * 0.001f;
    // Initialize lastTime to the current time
    if (lastTime == 0) {
        lastTime = currentTime;
    }

    // increase frame count
    frameCount++;
    if (currentTime - lastTime > 1.0f) {
        // insert the current fps in the circular buffer
        circularBuffer[firstInd] = ((float)frameCount) / (currentTime - lastTime);

        // update variable lastTime
        lastTime = currentTime;

        //circularBuffer[firstInd] = (float)frameCount;
        firstInd = ((firstInd+1)%NELS);
        if (nEls < NELS) {
            nEls++;
        }
        frameCount = 0;

        // sum elements in circular buffer
        sumFPS = sumCircularBuffer();
        snprintf(windowTitle, 100, "FPS = %6.2f", sumFPS/nEls);
        // update window title
        glutSetWindowTitle(windowTitle);
    }
}

// display function
void display(void)
{
    int currLineInd;

    // get current frame rate
    computeAndShowFrameRate();

    // clear buffer
    glClear (GL_COLOR_BUFFER_BIT);

    for (currLineInd = 0; currLineInd<NLINES; currLineInd++) {
        // draw line
        glBegin(GL_LINES);
        // random color
        glColor3f((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
        // random first point

        // aggiungo la coordianta z, passo cosi da 2d a 3d, in quanto ora uso glFrustum

        glVertex3f((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, -2.0);
        // random color
        glColor3f((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
        // random second point
        glVertex3f((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, -2.0);
        glEnd();
    }

    glFinish();
    glutPostRedisplay();
}

// initialization function
void init (void)
{
    // Use current time as seed for random generator
    srand(time(0));

    // select clearing color
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // Frustum
    glFrustum(0.0, 5.0, 0.0, 5.0, 10.0, 1.5);
}

// Window size and mode
int main(int argc, char** argv)
{
    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    // set display mode
    // GLUT_SINGLE = single buffer window
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("OpenGL Window");

    // Call initialization routinesx
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
