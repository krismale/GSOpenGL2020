#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <cmath>

#include <iostream>

#include "shader.h"
#include "mainwindow.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

    mSceneManager = new SceneManager();

}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized) {
        mInitialized = true;
    }

    initializeOpenGLFunctions();


    //Print render version info (what GPU is used):
    //(Have to use cout to see text- qDebug just writes numbers...)
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    std::cout << "The active GPU and API: \n";
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);           //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT   

    initShaderUniforms();

    //********************** Making the object to be drawn **********************

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glBindVertexArray(0);

    mSceneManager->init(mRgbShaderUniforms, mTexShaderUniforms, mPhongShaderUniforms, mBillboardShaderUniforms, mSkyboxShaderUniforms);

}

void RenderWindow::initShaderUniforms()
{
    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram = new Shader("../GSOpenGL2020/plainvertex.vert", "../GSOpenGL2020/plainfragment.frag");
    mTexShaderProgram = new Shader("../GSOpenGL2020/plainvertex.vert", "../GSOpenGL2020/plaintexfragment.frag");
    mPhongShaderProgram = new Shader("../GSOpenGL2020/phongvertex.vert", "../GSOpenGL2020/phongfragment.frag");
    mBillboardShaderProgram = new Shader("../GSOpenGL2020/billboardvertex.vert", "../GSOpenGL2020/billboardfragment.frag");
    mSkyboxShaderProgram = new Shader("../GSOpenGL2020/skyboxvertex.vert", "../GSOpenGL2020/skyboxfragment.frag");

    // Setter opp rgb shader uniforms
    mRgbShaderUniforms.mPMatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "pMatrix" );
    mRgbShaderUniforms.mVMatrixUniform = glGetUniformLocation(mShaderProgram->getProgram(), "vMatrix");
    mRgbShaderUniforms.mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );

    // Setter opp texture shader uniforms
    mTexShaderUniforms.mPMatrixUniform = glGetUniformLocation(mTexShaderProgram->getProgram(), "pMatrix" );
    mTexShaderUniforms.mVMatrixUniform = glGetUniformLocation(mTexShaderProgram->getProgram(), "vMatrix");
    mTexShaderUniforms.mMatrixUniform = glGetUniformLocation( mTexShaderProgram->getProgram(), "matrix" );
    mTexShaderUniforms.mTextureUniform = glGetUniformLocation( mTexShaderProgram->getProgram(), "ourTexture" );

    // Setter opp phong shader uniforms
    mPhongShaderUniforms.mPMatrixUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "pMatrix" );
    mPhongShaderUniforms.mVMatrixUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "vMatrix");
    mPhongShaderUniforms.mMatrixUniform = glGetUniformLocation( mPhongShaderProgram->getProgram(), "matrix" );
    mPhongShaderUniforms.mTextureUniform = glGetUniformLocation( mPhongShaderProgram->getProgram(), "ourTexture" );
    mPhongShaderUniforms.mObjectColorUniform = glGetUniformLocation( mPhongShaderProgram->getProgram(), "objectColor" );
    mPhongShaderUniforms.mSpecularStrengthUniform = glGetUniformLocation( mPhongShaderProgram->getProgram(), "specularStrength" );
    mPhongShaderUniforms.mSpecularExponentUniform = glGetUniformLocation( mPhongShaderProgram->getProgram(), "specularExponent" );
    mPhongShaderUniforms.mLightPosUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "lightPosition");
    mPhongShaderUniforms.mLightPowerUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "lightPower");
    mPhongShaderUniforms.mLightStrengthUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "lightStrength");
    mPhongShaderUniforms.mAmbientStrengthUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "ambientStrength");
    mPhongShaderUniforms.mAmbientColUniform = glGetUniformLocation(mPhongShaderProgram->getProgram(), "ambientCol");

    // Setter opp billboard shader uniforms
    mBillboardShaderUniforms.mPMatrixUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "pMatrix" );
    mBillboardShaderUniforms.mVMatrixUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "vMatrix");
    mBillboardShaderUniforms.mMatrixUniform = glGetUniformLocation( mBillboardShaderProgram->getProgram(), "matrix" );
    mBillboardShaderUniforms.mTextureUniform = glGetUniformLocation( mBillboardShaderProgram->getProgram(), "ourTexture" );
    mBillboardShaderUniforms.mObjectColorUniform = glGetUniformLocation( mBillboardShaderProgram->getProgram(), "objectColor" );
    mBillboardShaderUniforms.mSpecularStrengthUniform = glGetUniformLocation( mBillboardShaderProgram->getProgram(), "specularStrength" );
    mBillboardShaderUniforms.mSpecularExponentUniform = glGetUniformLocation( mBillboardShaderProgram->getProgram(), "specularExponent" );
    mBillboardShaderUniforms.mLightPosUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "lightPosition");
    mBillboardShaderUniforms.mLightPowerUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "lightPower");
    mBillboardShaderUniforms.mLightStrengthUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "lightStrength");
    mBillboardShaderUniforms.mAmbientStrengthUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "ambientStrength");
    mBillboardShaderUniforms.mAmbientColUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "ambientCol");
    mBillboardShaderUniforms.mObjectCenterUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "objectCenter");
    mBillboardShaderUniforms.mObjectSizeUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "objectSize");
    mBillboardShaderUniforms.mUiElementUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "uiElement");
    mBillboardShaderUniforms.mCameraCenterUniform = glGetUniformLocation(mBillboardShaderProgram->getProgram(), "camCenter");

    mSkyboxShaderUniforms.mPMatrixUniform = glGetUniformLocation(mSkyboxShaderProgram->getProgram(), "pMatrix" );
    mSkyboxShaderUniforms.mVMatrixUniform = glGetUniformLocation(mSkyboxShaderProgram->getProgram(), "vMatrix" );
    mSkyboxShaderUniforms.mTextureUniform = glGetUniformLocation( mSkyboxShaderProgram->getProgram(), "ourTexture" );
    mSkyboxShaderUniforms.mCameraCenterUniform = glGetUniformLocation(mSkyboxShaderProgram->getProgram(), "camCenter");
    mSkyboxShaderUniforms.mObjectSizeUniform = glGetUniformLocation(mSkyboxShaderProgram->getProgram(), "objectSize");
}

void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Tegner objektene som er brukt i den åpne scenen
    mSceneManager->drawRGBObjects(mMainWindow, mShaderProgram);
    mSceneManager->drawTexObjects(mMainWindow, mTexShaderProgram);
    mSceneManager->drawPhongObjects(mMainWindow, mPhongShaderProgram);
    mSceneManager->drawBillboardObjects(mMainWindow, mBillboardShaderProgram);
    mSceneManager->drawSkybox(mMainWindow, mSkyboxShaderProgram);

    // Utfører logikken i scenen som er åpen
    if(mSceneManager->mIsPlaying) {
        mSceneManager->currentSceneLogic();
    } else {
        mSceneManager->cameraMovement();
    }

    //Calculate framerate before
    // checkForGLerrors() because that takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    // checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

void RenderWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(mSceneManager->mPlayerInput.camRotate) {
        mSceneManager->mCamera.rotateWithMouse(e->pos().x(), e->pos().y());
    }

}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

void RenderWindow::changeRenderMode()
{
    if(mIsWireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }


    if(event->isAutoRepeat()) {
        event->ignore();
    } else{
        mSceneManager->playerControls(event, mSceneManager->mIsPlaying);
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) {
        event->ignore();
    } else{
        mSceneManager->playerControls(event, mSceneManager->mIsPlaying);
    }

}


