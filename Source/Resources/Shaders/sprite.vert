#version 330 core
layout (location = 0) in vec4 vertex; // x,y = Position, z,w = TexCoords

out vec2 TexCoords;
out vec2 Pos;

uniform mat4 PM;

void main()
{
    TexCoords = vertex.zw;
    Pos = (PM * vec4(vertex.xy, 0.0, 1.0)).xy;

    gl_Position = PM * vec4(vertex.xy, 0.0, 1.0);
}