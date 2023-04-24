/*
    Assignment 3: Four faces cube with a hole in the center
    Vittorio Rossetto 159213

    Displaying a 4 faces cube with a hole in the center with the use of drawArrays without VBOs
*/


#include <GL/glut.h>

// number of faces
#define NRECTANGLES 16
// number of vertices per face
#define NVERTICES   4

// define vertices
GLfloat vertexArray[NRECTANGLES*NVERTICES][3] = {

    // right rectangle f0
    {0.5, -0.5, 0.5}, 
    {0.5, 0.5, 0.5}, 
    {0.1, -0.5, 0.5}, 
    {0.1, 0.5, 0.5}, 

    // left rectangle f0
    {-0.5, -0.5, 0.5}, 
    {-0.5, 0.5, 0.5}, 
    {-0.1, -0.5, 0.5}, 
    {-0.1, 0.5, 0.5}, 

    //upper rectangle f0
    {0.1, 0.1, 0.5}, 
    {0.1, 0.5, 0.5},
    {-0.1, 0.1, 0.5},
    {-0.1, 0.5, 0.5},

    // bottom rectangle f0
    {0.1, -0.1, 0.5}, 
    {0.1, -0.5, 0.5},
    {-0.1, -0.1, 0.5},
    {-0.1, -0.5, 0.5},



    // right rectangle f1
    {-0.5, -0.5, 0.5}, 
    {-0.5, 0.5, 0.5}, 
    {-0.5, -0.5, 0.1}, 
    {-0.5, 0.5, 0.1}, 

    // right rectangle f1
    {-0.5, -0.5, -0.5}, 
    {-0.5, 0.5, -0.5}, 
    {-0.5, -0.5, -0.1}, 
    {-0.5, 0.5, -0.1},

    //upper rectangle f1
    {-0.5, 0.1, 0.1},
    {-0.5, 0.5, 0.1},
    {-0.5, 0.1, -0.1},
    {-0.5, 0.5, -0.1},

    //upper rectangle f1
    {-0.5, -0.1, 0.1},
    {-0.5, -0.5, 0.1},
    {-0.5, -0.1, -0.1},
    {-0.5, -0.5, -0.1}, 



    // right rectangle f2
    {0.5, -0.5, -0.5}, 
    {0.5, 0.5, -0.5}, 
    {0.1, -0.5, -0.5}, 
    {0.1, 0.5, -0.5}, 

    // left rectangle f2
    {-0.5, -0.5, -0.5}, 
    {-0.5, 0.5, -0.5}, 
    {-0.1, -0.5, -0.5}, 
    {-0.1, 0.5, -0.5}, 

    //upper rectangle f2
    {0.1, 0.1, -0.5}, 
    {0.1, 0.5, -0.5},
    {-0.1, 0.1, -0.5},
    {-0.1, 0.5, -0.5},

    // bottom rectangle f2
    {0.1, -0.1, -0.5}, 
    {0.1, -0.5, -0.5},
    {-0.1, -0.1, -0.5},
    {-0.1, -0.5, -0.5},



    // right rectangle f3
    {0.5, -0.5, 0.5}, 
    {0.5, 0.5, 0.5}, 
    {0.5, -0.5, 0.1}, 
    {0.5, 0.5, 0.1}, 

    // right rectangle f3
    {0.5, -0.5, -0.5}, 
    {0.5, 0.5, -0.5}, 
    {0.5, -0.5, -0.1}, 
    {0.5, 0.5, -0.1},

    //upper rectangle f3
    {0.5, 0.1, 0.1},
    {0.5, 0.5, 0.1},
    {0.5, 0.1, -0.1},
    {0.5, 0.5, -0.1},

    //upper rectangle f3
    {0.5, -0.1, 0.1},
    {0.5, -0.5, 0.1},
    {0.5, -0.1, -0.1},
    {0.5, -0.5, -0.1} 
};

GLfloat colorArray[NRECTANGLES*NVERTICES][3] = {
    //f0
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 0.0}, 

    //f1
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 0.0},

    //f2
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},

    //f3
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
    {0.0, 1.0, 1.0},
};

// routine that udpates the
void display(void)
{
    GLshort indFace, indVertex;

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // store modelview settings
    glPushMatrix();

    // place the cube in the scene
    glTranslatef(0.0, 0.0, -1.5);
    glTranslatef(0.0, -.8, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(25.0, 1.0, 0.0, 0.0);

    // here we draw the triangle stripss
    for (indFace=0; indFace<NRECTANGLES; indFace++) {
        glBegin(GL_TRIANGLE_STRIP);
        for(indVertex=0; indVertex<NVERTICES; indVertex++) {
            // extract color
            glColor3fv(colorArray[indFace*NVERTICES + indVertex]);
            // extract vertices
            glVertex3fv(vertexArray[indFace*NVERTICES + indVertex]);
        };
        glEnd();
    };

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
    glFrustum(-0.2, 0.2, -0.3, 0.1, 0.1, 5.0);

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
    // IMPORTANT: initialize the depth buffer
    // otherwise things are displayed erratically!
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
    glutCreateWindow ("Cube with square holes Array method");

    // Call initialization routinesx
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}