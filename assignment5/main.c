/*
    Assignment 5: Solar System (using shaders)
    Vittorio Rossetto 159213

    Recreating the solar system using shaders and textures
 */

// Standard includes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Including other files
#include "shader.h"
#include "light.h"
#include "material.h"
#include "readBMP.h"
#include "hemisphere.h"
#include "scritta.h"
#include "funzioni.h"

// VAO one element for each planet and each name
static enum object {SOLE, TERRA, MERCURIO, VENERE, MARTE, GIOVE, SATURNO, URANO, NETTUNO,
                    SGIOVE, STERRA, SMERCURIO, SVENERE, SMARTE, SSATURNO,SURANO, SNETTUNO};

// VBO 3 for each planet (vertices, indices and the space used for name)
static enum buffer {SOLE_VERTICES, SOLE_INDICES, TERRA_VERTICES, TERRA_INDICES,
                    MERCURIO_VERTICES, MERCURIO_INDICES, VENERE_VERTICES, VENERE_INDICES,
                    MARTE_VERTICES, MARTE_INDICES,  GIOVE_VERTICES, GIOVE_INDICES,
                    SATURNO_VERTICES, SATURNO_INDICES, URANO_VERTICES, URANO_INDICES,
                    NETTUNO_VERTICES, NETTUNO_INDICES, SGIOVE_VERTICES, STERRA_VERTICES,
                    SMERCURIO_VERTICES, SVENERE_VERTICES, SMARTE_VERTICES, SSATURNO_VERTICES,
                    SURANO_VERTICES, SNETTUNO_VERTICES};

// Light properties.
static const Light light0 =
{
    (vec4){0.5, 0.5, 0.5, 1.0},//ambientale
    (vec4){1.0, 1.0, 1.0, 1.0},//diffusa
    (vec4){1.0, 1.0, 1.0, 1.0},//speculare
    (vec4){0.0, 0.0, -P, 0.0}//posizione
};

// Global ambient.
static vec4 globAmb = (vec4)
{
    0.0, 0.0, 0.0, 1.0
};

// Front and back material properties.
static const Material canFandB =
{
    (vec4){1.0, 1.0, 1.0, 1.0},//ambientale
    (vec4){1.0, 1.0, 1.0, 1.0},//diffusa
    (vec4){1.0, 1.0, 1.0, 1.0},//speculare
    (vec4){0.0, 0.0, 0.0, 1.0},
    10.0f //shining
};

// Matrices
mat4 modelViewMat = GLM_MAT4_IDENTITY_INIT;
static mat4 projMat = GLM_MAT4_IDENTITY_INIT;
static mat3 normalMat = GLM_MAT3_IDENTITY_INIT;

// Variables for movement on elipse
static float x[8] = {1, 1, 1, 1, 1, 1, 1, 1 };
static float y[8] = {0, 0, 0, 0, 0, 0, 0, 0 };

// SUN
Vertex soleVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int soleIndices[HEM_LATS][2*(HEM_LONGS+1)];
int soleCounts[HEM_LATS];
void* soleOffsets[HEM_LATS];

// EARTH
Vertex terraVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int terraIndices[HEM_LATS][2*(HEM_LONGS+1)];
int terraCounts[HEM_LATS];
void* terraOffsets[HEM_LATS];

// MERCURY
Vertex mercurioVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int mercurioIndices[HEM_LATS][2*(HEM_LONGS+1)];
int mercurioCounts[HEM_LATS];
void* mercurioOffsets[HEM_LATS];

// VENUS
Vertex venereVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int venereIndices[HEM_LATS][2*(HEM_LONGS+1)];
int venereCounts[HEM_LATS];
void* venereOffsets[HEM_LATS];

// MARS
Vertex marteVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int marteIndices[HEM_LATS][2*(HEM_LONGS+1)];
int marteCounts[HEM_LATS];
void* marteOffsets[HEM_LATS];

