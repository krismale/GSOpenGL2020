#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "visualobject.h"
#include "terrain.h"
#include "MyMath/misccalculations.h"

class Billboard : public VisualObject
{
public:
    Billboard();
    Billboard(const char *texture);
    void init(MyStructs::ShaderUniforms shaderUniforms) override;
    void draw() override;
    void generateTexture() override;

    void move(float dx, float dy, float dz) override;
    void scale(float dx, float dy, float dz) override;
    void placeOnTopOfGround(Terrain *ground, float addedHeight) override;

    void setTranslation(float x, float y, float z);
    void setScale(float dx, float dy, float dz);

    // Hvis false så vil den kun rotere mot kameraet i yaw
    // Hvis true som vil den også rotere mot kameraet i tilt
    bool mUiElement = false;
    QVector3D mRightVector = QVector3D(1,0,0);
    QVector3D mForwardVector = QVector3D(0,0,-1);

//    GLint mObjectCenterUniform;
//    GLint mObjectSizeUniform;
//    GLint mUiElementUniform;



};

#endif // BILLBOARD_H
