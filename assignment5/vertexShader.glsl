#version 430 core
//Uno per pianeta
#define SOLE 0
#define TERRA 1
#define MERCURIO 2
#define VENERE 3
#define MARTE 4
#define GIOVE 5
#define SATURNO 6
#define URANO 7
#define NETTUNO 8
//Uno per scritta
#define SGIOVE 9
#define STERRA 10
#define SMERCURIO 11
#define SVENERE 12
#define SMARTE 13
#define SSATURNO 14
#define SURANO 15
#define SNETTUNO 16

layout(location=0) in vec4 Coords;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
out vec2 texCoordsExport;

struct Light
{
    vec4 ambCols;
    vec4 difCols;
    vec4 specCols;
    vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;

struct Material
{
    vec4 ambRefl;
    vec4 difRefl;
    vec4 specRefl;
    vec4 emitCols;
    float shininess;
};
uniform Material canFandB;

vec3 normal, lightDirection, eyeDirection, halfway;
vec4 frontEmit, frontGlobAmb, frontAmb, frontDif, frontSpec,
     backEmit, backGlobAmb, backAmb, backDif, backSpec;
vec4 coords;

void main(void)
{
    // Here we select which object we are plotting
    if (object == SOLE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == TERRA) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == MERCURIO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
      if (object == VENERE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
  
       if (object == MARTE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
        
    }
       if (object == GIOVE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
       if (object == SATURNO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == URANO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == NETTUNO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
	if (object == SGIOVE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == STERRA) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == SMERCURIO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
     if (object == SVENERE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == SMARTE) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == SSATURNO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
     if (object == SURANO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    if (object == SNETTUNO) {
        coords = Coords;
        normal = Normal;
        texCoordsExport = TexCoords;
    }
    // object normal
    normal = normalize(normalMat * normal);
    // direction of the light
    lightDirection = normalize(vec3(light0.coords));
    // view direction
    eyeDirection = -1.0f * normalize(vec3(modelViewMat * coords));
    halfway = (length(lightDirection + eyeDirection) == 0.0f) ?
              vec3(0.0) : (lightDirection + eyeDirection)/
              length(lightDirection + eyeDirection);

    // PHONG Model - FRONT FACE
    // That is, emitted light and global, diffuse and specular
    // components.
    frontEmit = canFandB.emitCols;
    frontGlobAmb = globAmb * canFandB.ambRefl;
    frontAmb = light0.ambCols * canFandB.ambRefl;
    frontDif = max(dot(normal, lightDirection), 0.0f) *
        (light0.difCols * canFandB.difRefl);
    frontSpec = pow(max(dot(normal, halfway), 0.0f),
                    canFandB.shininess) * (light0.specCols * canFandB.specRefl);
    frontAmbDiffExport =  vec4(vec3(min(frontEmit + frontGlobAmb +
        frontAmb + frontDif, vec4(1.0))), 1.0);
    frontSpecExport =  vec4(vec3(min(frontSpec, vec4(1.0))), 1.0);

    // PHONG Model - BACK FACE
    // That is, emitted light and global, diffuse and specular
    // components.
    normal = -1.0f * normal;
    backEmit = canFandB.emitCols;
    backGlobAmb = globAmb * canFandB.ambRefl;
    backAmb = light0.ambCols * canFandB.ambRefl;
    backDif = max(dot(normal, lightDirection), 0.0f) *
        (light0.difCols * canFandB.difRefl);
    backSpec = pow(max(dot(normal, halfway), 0.0f),
        canFandB.shininess) * (light0.specCols * canFandB.specRefl);
    backAmbDiffExport =  vec4(vec3(min(backEmit + backGlobAmb + backAmb +
        backDif, vec4(1.0))), 1.0);
    backSpecExport =  vec4(vec3(min(backSpec, vec4(1.0))), 1.0);

    gl_Position = projMat * modelViewMat * coords;
}
