#version 330 core

// Attributes
layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoords;
layout(location=3) in vec3 Tangent;
layout(location=4) in vec3 Bitangent;

#define MAX_LIGHTS 10

// Vertex output
out VERT_OUT
{
	vec2 TexCoords;	
	float texH;
	flat int numLights;

	// Tangent-space
	vec3 FragPos;
	vec3 LightPos[MAX_LIGHTS];
	vec3 LightDir[MAX_LIGHTS];
	vec3 ViewPos;
} vertOut;



// Uniforms
uniform mat4 PV;
uniform mat4 model;
uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightDir[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;

void main()
{				
	vec4 worldPosition = model * vec4(Position, 1.f);

	vertOut.FragPos = worldPosition.xyz;
	vertOut.TexCoords = TexCoords;
	vertOut.numLights = numLights;	
		
	vec3 T =   normalize(vec3(model * vec4(Tangent, 0.f)));
	vec3 B =   normalize(vec3(model * vec4(Bitangent, 0.f)));
	vec3 N =   normalize(vec3(model * vec4(Normal, 0.f)));
	mat3 TBN = transpose(mat3(T,B,N));
		
	vertOut.FragPos = TBN * vertOut.FragPos;
	for(int i = 0; i < numLights; i++)
	{
		vertOut.LightPos[i] = TBN * lightPos[i];
		vertOut.LightDir[i] = TBN * lightDir[i];
	}
	vertOut.ViewPos = TBN * viewPos;

	vertOut.texH = (Position.y + 25.f) / 50.f;

	gl_Position = PV * worldPosition;
}
