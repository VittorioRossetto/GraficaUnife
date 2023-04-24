
#include "scritta.h"

// Fill the vertex array with co-ordinates of the sample points.
void scritta(Vertex discVertices[4])
{
      discVertices[0].coords.x =0;
      discVertices[0].coords.y = 0;
      discVertices[0].coords.z = 1.0;
      discVertices[0].coords.w = 1.0;
      discVertices[0].normal.x = 0.0;
      discVertices[0].normal.y = 0.0;
      discVertices[0].normal.z = 1.0;
      discVertices[0].texCoords.x =0;
      discVertices[0].texCoords.y =0;
      
      discVertices[1].coords.x = 6;
      discVertices[1].coords.y = 0;
      discVertices[1].coords.z = 1.0;
      discVertices[1].coords.w = 1.0;
      discVertices[1].normal.x = 0.0;
      discVertices[1].normal.y = 0.0;
      discVertices[1].normal.z = 1.0;
      discVertices[1].texCoords.x =1;
      discVertices[1].texCoords.y =0;
      
      discVertices[2].coords.x = 6;
      discVertices[2].coords.y = 4;
      discVertices[2].coords.z = 1.0;
      discVertices[2].coords.w = 1.0;
      discVertices[2].normal.x = 0.0;
      discVertices[2].normal.y = 0.0;
      discVertices[2].normal.z = 1.0;
      discVertices[2].texCoords.x =1;
      discVertices[2].texCoords.y =1;
      
      discVertices[3].coords.x = 0;
      discVertices[3].coords.y = 4;
      discVertices[3].coords.z = 1.0;
      discVertices[3].coords.w = 1.0;
      discVertices[3].normal.x = 0.0;
      discVertices[3].normal.y = 0.0;
      discVertices[3].normal.z = 1.0;
      discVertices[3].texCoords.x =0;
      discVertices[3].texCoords.y =1; 
}


