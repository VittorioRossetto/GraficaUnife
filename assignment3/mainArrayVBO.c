#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

// number of faces
#define NRECTANGLES 16
// number of vertices per face
#define NVERTICES   4

// Array of buffer ids.
unsigned int buffers[2];

// define vertices
GLfloat vertexArray[NRECTANGLES*NVERTICES*3] = {
    // right rectangle f0
    0.5, -0.5, 0.5, 
    0.5, 0.5, 0.5, 
    0.1, -0.5, 0.5, 
    0.1, 0.5, 0.5, 

    // left rectangle f0
    -0.5, -0.5, 0.5, 
    -0.5, 0.5, 0.5, 
    -0.1, -0.5, 0.5, 
    -0.1, 0.5, 0.5, 

    //upper rectangle f0
    0.1, 0.1, 0.5, 
    0.1, 0.5, 0.5,
    -0.1, 0.1, 0.5,
    -0.1, 0.5, 0.5,

    // bottom rectangle f0
    0.1, -0.1, 0.5, 
    0.1, -0.5, 0.5,
    -0.1, -0.1, 0.5,
    -0.1, -0.5, 0.5,


    // right rectangle f1
    -0.5, -0.5, 0.5, 
    -0.5, 0.5, 0.5, 
    -0.5, -0.5, 0.1, 
    -0.5, 0.5, 0.1, 

    // right rectangle f1
    -0.5, -0.5, -0.5, 
    -0.5, 0.5, -0.5, 
    -0.5, -0.5, -0.1, 
    -0.5, 0.5, -0.1,

    //upper rectangle f1
    -0.5, 0.1, 0.1,
    -0.5, 0.5, 0.1,
    -0.5, 0.1, -0.1,
    -0.5, 0.5, -0.1,

    //upper rectangle f1
    -0.5, -0.1, 0.1,
    -0.5, -0.5, 0.1,
    -0.5, -0.1, -0.1,
    -0.5, -0.5, -0.1, 


    // right rectangle f2
    0.5, -0.5, -0.5, 
    0.5, 0.5, -0.5, 
    0.1, -0.5, -0.5, 
    0.1, 0.5, -0.5, 

    // left rectangle f2
    -0.5, -0.5, -0.5, 
    -0.5, 0.5, -0.5, 
    -0.1, -0.5, -0.5, 
    -0.1, 0.5, -0.5, 

    //upper rectangle f2
    0.1, 0.1, -0.5, 
    0.1, 0.5, -0.5,
    -0.1, 0.1, -0.5,
    -0.1, 0.5, -0.5,

    // bottom rectangle f2
    0.1, -0.1, -0.5, 
    0.1, -0.5, -0.5,
    -0.1, -0.1, -0.5,
    -0.1, -0.5, -0.5,


    // right rectangle f3
    0.5, -0.5, 0.5, 
    0.5, 0.5, 0.5, 
    0.5, -0.5, 0.1, 
    0.5, 0.5, 0.1, 

    // right rectangle f3
    0.5, -0.5, -0.5, 
    0.5, 0.5, -0.5, 
    0.5, -0.5, -0.1, 
    0.5, 0.5, -0.1,

    //upper rectangle f3
    0.5, 0.1, 0.1,
    0.5, 0.5, 0.1,
    0.5, 0.1, -0.1,
    0.5, 0.5, -0.1,

    //upper rectangle f3
    0.5, -0.1, 0.1,
    0.5, -0.5, 0.1,
    0.5, -0.1, -0.1,
    0.5, -0.5, -0.1
};

GLfloat colorArray[NRECTANGLES*NVERTICES*3] = {
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,

    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0
};

// array containing indices
GLubyte elementIndices[NRECTANGLES*NVERTICES];

// display function
void display(void)
{
    GLint indFace, indVertex;

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // store modelview settings
    glPushMatrix();

    // place the cube in the scene
    glTranslatef(0.0, 0.0, -1.5);
    glTranslatef(0.0, -.8, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(25.0, 1.0, 0.0, 0.0);

    // here we draw the triangle strips
    for (indFace=0; indFace<NRECTANGLES; indFace++) {
        glDrawArrays(GL_TRIANGLE_STRIP, indFace*NVERTICES, 4);
    };

    // restore model view settings
    glPopMatrix();

    // flush graphics objects immediately
    glFinish();
}


void init (void)
{
    GLenum glErr;

    // select clearing color
    glClearColor (1.0, 1.0, 1.0, 0.0);

    // Enable two vertex arrays: coordinates and colors
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Generate buffer ids.
    glGenBuffers(2, buffers);

    // buffers[0] is for both vertices and colors
    // bind vertex buffer and reserve space.
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW);

    // Copy vertex coordinates data into first half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexArray), vertexArray);
    // Copy vertex color data into second half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexArray), sizeof(colorArray), colorArray);

    // buffers[1] is for indices
    // bind and fill indices buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    for(int currInd=0; currInd < NRECTANGLES*NVERTICES; currInd++) elementIndices[currInd]=0;
    // ... and copy it
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementIndices), elementIndices, GL_STATIC_DRAW);

    // Specify vertex and color pointers to the start of the respective data.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertexArray)));

    // initialize viewing values
    glMatrixMode(GL_PROJECTION);

    // set viewing frustum
    glFrustum(-0.2, 0.2, -0.3, 0.1, 0.1, 5.0);

    // ... it does not hurt to check that everything went OK
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // initialize model view transforms
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

// Window size and mode
int main(int argc, char** argv)
{
    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (600, 600);
    glutInitWindowPosition (200, 200);
    glutCreateWindow ("Cube with square holes Array with VBO method");

    // Here we add support for GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("GLEW init failed: %s\n", glewGetErrorString(err));
        exit(1);
    } else {
        printf("GLEW init success\n");
    };

    // Call initialization routinesx
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   
}
