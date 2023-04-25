/*
    Final Project: Pong
    Vittorio Rossetto 159213

    A version of the famous arcade game pong, Created 
    with the the use of Shaders
*/

// Standard includes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Including other files
#include "hemisphere.h"
#include "shader.h"
#include "cylinder.h"
#include "light.h"
#include "material.h"
#include "readBMP.h"
#include "scritta.h"


#define P 10

static enum object {HEMISPHERE, CYLINDER1, CYLINDER2, TITLE}; // VAO ids.
static enum buffer {HEM_VERTICES, HEM_INDICES, CYL1_VERTICES, CYL1_INDICES, CYL2_VERTICES, CYL2_INDICES, TITLE_VERTICES}; // VBO ids.

static int animationPeriod = 100;

// Light properties.
static const Light light0 =
{
    (vec4){0.0, 0.0, 0.0, 1.0},
    (vec4){1.0, 1.0, 1.0, 1.0},
    (vec4){1.0, 1.0, 1.0, 1.0},
    (vec4){0.0, 1.0, 2.0, 0.0}
};

// Global ambient.
static vec4 globAmb = (vec4)
{
    0.2, 0.2, 0.2, 1.0
};

// Front and back material properties.
static const Material canFandB =
{
    (vec4){1.0, 1.0, 1.0, 1.0},
    (vec4){1.0, 1.0, 1.0, 1.0},
    (vec4){1.0, 1.0, 1.0, 1.0},
    (vec4){0.0, 0.0, 0.0, 1.0},
    50.0f
};

// Hemisphere data.
static Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
static unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)];
static int hemCounts[HEM_LATS];
static void* hemOffsets[HEM_LATS];
static vec4 hemColors = {HEM_COLORS};

// Cylinder 1 data.
static Vertex cylVertices1[(CYL_LONGS + 1) * (CYL_LATS + 1)];
static unsigned int cylIndices1[CYL_LATS][2*(CYL_LONGS+1)];
static int cylCounts1[CYL_LATS];
static void* cylOffsets1[CYL_LATS];

// Cylinder 2 data.
static Vertex cylVertices2[(CYL_LONGS + 1) * (CYL_LATS + 1)];
static unsigned int cylIndices2[CYL_LATS][2*(CYL_LONGS+1)];
static int cylCounts2[CYL_LATS];
static void* cylOffsets2[CYL_LATS];

// Schermata di inizio
Vertex titleVertices[4];

//static mat4 modelViewMat = glm_mat4_identity();
mat4 modelViewMat = GLM_MAT4_IDENTITY_INIT;
static mat4 projMat = GLM_MAT4_IDENTITY_INIT;
static mat3 normalMat = GLM_MAT3_IDENTITY_INIT;


static unsigned int
width,
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
normalMatLoc,
projMatLoc,
objectLoc,
hemColorLoc,
cyl1TexLoc,
cyl2TexLoc,
ballTexLoc,
titleTexLoc,
vao[4],
buffer[7],
texture[4],
score[] = {0, 0},
height;

// Variables for game mechanichs
static float paddleSpeed = 0.5;
static float ballSpeed = 0.5;
float ballDirectionX = 0;
int ballDirectionY = 1;

int isAnimate = 0;

vec3 ballPosition = {0, 0, 0};
vec3 cyl1Position = {0, -6, 0};
vec3 cyl2Position = {0, 6, 0};
vec3 titlePosition = {3.0, 1.5, 0.0};
vec3 titleStopPosition = {3.0, 1.5, 0.0};
vec3 titlePlayPosition = {300.0, 1.5, 0.0};

static BitMapFile *image[4]; // Local storage for bmp image data.

