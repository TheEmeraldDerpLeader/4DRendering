#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 TexCoord;

uniform sampler3D texture0;

void main()
{
	FragColor = texture(texture0, TexCoord) * vec4(vertexColor,1.0f);
}