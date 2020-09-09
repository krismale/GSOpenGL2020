#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"
#include "Dependencies/stb_image.h"
#include "shader.h"
#include "MyStructs.h"

// Defineres for å unngå circular include
class Collider;
class Terrain;
class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject();
    ~VisualObject();
    virtual void init(MyStructs::ShaderUniforms shaderUniforms)=0;
    virtual void draw()=0;

    // Transformasjonsfunksjoner for rotasjon, skalering og translering
    virtual void move(float dx, float dy, float dz) {}
    virtual void moveTo(float x, float y, float z) {}
    virtual void rotate(float tilt, float yaw, float roll) {}
    virtual void scale(float dx, float dy, float dz) {}

    // Setter transformasjon på objektet og alle komponentene dens
    virtual void setTranslationForAll(float x, float y, float z) {}
    virtual void setRotationForAll(float roll, float yaw, float pitch) {}
    virtual void setScaleForAll(float x, float y, float z) {}

    // PLasserer objektet på toppen av ground
    virtual void placeOnTopOfGround(Terrain* ground, float addedHeight = 0) {}
    virtual void setColor(QVector3D color) {}

    // Genererer tekstur på objektet
    virtual void generateTexture() {}

    // Setter ID til objektet, for å putte det i outlineren
    virtual void setID(int id) {}

    // Transformasjonen til objektet
    MyStructs::Transformation mTransformation;

    // 0: Holder på høyden som objektet har før noen scaling er utført
    // 1: Holder på høyden som objektet har akkurat nå, etter scaling
    float mHeight[2];

    // 0: Holder på bredden som objektet har før noen scaling er utført
    // 1: Holder på bredden som objektet har akkurat nå, etter scaling
    float mXWidth[2];

    // 0: Holder på dybden som objektet har før noen scaling er utført
    // 1: Holder på dybden som objektet har akkurat nå, etter scaling
    float mZDepth[2];

    bool mVisible{true}; // Bestemmer om objektet skal rendres eller ikke
    bool mDrawDebugLines{false}; // Hvis true, så tegnes objektet kun med linjer

    std::vector<Vertex> mVertices; // Array som holder på vertekser, for å tegne objektet
    std::vector<unsigned int> mIndices; // tre og tre tall viser til hvilken indekser i mVertices som skal danne trekanter

    std::vector<MyStructs::Texture> mTexture; // Alle vanlige teksturer
    MyStructs::Texture mColorBasedRenderTexture; // En fargebasert tekstur til phong shadern

    std::vector<Collider*> mColliders; // Colliderne til objektet

    QMatrix4x4 mMatrix; // Model matrixen

    std::string mLabel = "VisualObject"; // Objektets label i outlineren
    int mID = 1001; // IDen til objektet. Starter på 1001 fordi 1000 er reservert til kamera

protected:


    unsigned int mIBO;
    GLuint mVAO{0};
    GLuint mVBO{0};

    QVector3D mObjectColor = QVector3D(0.7f,0.7f,0.7f); // Fargen til objektet
    float mSpecularStrength = 0.5f; // Hvor mye den skal lyses opp av specular faktoren fra lyset
    int mSpecularExponent = 64; // Hvor eksponert objektet er for specular lys

    MyStructs::ShaderUniforms mShaderUniforms; // Objektets shader uniforms
};

#endif // VISUALOBJECT_H
