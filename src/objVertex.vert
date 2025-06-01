#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TextCoord;
out mat3 TBN;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TextCoord = aTextCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vec3 N = normalize(normalMatrix * aNormal);
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 B = normalize(normalMatrix * aBitangent);

	TBN = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}