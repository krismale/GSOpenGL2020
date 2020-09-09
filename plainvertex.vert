#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;
layout(location = 2) in vec2 texCoordAttr;
out vec4 col;
out vec2 texCoord;
uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;


void main() {
    texCoord = texCoordAttr;
    col = colAttr;
    gl_Position = pMatrix * vMatrix * matrix * posAttr;
}




