#ifndef HEMISPHERE_H_INCLUDED
#define HEMISPHERE_H_INCLUDED

#include <math.h>
#include "vertex.h"

// #define PI 3.14159265
#define HEM_LONGS 30 // Number of longitudinal slices.
#define HEM_LATS 30 // Number of latitudinal slices.



void fillHemVertexArray(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)], float raggio,int flag);
void fillHemIndices(unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)]);
void fillHemCounts(int hemCounts[HEM_LATS]);
void fillHemOffsets(void* hemOffsets[HEM_LATS]);

void fillHemisphere(Vertex hemVertices[(HEM_LONGS + 1) * (HEM_LATS + 1)],
	         unsigned int hemIndices[HEM_LATS][2*(HEM_LONGS+1)],
			 int hemCounts[HEM_LATS],
			 void* hemOffsets[HEM_LATS],float raggio, int flag);



#endif // HEMISPHERE_H_INCLUDED
