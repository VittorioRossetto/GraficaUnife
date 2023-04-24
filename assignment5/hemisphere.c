#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

#include "hemisphere.h"

// Fill the vertex array with co-ordinates of the sample points.
// We use variable raggio to determine dimension for each hemisphere
void fillHemVertexArray(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)],float raggio, int flag) {
    int i, j, k;
    k = 0;
    for (j = 0; j <= HEM_LATS; j++)
        for (i = 0; i <= HEM_LONGS; i++) {
            hemVertices[k].coords.x = raggio * sin( (float)j/HEM_LATS * M_PI ) *
                sin( 2.0 * (float)i/HEM_LONGS * M_PI );

            hemVertices[k].coords.y = raggio * cos( (float)j/HEM_LATS * M_PI );

            hemVertices[k].coords.z = raggio * sin( (float)j/HEM_LATS * M_PI ) *
                cos( 2.0 * (float)i/HEM_LONGS * M_PI );
            hemVertices[k].coords.w = 1.0;

	         hemVertices[k].coords.w = 1.0;
		if(flag==1){
            hemVertices[k].normal.x = hemVertices[k].coords.x;
            hemVertices[k].normal.y = hemVertices[k].coords.y;
            hemVertices[k].normal.z = hemVertices[k].coords.z;
	}
	else{
	hemVertices[k].normal.x = -hemVertices[k].coords.x;
            hemVertices[k].normal.y = -hemVertices[k].coords.y;
            hemVertices[k].normal.z = -hemVertices[k].coords.z;
	}
             hemVertices[k].texCoords.x = (float)i/HEM_LONGS;
             hemVertices[k].texCoords.y = (float)j/HEM_LATS;
		 k++;
        }
}

// Fill the array of index arrays.
void fillHemIndices(unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)]) {
    int i, j;
    for(j = 0; j < HEM_LATS; j++)
    {
        for (i = 0; i <= HEM_LONGS; i++)
        {
            hemIndices[j][2*i] = (j+1)*(HEM_LONGS + 1) + i;
            hemIndices[j][2*i+1] = j*(HEM_LONGS + 1) + i;
        }
    }
}

// Fill the array of counts.
void fillHemCounts(int hemCounts[HEM_LATS]) {
    int j;
    for(j = 0; j < HEM_LATS; j++) hemCounts[j] = 2*(HEM_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillHemOffsets(void* hemOffsets[HEM_LATS]) {
    int j;
    for(j = 0; j < HEM_LATS; j++) hemOffsets[j] =
        (GLvoid*)(2*(HEM_LONGS+1)*j*sizeof(unsigned int));
}

// Initialize the hemisphere.
void fillHemisphere(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)],
                    unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)],
                    int hemCounts[HEM_LATS],
                    void* hemOffsets[HEM_LATS],float raggio, int flag) {
    fillHemVertexArray(hemVertices,raggio,flag);
    fillHemIndices(hemIndices);
    fillHemCounts(hemCounts);
    fillHemOffsets(hemOffsets);
}

