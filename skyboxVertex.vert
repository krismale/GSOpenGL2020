#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform vec3 camCenter;
uniform vec3 objectSize;

void main()
{
    vec3 vertexPos = camCenter + (aPos * objectSize);

    TexCoords = aPos;
    gl_Position = pMatrix * vMatrix * vec4(vertexPos, 1.0);
}




