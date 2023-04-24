#version 430 core
//Pianeti
#define SOLE 0
#define TERRA 1
#define MERCURIO 2
#define VENERE 3
#define MARTE 4
#define GIOVE 5
#define SATURNO 6
#define URANO 7
#define NETTUNO 8
//Scritte per pianeti
#define SGIOVE 9
#define STERRA 10
#define SMERCURIO 11
#define SVENERE 12
#define SMARTE 13
#define SSATURNO 14
#define SURANO 15
#define SNETTUNO 16
// Material's properties
in vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
// texture coordinates
in vec2 texCoordsExport;
// samplers for the textures for planes
uniform sampler2D sole;
uniform sampler2D terra;
uniform sampler2D mercurio;
uniform sampler2D venere;
uniform sampler2D marte;
uniform sampler2D giove;
uniform sampler2D saturno;
uniform sampler2D urano;
uniform sampler2D nettuno;
// samplers for the textures for linee
uniform sampler2D sgiove;
uniform sampler2D sterra;
uniform sampler2D smercurio;
uniform sampler2D svenere;
uniform sampler2D smarte;
uniform sampler2D ssaturno;
uniform sampler2D surano;
uniform sampler2D snettuno;

uniform uint object;

out vec4 colorsOut;

vec4 texColor;

void main(void)
{
    //Pianeti
    if (object == SOLE) texColor = texture(sole, texCoordsExport);
    if (object == TERRA) texColor = texture(terra, texCoordsExport);
    if (object == MERCURIO) texColor = texture(mercurio, texCoordsExport);
    if (object == VENERE) texColor = texture(venere, texCoordsExport);
    if (object == MARTE) texColor = texture(marte, texCoordsExport);
    if (object == GIOVE) texColor = texture(giove, texCoordsExport);
    if (object == SATURNO) texColor = texture(saturno, texCoordsExport);
    if (object == URANO) texColor = texture(urano, texCoordsExport);
    if (object == NETTUNO) texColor = texture(nettuno, texCoordsExport);
    //Scritte
    if (object == SGIOVE) texColor = texture(sgiove, texCoordsExport);
    if (object == STERRA) texColor = texture(sterra, texCoordsExport);
    if (object == SMERCURIO) texColor = texture(smercurio, texCoordsExport);
    if (object == SVENERE) texColor = texture(svenere, texCoordsExport);
    if (object == SMARTE) texColor = texture(smarte, texCoordsExport);
    if (object == SSATURNO) texColor = texture(ssaturno, texCoordsExport);
    if (object == SURANO) texColor = texture(surano, texCoordsExport);
    if (object == SNETTUNO) texColor = texture(snettuno, texCoordsExport);
    // set color of the fragment
    colorsOut = gl_FrontFacing? (frontAmbDiffExport * texColor + frontSpecExport) :
                               (backAmbDiffExport * texColor + backSpecExport);
}
