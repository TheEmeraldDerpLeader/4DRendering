#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTexCoord;
layout (location = 3) in vec4 aNormals;

out vec3 vertexColor;
out vec3 texCoord;
out vec4 normal;
out vec3 fragPos;

uniform mat4x4 perspectiveMat;

void main()
{
	gl_Position = perspectiveMat * vec4(aPos, 1.0);
	vertexColor = aColor;
	texCoord = aTexCoord;
	if (aNormals != vec4(0,0,0,0))
		normal = normalize(aNormals);
	else
		normal = aNormals;
	fragPos = aPos;
}