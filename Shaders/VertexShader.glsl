#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTexCoord;

out vec3 vertexColor;
out vec3 TexCoord;

uniform mat4x4 perspectiveMat;

void main()
{
	gl_Position = perspectiveMat * vec4(aPos.xyz, 1.0f);
	vertexColor = aColor;
	TexCoord = aTexCoord;
}