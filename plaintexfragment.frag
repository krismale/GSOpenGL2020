#version 330 core

in vec2 texCoord;
out vec4 fragTex;

uniform sampler2D ourTexture;

void main() {

    vec4 fragCol = texture(ourTexture, texCoord).rgba;
    if(fragCol.a < 0.1) {
        discard;
    }
    fragTex = fragCol;

}



