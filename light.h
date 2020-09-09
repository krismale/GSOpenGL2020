#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"
#include "shader.h"
#include <QVector4D>
#include <QMatrix4x4>

class Light : public VisualObject
{
public:
    Light();
    void init(MyStructs::ShaderUniforms shaderUniforms) override {}
    void init(MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms meshShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms);
    void draw() override; // Tegner selve lyspæra. Rendres ved hjelp av rgb shaderen
    void renderLight(); // Rendrer lyset i henhold til phong shader. Rendres da av phong shaderen
    void renderLightBillboard(); // Rendrer lyset i henhold til billboard shader. Rendres da av billboard shaderen

    // Transform funksjoner
    // Her gjør egentlig for eksempel setTranslation og setTranslationForAll akkurat det samme
    // Grunnen til at begge er implementert er fordi det gjør det lettere for translasjon gjennom outlineren
    void setTranslation(float x, float y, float z);
    void setScale(float dx, float dy, float dz);
    void setTranslationForAll(float x, float y, float z) override;
    void setScaleForAll(float x, float y, float z) override;
    void setRotationForAll(float roll, float yaw, float pitch) override;

    // Forflytter lyset
    void move(float dx, float dy, float dz) override;

    // Shader uniforms
    // Har mange individuelle structs per shader her fordi jeg vil ha et tydelig skille mellom de
    // Jeg får bedre oversikt enn om jeg skulle ha puttet disse inn i et array, når jeg har individuelle navn
    MyStructs::ShaderUniforms mPhongShaderUniforms;
    MyStructs::ShaderUniforms mBillboardShaderUniforms;

    // Variabler som styrer hvordan lyset påvirker scenen
    float mLightStrength = 0.7f;
    float mLightPower = 1.5f;

    // Brukes for å sette IDen til objektet i outlineren
    void setID(int id) override {
        mID = id;
    }
};

#endif // LIGHT_H
