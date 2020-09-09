#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoordAttr;

out vec3 fragmentPosition;
out vec3 normalTransposed;
out vec2 texCoord;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;


void main() {
    fragmentPosition = vec3(matrix * vec4(vertexPosition, 1.0));
    normalTransposed = mat3(transpose(inverse(matrix))) * vertexNormal;
    texCoord = texCoordAttr;

    gl_Position = pMatrix * vMatrix * matrix * vec4(vertexPosition, 1.0);
}