void setup(void) {
    GLenum glErr;

      // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore setup = %d \n", glErr);
        exit(-1);
    }

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);


    // Create shader program executable.
    vertexShaderId = setShader("vertex", "vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    // Initialize cylinders, hemisphere and title screen
    fillCylinder(cylVertices1, cylIndices1, cylCounts1, cylOffsets1);
    fillCylinder(cylVertices2, cylIndices2, cylCounts2, cylOffsets2);
    fillHemisphere(hemVertices, hemIndices, hemCounts, hemOffsets);
    scritta(titleVertices);

    // Create VAOs and VBOs...
    glGenVertexArrays(4, vao);
    glGenBuffers(7, buffer);

    // ...and associate data with vertex shader.
    glBindVertexArray(vao[CYLINDER1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[CYL1_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylVertices1), cylVertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[CYL1_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylIndices1), cylIndices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cylVertices1[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(cylVertices1[0]), (void*)sizeof(cylVertices1[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cylVertices1[0]),
                          (void*)(sizeof(cylVertices1[0].coords)+sizeof(cylVertices1[0].normal)));
    glEnableVertexAttribArray(2);

    if ((glErr = glGetError()) != 0)
    {
        printf("Errore cylinder 1= %d \n", glErr);
        exit(-1);
    }

    // ...and associate data with vertex shader.
    glBindVertexArray(vao[CYLINDER2]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[CYL2_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylVertices2), cylVertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[CYL2_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylIndices2), cylIndices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cylVertices2[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(cylVertices2[0]), (void*)sizeof(cylVertices2[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cylVertices2[0]),
                          (void*)(sizeof(cylVertices2[0].coords)+sizeof(cylVertices2[0].normal)));
    glEnableVertexAttribArray(2);

    if ((glErr = glGetError()) != 0)
    {
        printf("Errore cylinder 2= %d \n", glErr);
        exit(-1);
    }

    // ...and associate data with vertex shader.
    glBindVertexArray(vao[HEMISPHERE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[HEM_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hemVertices), hemVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[HEM_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hemIndices), hemIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), 0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]), (void*)sizeof(hemVertices[0].coords));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(hemVertices[0]),
                          (void*)(sizeof(hemVertices[0].coords)+sizeof(hemVertices[0].normal)));
    glEnableVertexAttribArray(5);

    // Obtain color uniform locations and set values.
    hemColorLoc = glGetUniformLocation(programId, "hemColor");
    glUniform4fv(hemColorLoc, 1, &hemColors[0]); 

    if ((glErr = glGetError()) != 0)
    {
        printf("Errore hemisphere= %d \n", glErr);
        exit(-1);
    }

    // ...and associate data with vertex shader.
    glBindVertexArray(vao[TITLE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[TITLE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(titleVertices), titleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(titleVertices[0]), 0);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(titleVertices[0]), (void*)sizeof(titleVertices[0].coords));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(titleVertices[0]),
                          (void*)(sizeof(titleVertices[0].coords)+sizeof(titleVertices[0].normal)));
    glEnableVertexAttribArray(5);

    // Obtain modelview matrix, projection matrix, normal matrix and object uniform locations.
    modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
    projMatLoc = glGetUniformLocation(programId, "projMat");
    normalMatLoc = glGetUniformLocation(programId, "normalMat");
    objectLoc = glGetUniformLocation(programId, "object");

    // Obtain light property uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
    glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

    // Obtain global ambient uniform location and set value.
    glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

    // Obtain material property uniform locations and set values.
    glUniform4fv(glGetUniformLocation(programId, "canFandB.ambRefl"), 1, &canFandB.ambRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "canFandB.difRefl"), 1, &canFandB.difRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "canFandB.specRefl"), 1, &canFandB.specRefl[0]);
    glUniform4fv(glGetUniformLocation(programId, "canFandB.emitCols"), 1, &canFandB.emitCols[0]);
    glUniform1f(glGetUniformLocation(programId, "canFandB.shininess"), canFandB.shininess);

    // Load the images.
    image[0] = readBMP("./textures/blueWoodTex.bmp");
    image[1] = readBMP("./textures/redWoodTex.bmp");
    image[2] = readBMP("./textures/ballTex.bmp");
    image[3] = readBMP("./textures/TitleScreen.bmp");

    // Create texture ids.
    glGenTextures(4, texture);

    // Bind blue wood image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    cyl1TexLoc = glGetUniformLocation(programId, "blueWoodTex");
    glUniform1i(cyl1TexLoc, 0);

    // Bind red wood image
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    cyl2TexLoc = glGetUniformLocation(programId, "redWoodTex");
    glUniform1i(cyl2TexLoc, 1);

    // Bind ball immage
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ballTexLoc = glGetUniformLocation(programId, "ballTex");
    glUniform1i(ballTexLoc, 2);

    // Bind title screen image
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    titleTexLoc = glGetUniformLocation(programId, "titleScreen");
    glUniform1i(titleTexLoc, 3);

    // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore fine setup= %d \n", glErr);
        exit(-1);
    }
}

