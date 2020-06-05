#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 TexCoord;

uniform sampler3D texture0;

void main()
{
	vec4 texColor = texture(texture0, TexCoord) * vec4(vertexColor,1.0f);
	if(texColor.a< 0.1f)
		discard;
	FragColor =  texColor;
}