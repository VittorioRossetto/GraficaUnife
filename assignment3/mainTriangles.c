/*
* Draw a cube ... triangles
*/
#include <GL/glut.h>


// display function
void display(void)
{
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // store modelview settings
    glPushMatrix();

    // place the cube in the scene
    glTranslatef(0.0, 0.0, -1.5);
    glTranslatef(0.0, -.8, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(25.0, 1.0, 0.0, 0.0);

    // ---------- Lateral faces of the cube ----------
    glBegin(GL_TRIANGLES);

    //color for f0
    glColor3f(1.0, 0.0, 0.0);
    // right rectangle on f0
    glVertex3f(0.5, -0.5, 0.5); 
    glVertex3f(0.5, 0.5, 0.5); 
    glVertex3f(0.1, -0.5, 0.5); 

    glVertex3f(0.5, 0.5, 0.5); 
    glVertex3f(0.1, -0.5, 0.5); 
    glVertex3f(0.1, 0.5, 0.5); 

    //left rectangle in f0
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.1, 0.5, 0.5);

    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.1, 0.5, 0.5);
    glVertex3f(-0.1, -0.5, 0.5);

    //upper rectangle in f0
    glVertex3f(-0.1, 0.5, 0.5);
    glVertex3f(0.1, 0.5, 0.5);
    glVertex3f(0.1, 0.1, 0.5);

    glVertex3f(0.1, 0.1, 0.5);
    glVertex3f(-0.1, 0.5, 0.5);
    glVertex3f(-0.1, 0.1, 0.5);

    //bottom rectangle in f0
    glVertex3f(-0.1, -0.5, 0.5);
    glVertex3f(0.1, -0.5, 0.5);
    glVertex3f(-0.1, -0.1, 0.5);

    glVertex3f(0.1, -0.5, 0.5);
    glVertex3f(-0.1, -0.1, 0.5);
    glVertex3f(0.1, -0.1, 0.5);


    // f1 colors
    glColor3f(0.0, 1.0, 0.0);

    //right rectangle on f1
    glVertex3f(-0.5, -0.5, 0.5); 
    glVertex3f(-0.5, 0.5, 0.5); 
    glVertex3f(-0.5, -0.5, 0.1); 

    glVertex3f(-0.5, 0.5, 0.5); 
    glVertex3f(-0.5, -0.5, 0.1); 
    glVertex3f(-0.5, 0.5, 0.1);

    //left rectangle on f1
    glVertex3f(-0.5, -0.5, -0.5); 
    glVertex3f(-0.5, 0.5, -0.5); 
    glVertex3f(-0.5, -0.5, -0.1); 

    glVertex3f(-0.5, 0.5, -0.5); 
    glVertex3f(-0.5, -0.5, -0.1); 
    glVertex3f(-0.5, 0.5, -0.1);

    //upper rectangle on f1
    glVertex3f(-0.5, 0.5, 0.1);
    glVertex3f(-0.5, 0.5, -0.1);    
    glVertex3f(-0.5, 0.1, 0.1);

    glVertex3f(-0.5, 0.5, -0.1);    
    glVertex3f(-0.5, 0.1, 0.1);
    glVertex3f(-0.5, 0.1, -0.1);

    //bottom rectangle on f1
    glVertex3f(-0.5, -0.5, 0.1);
    glVertex3f(-0.5, -0.5, -0.1);    
    glVertex3f(-0.5, -0.1, 0.1);

    glVertex3f(-0.5, -0.5, -0.1);    
    glVertex3f(-0.5, -0.1, 0.1);
    glVertex3f(-0.5, -0.1, -0.1);


    //color for f2
    glColor3f(0.0, 0.0, 1.0);
    // right rectangle on f2
    glVertex3f(0.5, -0.5, -0.5); 
    glVertex3f(0.5, 0.5, -0.5); 
    glVertex3f(0.1, -0.5, -0.5); 

    glVertex3f(0.5, 0.5, -0.5); 
    glVertex3f(0.1, -0.5, -0.5); 
    glVertex3f(0.1, 0.5, -0.5); 

    //left rectangle in f2
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.1, 0.5, -0.5);

    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.1, 0.5, -0.5);
    glVertex3f(-0.1, -0.5, -0.5);

    //upper rectangle in f2
    glVertex3f(-0.1, 0.5, -0.5);
    glVertex3f(0.1, 0.5, -0.5);
    glVertex3f(0.1, 0.1, -0.5);

    glVertex3f(0.1, 0.1, -0.5);
    glVertex3f(-0.1, 0.5, -0.5);
    glVertex3f(-0.1, 0.1, -0.5);

    //bottom rectangle in f2
    glVertex3f(-0.1, -0.5, -0.5);
    glVertex3f(0.1, -0.5, -0.5);
    glVertex3f(-0.1, -0.1, -0.5);

    glVertex3f(0.1, -0.5, -0.5);
    glVertex3f(-0.1, -0.1, -0.5);
    glVertex3f(0.1, -0.1, -0.5);


    //color for f3
    glColor3f(1.0, 0.0, 1.0);
    //right rectangle on f3
    glVertex3f(0.5, 0.5, 0.5); 
    glVertex3f(0.5, -0.5, 0.1); 
    glVertex3f(0.5, -0.5, 0.5); 

    glVertex3f(0.5, 0.5, 0.5); 
    glVertex3f(0.5, -0.5, 0.1); 
    glVertex3f(0.5, 0.5, 0.1);

    //left rectangle on f3
    glVertex3f(0.5, -0.5, -0.5); 
    glVertex3f(0.5, 0.5, -0.5); 
    glVertex3f(0.5, -0.5, -0.1); 

    glVertex3f(0.5, 0.5, -0.5); 
    glVertex3f(0.5, -0.5, -0.1); 
    glVertex3f(0.5, 0.5, -0.1);

    //upper rectangle on f3
    glVertex3f(0.5, 0.5, 0.1);
    glVertex3f(0.5, 0.5, -0.1);    
    glVertex3f(0.5, 0.1, 0.1);

    glVertex3f(0.5, 0.5, -0.1);    
    glVertex3f(0.5, 0.1, 0.1);
    glVertex3f(0.5, 0.1, -0.1);

    //bottom rectangle on f3
    glVertex3f(0.5, -0.5, 0.1);
    glVertex3f(0.5, -0.5, -0.1);    
    glVertex3f(0.5, -0.1, 0.1);

    glVertex3f(0.5, -0.5, -0.1);    
    glVertex3f(0.5, -0.1, 0.1);
    glVertex3f(0.5, -0.1, -0.1);

    glEnd();

    // restore model view settings
    glPopMatrix();

    // flush graphics objects immediately
    glFinish();
}

void init (void)
{
    // select clearing color
    glClearColor (1.0, 1.0, 1.0, 0.0);

    // initialize viewing values
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // set viewing frustum
    glFrustum(-0.2, 0.2, -0.3, 0.1, 0.1, 4.0);

    // ... it does not hurt to check that everything went OK
    if (glGetError() != 0) {
        exit(-1);
    }

    // Turn on wireframe mode (only for debugging purposes)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // initialize model view transforms
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    // glEnable(GL_CULL_FACE);
}

// Window size and mode
int main(int argc, char** argv)
{
    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (600, 600);
    glutInitWindowPosition (200, 200);
    glutCreateWindow ("Cube with square holes GL_TRIANGLES");

    // Call initialization routinesx
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}