// JUPITER
Vertex gioveVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int gioveIndices[HEM_LATS][2*(HEM_LONGS+1)];
int gioveCounts[HEM_LATS];
void* gioveOffsets[HEM_LATS];

// SATURN
Vertex saturnoVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int saturnoIndices[HEM_LATS][2*(HEM_LONGS+1)];
int saturnoCounts[HEM_LATS];
void* saturnoOffsets[HEM_LATS];

// URANUS
Vertex uranoVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int uranoIndices[HEM_LATS][2*(HEM_LONGS+1)];
int uranoCounts[HEM_LATS];
void* uranoOffsets[HEM_LATS];

// NEPTUNE
Vertex nettunoVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)];
unsigned int nettunoIndices[HEM_LATS][2*(HEM_LONGS+1)];
int nettunoCounts[HEM_LATS];
void* nettunoOffsets[HEM_LATS];

// NAMES
Vertex Verticesgi[4];
Vertex Verticeste[4];
Vertex Verticesme[4];
Vertex Verticesve[4];
Vertex Verticesma[4];
Vertex Verticesne[4];
Vertex Verticesu[4];
Vertex Verticessa[4];

// Defining ray for each planet
float raggiosole=11.0;
float raggioterra=0.8;
float raggiomercurio=0.6;
float raggiovenere=0.8;
float raggiomarte=0.5;
float raggiogiove=6.0;
float raggiosaturno=4.8;
float raggiourano=1.5;
float raggionettuno=1.4;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
normalMatLoc,
projMatLoc,
textureLoc,
textureLoc,
objectLoc,
buffer[26],// 3 for each planet and 2 for the sun (cause sun hasn't name in the scene)
vao[17],// 2 for each planet and 1 for sun
texture[17],// One for each image (textures and names)
width,
height;

static BitMapFile *image[17]; // Local storage for bmp image data.

