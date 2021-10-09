#version 330 core
/* Attributes */
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

/* Output */
out VERT_OUT
{
    vec2 TexCoords;
    vec3 WorldPos;
    vec3 Normal;
} vertOut;

uniform mat4 PV;
uniform mat4 model;

void main()
{
    vertOut.TexCoords = aTexCoords;
    vertOut.WorldPos = vec3(model * vec4(aPosition, 1.0));
    vertOut.Normal = mat3(model) * aNormal;

    gl_Position =  PV * vec4(vertOut.WorldPos, 1.0);
}