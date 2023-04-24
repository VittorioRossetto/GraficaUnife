#version 430 core

#define HEMISPHERE 0
#define CYLINDER1 1
#define CYLINDER2 2
#define TITLE 3

// Material's properties
in vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
// texture coordinates
in vec2 texCoordsExport;

// samplers for the textures
uniform sampler2D blueWoodTex;
uniform sampler2D redWoodTex;
uniform sampler2D ballTex;
uniform sampler2D titleScreen;

uniform uint object;
uniform vec4 hemColor;

out vec4 colorsOut;

vec4 texColor;

void main(void)
{
    // check which object we are currently drawing
    if (object == CYLINDER1) texColor = texture(redWoodTex, texCoordsExport);
    if (object == CYLINDER2) texColor = texture(blueWoodTex, texCoordsExport);
    if (object == HEMISPHERE) texColor = hemColor;
    if (object == TITLE) texColor = texture(titleScreen, texCoordsExport);

    // set color of the fragment
    colorsOut = gl_FrontFacing? (frontAmbDiffExport * texColor + frontSpecExport) :
                               (backAmbDiffExport * texColor + backSpecExport);

    
}