#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoordAttr;
out vec2 texCoord;
out vec3 fragmentPosition;
out vec3 normalTransposed;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 matrix;
uniform vec3 camCenter;
uniform vec3 objectSize;
uniform vec3 objectCenter;
uniform bool uiElement;


void main() {
    vec3 CameraRight = {vMatrix[0][0], vMatrix[1][0], vMatrix[2][0]};
    vec3 CameraUp = {vMatrix[0][1], vMatrix[1][1], vMatrix[2][1]};

    vec3 camForward = normalize(cross(CameraRight, CameraUp));

    vec3 vertexPos;



    if(uiElement) {
        vertexPos = camCenter - (camForward * 0.7)
                             + CameraRight * posAttr.x * objectSize.x
                             + CameraUp * posAttr.y * objectSize.y;
    } else {
        vertexPos.x = objectCenter.x + CameraRight.x * posAttr.x * objectSize.x;
        vertexPos.y = objectCenter.y + posAttr.y;
        vertexPos.z = objectCenter.z + CameraRight.z * posAttr.x * objectSize.x;
    }

    fragmentPosition = vec3(matrix * vec4(vertexPos, 1.0f));
    normalTransposed = mat3(transpose(inverse(matrix))) * camForward;
    texCoord = texCoordAttr;

    gl_Position = pMatrix * vMatrix * vec4(vertexPos, 1.0f);

}




