#version 330 core
out vec4 fragColor;

in vec3 normalTransposed;
in vec3 fragmentPosition;
in vec2 texCoord;

uniform float ambientStrength = 0.3f;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float lightStrength = 0.7f;
uniform vec3 objectColor = vec3(0.7f,0.7f,0.7f);
uniform vec3 ambientCol = vec3(0.8f, 0.8f, 0.3f);

uniform float lightPower = 0.5f;
uniform float specularStrength = 0.5f;
uniform int specularExponent = 64;

uniform bool uiElement;


uniform sampler2D ourTexture;

void main()
{
    vec4 texResult = texture(ourTexture, texCoord).rgba;
    if(texResult.a < 0.1) {
        discard;
    }

    if(uiElement) {
        fragColor = texResult;
    } else {

        //ambient
        vec3 ambient = ambientCol * ambientStrength;

        //diffuse
        vec3 normalCorrected = normalize(normalTransposed);
        vec3 lightDirection = normalize(lightPosition - fragmentPosition);
        float angleFactor = max(dot(normalCorrected, lightDirection), 0.0);
        vec3 diffuse = objectColor * ambientCol * lightStrength * angleFactor;
        vec3 viewDirection = normalize(cameraPosition - fragmentPosition);

        // Specular
        float spec = 0.0;
        if(angleFactor > 0.0) {
            vec3 reflectDirection = reflect(-lightDirection, normalCorrected);
            spec = pow(max(dot(viewDirection, reflectDirection), 0.0), specularExponent);
        }
        vec3 specular = spec * ambientCol * specularStrength;

        vec4 lightResult = vec4(vec3(ambient + diffuse + specular),1);

        fragColor = texResult * lightResult;
    }
}



