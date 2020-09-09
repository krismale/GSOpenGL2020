#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include "camera.h"
#include "scenemanager.h"

class QOpenGLContext;
class Shader;
class MainWindow;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

     // Plukker opp musens posisjon i vinduet. Brukes til å rotere kameraet
    void mouseMoveEvent(QMouseEvent *e) override;

    void exposeEvent(QExposeEvent *) override;
    void changeRenderMode(); // Bytter mellom wireframe og fill mode for alle objekter

    bool mIsWireframeMode = false;
    bool mIsAxisVisible = true;

    SceneManager *mSceneManager = nullptr;


private slots:
    void render();

private:
    void init();

    QOpenGLContext *mContext;
    bool mInitialized;

    Shader *mShaderProgram; // Dag sin originale RGB baserte shader
    Shader *mTexShaderProgram; // Min texture shader
    Shader *mPhongShaderProgram;
    Shader *mBillboardShaderProgram;
    Shader *mSkyboxShaderProgram;

    GLuint mVAO;
    GLuint mVBO;

    MyStructs::ShaderUniforms mRgbShaderUniforms;
    MyStructs::ShaderUniforms mTexShaderUniforms;
    MyStructs::ShaderUniforms mPhongShaderUniforms;
    MyStructs::ShaderUniforms mBillboardShaderUniforms;
    MyStructs::ShaderUniforms mSkyboxShaderUniforms;

    QTimer *mRenderTimer;           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow;        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    void initShaderUniforms();


protected:
    //The QWindow that we inherit from has these functions to capture
    //mouse and keyboard. Uncomment to use (you also have to make the definitions of these functions
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
