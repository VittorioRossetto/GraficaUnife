#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader.h"
#include "light.h"
#include "material.h"
#include "readBMP.h"
#include "hemisphere.h"
#include "funzioni.h"
/*
    Function to implement movement on an elipse, used for both planet and names (that follow planets),
    we calcolate position using the rays x and y, and then we update movelViewMat
*/
void MoveEllipse(float rx, float ry,int num_segments,mat4 modelViewMat, float *x, float *y)
{
    float theta = ((2 * 3.1415926)/num_segments);
    float c = cosf(theta);
    float s = sinf(theta);
    float t=0;
    vec3 posizione;

    t = *x;
    *x = c * (*x) - s * (*y);
    *y = s * t + c * (*y);

    posizione[0]= (*x) * rx;
    posizione[1]= (*y) * ry;
    posizione[2]= 0;

    glm_translate(modelViewMat, posizione);
    glEnd();
}
