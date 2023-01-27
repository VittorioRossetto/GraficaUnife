#include "Sphere.h"

const float PI = 3.14159265358979323846;
float radius = 2.0f, sectorCount = 72, stackCount = 24; // da definire
float x, y, z, xy;              // vertex position
float nx, ny, nz, lengthInv;    // vertex normal    lengthInv = 1.0f / radius;
float s, t;                     // vertex texCoord

float sectorStep, stackStep, sectorAngle, stackAngle;

void clearVertices() {

}

void addVertices(float x, float y, float z) {
    // Store the vertices that will be used for the sphere rendering
}

void addNormals(float nx, float ny, float nz) {
    // Store the vertices that will be used for the sphere rendering
}

void addTexCoords(float s, float t) {
    // Store the vertices that will be used for the sphere rendering
}

void addIndices() {
    
}

void addLineIndices() {
    
}


void initSphere(){
    sectorStep = 2* PI / sectorCount;
    stackStep = PI / stackCount;

    for(int i = 0; i <= stackCount; i++) {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectorCount; j++) {
            sectorAngle = j * sectorStep;   // from 0 to 2pi

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            addVertices(x, y, z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormals(nx, ny, nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoords(s, t);

        }
    }

    int indices[10000], lineIndices[10000];
    int k1, k2;

    for (int i = 0; i < sectorCount; i++) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; j++, k1++, k2++) {
            // 2 triangles in each sector including first and last stack
            // k1 => k2 => k1 + 1 

            if(i != 0) {
                addIndices(k1);
                addIndices(k2);
                addIndices(k1 + k1);
            }

            if(i != (stackCount -1)) {
                addIndices(k1 + 1);
                addIndices(k2);
                addIndices(k1 + k1);
            }

            addLineIndices(k1);
            addLineIndices(k2);
            if( i != 0 ) {
                addLineIndices(k1);
                addLineIndices(k1 + 1);
            }
        }
    }
}

void drawSphere() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 32,);
}