// Initialization routine.
void setup(void)
{
    GLenum glErr;

      // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);// Dark background
    glEnable(GL_DEPTH_TEST);// Enabling depth buffer

    // Create shader program executable.
    vertexShaderId = setShader("vertex", "vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    // Initializing each hemisphere
    fillHemisphere(soleVertices, soleIndices, soleCounts, soleOffsets, raggiosole,1);
    fillHemisphere(terraVertices, terraIndices, terraCounts, terraOffsets, raggioterra,0);
    fillHemisphere(mercurioVertices, mercurioIndices, mercurioCounts, mercurioOffsets, raggiomercurio,0);
    fillHemisphere(venereVertices, venereIndices, venereCounts, venereOffsets, raggiovenere,0);
    fillHemisphere(marteVertices, marteIndices, marteCounts, marteOffsets, raggiomarte,0);
    fillHemisphere(gioveVertices, gioveIndices, gioveCounts, gioveOffsets, raggiogiove,0);
    fillHemisphere(saturnoVertices, saturnoIndices, saturnoCounts, saturnoOffsets, raggiosaturno,0);
    fillHemisphere(uranoVertices, uranoIndices, uranoCounts, uranoOffsets, raggiourano,0);
    fillHemisphere(nettunoVertices, nettunoIndices, nettunoCounts, nettunoOffsets, raggionettuno,0);

    // Initializing each name
    scritta(Verticesgi);
    scritta(Verticeste);
    scritta(Verticesme);
    scritta(Verticesve);
    scritta(Verticesma);
    scritta(Verticessa);
    scritta(Verticesu);
    scritta(Verticesne);

    // Create VAOs and VBOs...
    glGenVertexArrays(17, vao);
    glGenBuffers(26, buffer);

    // Passing data to GPU, soociating data with vertex shader

    // SUN :
    glBindVertexArray(vao[SOLE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SOLE_VERTICES]);   // First we define SOLE_VERTICES type 
    glBufferData(GL_ARRAY_BUFFER, sizeof(soleVertices), soleVertices, GL_STATIC_DRAW);  // We allocate its space in the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SOLE_INDICES]);    // We do the same thing with SOLE_INDICES
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(soleIndices), soleIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(soleVertices[0]), 0);    // Passing coords 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(soleVertices[0]), (void*)sizeof(soleVertices[0].coords));    // passing normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(soleVertices[0]),
                          (void*)(sizeof(soleVertices[0].coords)+sizeof(soleVertices[0].normal)));  // passing texture coords
    glEnableVertexAttribArray(2);

    // EARTH :.
    glBindVertexArray(vao[TERRA]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[TERRA_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terraVertices), terraVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[TERRA_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(terraIndices), terraIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terraVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(terraVertices[0]), (void*)sizeof(terraVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(terraVertices[0]),
                          (void*)(sizeof(terraVertices[0].coords)+sizeof(terraVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // MERCURY :
    glBindVertexArray(vao[MERCURIO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[MERCURIO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mercurioVertices), mercurioVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[MERCURIO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mercurioIndices), mercurioIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(mercurioVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mercurioVertices[0]), (void*)sizeof(mercurioVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mercurioVertices[0]),
                          (void*)(sizeof(mercurioVertices[0].coords)+sizeof(mercurioVertices[0].normal)));
    glEnableVertexAttribArray(2);

   // VENUS :
    glBindVertexArray(vao[VENERE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VENERE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(venereVertices), venereVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[VENERE_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(venereIndices), venereIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(venereVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(venereVertices[0]), (void*)sizeof(venereVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(venereVertices[0]),
                          (void*)(sizeof(venereVertices[0].coords)+sizeof(venereVertices[0].normal)));
    glEnableVertexAttribArray(2);

     // MARS :
    glBindVertexArray(vao[MARTE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[MARTE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(marteVertices), marteVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[MARTE_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(marteIndices), marteIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(marteVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(marteVertices[0]), (void*)sizeof(mercurioVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mercurioVertices[0]),
                          (void*)(sizeof(mercurioVertices[0].coords)+sizeof(mercurioVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // GIOVE :
    glBindVertexArray(vao[GIOVE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[GIOVE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gioveVertices), gioveVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[GIOVE_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gioveIndices), gioveIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(gioveVertices[0]), 0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gioveVertices[0]), (void*)sizeof(gioveVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gioveVertices[0]),
                          (void*)(sizeof(gioveVertices[0].coords)+sizeof(gioveVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // SATURN :
    glBindVertexArray(vao[SATURNO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SATURNO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(saturnoVertices), saturnoVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SATURNO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(saturnoIndices), saturnoIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(saturnoVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(saturnoVertices[0]), (void*)sizeof(saturnoVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(saturnoVertices[0]),
                          (void*)(sizeof(saturnoVertices[0].coords)+sizeof(saturnoVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // URANUS :
    glBindVertexArray(vao[URANO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[URANO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uranoVertices), uranoVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[URANO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uranoIndices), uranoIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(uranoVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(uranoVertices[0]), (void*)sizeof(uranoVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(uranoVertices[0]),
                          (void*)(sizeof(uranoVertices[0].coords)+sizeof(uranoVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // NETTUNO :
    glBindVertexArray(vao[NETTUNO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[NETTUNO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nettunoVertices), nettunoVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[NETTUNO_INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nettunoIndices), nettunoIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(nettunoVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(nettunoVertices[0]), (void*)sizeof(nettunoVertices[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(nettunoVertices[0]),
                          (void*)(sizeof(nettunoVertices[0].coords)+sizeof(nettunoVertices[0].normal)));
    glEnableVertexAttribArray(2);

    // NAMES
    // We are doing the same thing with names (but we don't have indices)

    // JUPITER
    glBindVertexArray(vao[SGIOVE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SGIOVE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesgi), Verticesgi, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesgi[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesgi[0]), (void*)sizeof(Verticesgi[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesgi[0]),
                          (void*)(sizeof(Verticesgi[0].coords)+sizeof(Verticesgi[0].normal)));
    glEnableVertexAttribArray(2);

    // EARTH
    glBindVertexArray(vao[STERRA]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[STERRA_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticeste), Verticeste, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticeste[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticeste[0]), (void*)sizeof(Verticeste[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticeste[0]),
                          (void*)(sizeof(Verticeste[0].coords)+sizeof(Verticeste[0].normal)));
    glEnableVertexAttribArray(2);

    // MERCURY
    glBindVertexArray(vao[SMERCURIO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SMERCURIO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesme), Verticesme, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesme[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesme[0]), (void*)sizeof(Verticesme[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesme[0]),
                          (void*)(sizeof(Verticesme[0].coords)+sizeof(Verticesme[0].normal)));
    glEnableVertexAttribArray(2);

    //VENERE
    glBindVertexArray(vao[SVENERE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SVENERE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesve), Verticesve, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesve[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesve[0]), (void*)sizeof(Verticesve[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesve[0]),
                          (void*)(sizeof(Verticesve[0].coords)+sizeof(Verticesve[0].normal)));
    glEnableVertexAttribArray(2);

    // MARS
    glBindVertexArray(vao[SMARTE]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SMARTE_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesma), Verticesma, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesma[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesma[0]), (void*)sizeof(Verticesma[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesma[0]),
                          (void*)(sizeof(Verticesma[0].coords)+sizeof(Verticesma[0].normal)));
    glEnableVertexAttribArray(2);

    // SATURN
    glBindVertexArray(vao[SSATURNO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SSATURNO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticessa), Verticessa, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticessa[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticessa[0]), (void*)sizeof(Verticessa[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticessa[0]),
                          (void*)(sizeof(Verticessa[0].coords)+sizeof(Verticessa[0].normal)));
    glEnableVertexAttribArray(2);

    // URANUS
    glBindVertexArray(vao[SURANO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SURANO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesu), Verticesu, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesu[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesu[0]), (void*)sizeof(Verticesu[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesu[0]),
                          (void*)(sizeof(Verticesu[0].coords)+sizeof(Verticesu[0].normal)));
    glEnableVertexAttribArray(2);

    // NEPTUNE
    glBindVertexArray(vao[SNETTUNO]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[SNETTUNO_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verticesne), Verticesne, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Verticesne[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verticesne[0]), (void*)sizeof(Verticesne[0].coords));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Verticesne[0]),
                          (void*)(sizeof(Verticesne[0].coords)+sizeof(Verticesne[0].normal)));
    glEnableVertexAttribArray(2);

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
    image[0] = readBMP("./Texture/sole.bmp");
    image[1] = readBMP("./Texture/terra.bmp");
    image[2] = readBMP("./Texture/mercurio.bmp");
    image[3] = readBMP("./Texture/venere.bmp");
    image[4] = readBMP("./Texture/marte.bmp");
    image[5] = readBMP("./Texture/giove.bmp");
    image[6] = readBMP("./Texture/saturno.bmp");
    image[7] = readBMP("./Texture/urano.bmp");
    image[8] = readBMP("./Texture/nettuno.bmp");
    image[9] = readBMP("./NomePianeti/sgiove.bmp");
    image[10] = readBMP("./NomePianeti/sterra.bmp");
    image[11] = readBMP("./NomePianeti/smercurio.bmp");
    image[12] = readBMP("./NomePianeti/svenere.bmp");
    image[13] = readBMP("./NomePianeti/smarte.bmp");
    image[14] = readBMP("./NomePianeti/ssaturno.bmp");
    image[15] = readBMP("./NomePianeti/surano.bmp");
    image[16] = readBMP("./NomePianeti/snettuno.bmp");

    // Create texture ids.
    glGenTextures(17, texture);

    //TEXTURE :

    // SUN
    glActiveTexture(GL_TEXTURE0); // Activating texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture[0]); // Defining texture[0] type, 2D image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data); //Copying data from image[0] to texture[0]         
    // Setting Interpolation params, if texture coords are >1 or <0 the image will be repeated
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // We choose closest texel to the calcolated center
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    textureLoc = glGetUniformLocation(programId, "sole"); // Passing data to fragment shader
    glUniform1i(textureLoc, 0);

    // EARTH
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "terra");
    glUniform1i(textureLoc, 1);

    // MERCURY
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[2]->sizeX, image[2]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[2]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "mercurio");
    glUniform1i(textureLoc, 2);

    // VENUS:
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[3]->sizeX, image[3]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[3]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "venere");
    glUniform1i(textureLoc, 3);

    // MARS
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[4]->sizeX, image[4]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[4]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "marte");
    glUniform1i(textureLoc, 4);

    // JUPITER
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[5]->sizeX, image[5]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[5]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "giove");
    glUniform1i(textureLoc, 5);

    // SATURN
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[6]->sizeX, image[6]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[6]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "saturno");
    glUniform1i(textureLoc, 6);

    // URANUS
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[7]->sizeX, image[7]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[7]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "urano");
    glUniform1i(textureLoc, 7);

     // NEPTUNE
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[8]->sizeX, image[8]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[8]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "nettuno");
    glUniform1i(textureLoc, 8);

    //NAMES:

    // JUPITER:
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[9]->sizeX, image[9]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[9]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "sgiove");
    glUniform1i(textureLoc, 9);

    // EARTH:
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[10]->sizeX, image[10]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[10]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "sterra");
    glUniform1i(textureLoc, 10);

    // MERCURY:
    glActiveTexture(GL_TEXTURE11);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[11]->sizeX, image[11]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[11]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "smercurio");
    glUniform1i(textureLoc, 11);

    // VENUS:
    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[12]->sizeX, image[12]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[12]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "svenere");
    glUniform1i(textureLoc, 12);

    // MARS:
    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[13]->sizeX, image[13]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[13]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "smarte");
    glUniform1i(textureLoc, 13);

    // SATURN:
    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[14]->sizeX, image[14]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[14]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "ssaturno");
    glUniform1i(textureLoc, 14);

    //  URANUS:
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, texture[15]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[15]->sizeX, image[15]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[15]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "surano");
    glUniform1i(textureLoc, 15);

     // NEPTUNE:
    glActiveTexture(GL_TEXTURE16);
    glBindTexture(GL_TEXTURE_2D, texture[16]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[16]->sizeX, image[16]->sizeY, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image[16]->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    textureLoc = glGetUniformLocation(programId, "snettuno");
    glUniform1i(textureLoc, 16);

    // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

}

// Drawing routine.
void display(void)
{
    mat3 TMP;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate and update projection matrix.
    //angolo visuale in radianti, grandezza x-y piano vicino alla telecamera paino lontano(profondita)
    glm_perspective(9.0f, (float)width/(float)height, 1.0f, 100.0f, projMat);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *) projMat);

     // Calculate and update normal matrix.
    glm_mat4_pick3(modelViewMat, TMP);
    glm_mat3_inv(TMP, normalMat);
    glm_mat3_transpose(normalMat);
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, (GLfloat *)normalMat);

    // SUN:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
        (vec3){0.0, 5.0, 0.0}, modelViewMat);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, SOLE); // Update object name.
    glBindVertexArray(vao[SOLE]); // Binding datas from vao[SOLE]
    // processing vertices and showing the on screen with the primitive TRIANGLE_STRIP
    glMultiDrawElements(GL_TRIANGLE_STRIP, soleCounts, GL_UNSIGNED_INT, (const void **)soleOffsets, HEM_LATS);

    // EARTH:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 35, 30 , 340, modelViewMat, &x[0], &y[0]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, TERRA); // Update object name.
    glBindVertexArray(vao[TERRA]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, terraCounts, GL_UNSIGNED_INT, (const void **)terraOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, STERRA);
    glBindVertexArray(vao[STERRA]);
    glm_translate(modelViewMat, (vec3){-5.0, 2.0, -1.0}); // Making a translation so the name is not in the same position as the planet
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    // processing vertices and showing the on screen with the primitive TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // MERCURY:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 25, 20 , 200, modelViewMat, &x[1], &y[1]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, MERCURIO); // Update object name.
    glBindVertexArray(vao[MERCURIO]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, mercurioCounts, GL_UNSIGNED_INT, (const void **)mercurioOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SMERCURIO);
    glBindVertexArray(vao[SMERCURIO]);
    glm_translate(modelViewMat, (vec3){-5.0, 2.0, -2.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // VENUS:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 30, 25 , 280, modelViewMat,&x[2], &y[2] );
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, VENERE); // Update object name.
    glBindVertexArray(vao[VENERE]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, venereCounts, GL_UNSIGNED_INT, (const void **)venereOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SVENERE);
    glBindVertexArray(vao[SVENERE]);
    glm_translate(modelViewMat, (vec3){-5.0, 2.0, -3.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // MARS:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 40, 35 , 400, modelViewMat, &x[3], &y[3]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, MARTE); // Update object name.
    glBindVertexArray(vao[MARTE]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, marteCounts, GL_UNSIGNED_INT, (const void **)marteOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SMARTE);
    glBindVertexArray(vao[SMARTE]);
    glm_translate(modelViewMat, (vec3){-5.0, 2.0, -4.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // JUPITER:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 52, 50 , 700, modelViewMat, &x[4], &y[4]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, GIOVE); // Update object name.
    glBindVertexArray(vao[GIOVE]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, gioveCounts, GL_UNSIGNED_INT, (const void **)gioveOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SGIOVE);
    glBindVertexArray(vao[SGIOVE]);
    glm_translate(modelViewMat, (vec3){-3.0, 15.0, -2.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // SATURN:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 80, 75 , 1000,  modelViewMat, &x[5], &y[5]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, SATURNO); // Update object name.
    glBindVertexArray(vao[SATURNO]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, saturnoCounts, GL_UNSIGNED_INT, (const void **)saturnoOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SSATURNO);
    glBindVertexArray(vao[SSATURNO]);
    glm_translate(modelViewMat, (vec3){-3.0, 15.0, -2.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

     //URANO
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 105, 95 , 1500, modelViewMat, &x[6], &y[6]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, URANO); // Update object name.
    glBindVertexArray(vao[URANO]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, uranoCounts, GL_UNSIGNED_INT, (const void **)uranoOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SURANO);
    glBindVertexArray(vao[SURANO]);
    glm_translate(modelViewMat, (vec3){-5.0, 8.0, -2.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // NEPTUNE:
    glm_mat4_identity(modelViewMat);
    glm_lookat((vec3){0.0, 0.0, P}, (vec3){0.0, 0.0, 0.0},
    (vec3){0.0, 5.0, 0.0}, modelViewMat);
    MoveEllipse( 125, 110 , 1900, modelViewMat, &x[7], &y[7]);
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glUniform1ui(objectLoc, NETTUNO); // Update object name.
    glBindVertexArray(vao[NETTUNO]);
    glMultiDrawElements(GL_TRIANGLE_STRIP, nettunoCounts, GL_UNSIGNED_INT, (const void **)nettunoOffsets, HEM_LATS);
    // NAME
    glUniform1ui(objectLoc, SNETTUNO);
    glBindVertexArray(vao[SNETTUNO]);
    glm_translate(modelViewMat, (vec3){-5.0, 8.0, -2.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)(modelViewMat));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}
// Updating scene every 50 milliseconds
void tempo(int y)
{
    glutPostRedisplay();
    glutTimerFunc (50 ,tempo, 0);
}

// Main routine.
int main(int argc, char **argv)
{
    GLenum glErr;

    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Sistema Solare");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc (50 ,tempo, 0);
    glewInit();

    // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    setup();
    glutMainLoop();

    return 0;
}
