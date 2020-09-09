#ifndef SKYBOX_H
#define SKYBOX_H

#include "visualobject.h"

class Skybox : public VisualObject
{
public:
    Skybox();
    Skybox(std::vector<std::string> skyboxFaces);
    void init(MyStructs::ShaderUniforms shaderUniforms) override;
    void draw() override;

    void generateTexture() override; // Lager CUBE_MAP texture til skyboxen
};

#endif // SKYBOX_H