// Drawing routine
void display(void) {

    mat3 TMP;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate and update projection matrix.
    //angolo visuale in radianti, grandezza x-y piano vicino alla telecamera paino lontano(profondita)
    glm_perspective(5.0f, (float)width/(float)height, 1.0f, 100.0f, projMat);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *) projMat);

     // Calculate and update normal matrix.
    glm_mat4_pick3(modelViewMat, TMP);
    glm_mat3_inv(TMP, normalMat);
    glm_mat3_transpose(normalMat);
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, (GLfloat *)normalMat);

    // Draw title screen
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    glm_translate(modelViewMat, titlePosition);
    glm_rotate(modelViewMat, 3.14, (vec3){0.0, 0.0, 1.0});
    glUniform1ui(objectLoc, TITLE);
    glBindVertexArray(vao[TITLE]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    // Draw cylinder.
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    glm_translate(modelViewMat, cyl1Position);
    glm_rotate(modelViewMat, 80.0, (vec3){0.0, 1.0, 0.0});
    glm_rotate(modelViewMat, 0.08, (vec3){1.0, 0.0, 0.0});
    glUniform1ui(objectLoc, CYLINDER1);
    glBindVertexArray(vao[CYLINDER1]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat); // Update modelview matrix.
    glMultiDrawElements(GL_TRIANGLE_STRIP, cylCounts1, GL_UNSIGNED_INT,
        (const void **)cylOffsets1, CYL_LATS);
    

    // Draw cylinder.
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    glm_translate(modelViewMat, cyl2Position);
    glm_rotate(modelViewMat, 80.0, (vec3){0.0, 1.0, 0.0});
    glm_rotate(modelViewMat, -0.08, (vec3){1.0, 0.0, 0.0});
    glUniform1ui(objectLoc, CYLINDER2);
    glBindVertexArray(vao[CYLINDER2]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat); // Update modelview matrix.
    glMultiDrawElements(GL_TRIANGLE_STRIP, cylCounts2, GL_UNSIGNED_INT,
        (const void **)cylOffsets2, CYL_LATS);

    // Draw ball as two hemispheres.
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    glm_translate(modelViewMat, ballPosition);
    glUniform1ui(objectLoc, HEMISPHERE); // Update object name.
    glBindVertexArray(vao[HEMISPHERE]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat); // Update modelview matrix.
    glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);
    glm_scale(modelViewMat, (vec3){1.0, -1.0, 1.0}); // Scale to make inverted hemisphere.
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat); // Update modelview matrix.
    glMultiDrawElements(GL_TRIANGLE_STRIP, hemCounts, GL_UNSIGNED_INT, (const void **)hemOffsets, HEM_LATS);

    glutSwapBuffers();
}

// Routine for window resizing 
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}

// Function to handle normal keys input
void keyInput(unsigned char key, int x, int y) {

    switch(key) {
        case ' ':   //Space-bar for game starting
            if (isAnimate)
                isAnimate = 0;
		    else {
                isAnimate = 1;
                titlePosition[0] = titlePlayPosition[0];
                animate(1);

            }
            break;

        //Player 1 controllers
        case 'a':
            cyl1Position[0] += paddleSpeed;
            break;

        case 'd':
            cyl1Position[0] -= paddleSpeed;
            break;
            
        //Making sure controls could work even with caps-lock
        case 'A':
            cyl1Position[0] += paddleSpeed;
            break;
        case 'D':
            cyl1Position[0] -= paddleSpeed;
            break;

        default:
            break;
    }
}

// Controls for player 2 who uses arrow keys
void special_keypress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			cyl2Position[0] += paddleSpeed;
			break;
		case GLUT_KEY_RIGHT:
			cyl2Position[0] -= paddleSpeed;
			break;
		default:
			break;
	}
}

// Function to handle scene animation
void animate(int val) {
    if(isAnimate) {
        // Switching ball X direction on border bounce
        if(ballPosition[0] > 6 || ballPosition[0] < -6 )
            ballDirectionX = -ballDirectionX;
        
        // Handling paddle collision
        if(ballPosition[1] == cyl1Position[1] + 1 &&
         (ballPosition[0] >= cyl1Position[0] - 1 && ballPosition[0] <= cyl1Position[0] + 1))
         {
            // Switching ball Y direction on border bounce
            ballDirectionY = -ballDirectionY;
            // Deviating ball on non centered hits
            ballDirectionX += (ballPosition[0] - cyl1Position[0]);
         }
        else if(ballPosition[1] == cyl2Position[1] - 1 &&
         (ballPosition[0] >= cyl2Position[0] - 1 && ballPosition[0] <= cyl2Position[0] + 1))
        {
            // Switching ball Y direction on border bounce
            ballDirectionY = -ballDirectionY;
            // Deviating ball on non centered hits
            ballDirectionX += (ballPosition[0] - cyl2Position[0]);
        }

        //Handling Score making
        if(ballPosition[1] < cyl1Position[1])
            scored(1);
        else if(ballPosition[1] > cyl2Position[1])
            scored(2);
        

        //Updating ball position
        ballPosition[0] += ballSpeed * ballDirectionX; 
        ballPosition[1] += ballSpeed * ballDirectionY; 

        glutPostRedisplay();
        glutTimerFunc(animationPeriod, animate, 1);
    } 
}

// Function handling scores and positions resetting
void scored(int scorer) {

    //Resetting ball
    ballPosition[0] = 0;
    ballPosition[1] = 0;
    ballDirectionX = 0;
    ballDirectionY = 1;

    //Resetting Paddles
    cyl1Position[0] = 0;
    cyl2Position[0] = 0; 

    if(scorer == 1)
        score[0]++;
    else if(scorer == 2)
        score[1]++;

    printf("\nScore: %d - %d", score[0], score[1]);
}

int main(int argc, char** argv) {
    GLenum glErr;

    printf("\n\n==========   PONG   ==========");
    printf("\n\nCommands: \n");
    printf("Press space-bar to play/pause\n");
    printf("Press a and d for player 1\nPress left-key and right-key for player 2\n\n");

    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Pong");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(&special_keypress);
    glewInit();

    // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore main= %d \n", glErr);
        exit(-1);
    }

    setup();
    glutMainLoop();

    return 0;

    return 0;
}