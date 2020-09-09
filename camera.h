#ifndef CAMERA_H
#define CAMERA_H
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"
#include "shader.h"
#include "MyStructs.h"
#include "trianglesurface.h"
#include "ui.h"
#include "MyMath/matrix4x4.h"

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();

    void init(MyStructs::ShaderUniforms rgbShaderUniforms, MyStructs::ShaderUniforms texShaderUniforms,
              MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms,
              MyStructs::ShaderUniforms skyboxShaderUniforms);

    // Render funksjoner
    void renderCol(float windowAspectRatio = 4/3);
    void renderTex(float windowAspectRatio = 4/3);
    void renderSkybox(float windowAspectRatio = 4/3);
    void renderPhong(float windowAspectRatio = 4/3);
    void renderBillboard(float windowAspectRatio = 4/3);

    // Movement og rotasjon for debug-mode kamera
    void moveCamera(QVector3D cameraPosition, QVector3D cameraLookAt, QVector3D cameraUpVector);
    void rotateWithMouse(float xPos, float yPos);

    // Movement og rotasjon for target/game-mode
    void move(float dx, float dy, float dz);
    void move(QVector3D deltaMove);
    void rotateCameraAroundTarget(float pitch, float yaw);
    void moveCameraToTarget(VisualObject* target);

    // Oppdaterer hvor kameraet skal starte når man starter spillet
    void updateGameStartTransform(QVector3D pos, QVector3D rot);

    // Setter kameraet til GameStartTransform
    void setCameraToGameStart();

    // Oppdaterer cameraFront variablen
    void setCameraFront();


    // Kamera transform, up og forward vector
    MyStructs::Transformation mTransformation;
    MyStructs::Transformation mGameStartTransformation;
    QVector3D mCameraUp = QVector3D(0,1,0);
    QVector3D mCameraFront = QVector3D(0,0,-1);

    Matrix4x4 *mPMatrix; // QMatrix4x4 sin perspective fungerte ikke for meg, så må bruke min egen
    QMatrix4x4 *mViewMatrix;

    // Shader uniforms
    // Har mange structs her fordi jeg vil ha et tydelig skille mellom shaderne
    // Jeg får bedre oversikt enn om jeg skulle ha puttet disse inn i et array, når jeg har individuelle navn
    MyStructs::ShaderUniforms mColorShaderUniforms;
    MyStructs::ShaderUniforms mTextureShaderUniforms;
    MyStructs::ShaderUniforms mPhongShaderUniforms;
    MyStructs::ShaderUniforms mBillboardShaderUniforms;
    MyStructs::ShaderUniforms mSkyboxShaderUniforms;

    // Kamera kan ha et target, og mPosRelativeToTarget bestemmer hvor langt unna kameraet skal være target
    VisualObject* mTarget = nullptr;
    QVector3D mPosRelativeToTarget = QVector3D(1, 6, 5);

    // Debug movement og rotasjonsvariabler
    const float mCamSpeed = 0.1f; // Hvor raskt kameraet beveger seg
    float mLastX = 400, mLastY = 300; // Siste y og x verdier for musa
    bool mFirstMove = false; // Om dette er første gang musa forflyttes etter at programmet starter

    // Referanse til kamera sin mesh, som viser hvor kameraet skal starte når spillet starter
    TriangleSurface* mCameraMesh = nullptr;

    // mLabel og mID brukes for å finne kameraet i outlineren
    int mID = 1000;
    std::string mLabel = "Camera";

    // Variabler som bestemmer hvor langt unna og hvor nærme man kan se objekter
    QVector2D mSkyboxViewDist = QVector2D(0.1f, 500); // Skybox skal være langt unna så den har anderledes range
    QVector2D mViewDist = QVector2D(0.1f, 10);
};

#endif // CAMERA_H
