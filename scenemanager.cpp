#include "scenemanager.h"
#include <QKeyEvent>
#include "Colliders/cubecollider.h"
#include "Colliders/spherecollider.h"
#include "MyMath/misccalculations.h"
#include <cstdlib>
#include <ctime>

SceneManager::SceneManager()
{
    std::srand(static_cast<int>(std::time(nullptr)));

    mRgbVisualObjects.push_back(new XYZ());
    mGrid = static_cast<XYZ*>(mRgbVisualObjects.back());

    mSceneLights.push_back(new Light());
    mSceneLights.back()->mLabel = "Light";
    mSceneLights.back()->move(0,1,0);

    setupCameraMesh();

    SetupScene1();


}

void SceneManager::setupCameraMesh()
{
    mCameraMesh = new TriangleSurface("../GSOpenGL2020/Assets/camera.obj", QVector3D(1.0f, 0.f, 0.f));
    mCamera.mCameraMesh = mCameraMesh;
    mCameraMesh->mLabel = "Camera Placeholder";
    mCamera.updateGameStartTransform(mCamera.mPosRelativeToTarget - mCamera.mCameraFront, QVector3D(345, 0,0));
    mCameraMesh->scale(mCamera.mGameStartTransformation.scale.x(),
                      mCamera.mGameStartTransformation.scale.y(),
                      mCamera.mGameStartTransformation.scale.z());
}

void SceneManager::SetupScene1()
{

    SetupPawn();

    mPhongVisualObjects.push_back(new Terrain("../GSOpenGL2020/Assets/heightMap.png", "../GSOpenGL2020/Assets/grass.jpg"));
    mPhongVisualObjects.back()->mLabel = "Terrain";
    mGround = dynamic_cast<Terrain*>(mPhongVisualObjects.back());

    // Lager trær
    for(int i = 0; i < 10; ++i) {
        mPhongVisualObjects.push_back(new TriangleSurface("../GSOpenGL2020/Assets/tree.obj", "../GSOpenGL2020/Assets/treeTexture.jpg"));
        mPhongVisualObjects.back()->mLabel = "Tree" + std::to_string(i);
        mPhongVisualObjects.back()->mColliders.push_back(new CubeCollider(mPhongVisualObjects.back()->mHeight[1]/2, mPhongVisualObjects.back()->mZDepth[1], mPhongVisualObjects.back()->mXWidth[1]));   
        mPhongVisualObjects.back()->mColliders.push_back(new CubeCollider(mPhongVisualObjects.back()->mHeight[1]/2, mPhongVisualObjects.back()->mZDepth[1]/6, mPhongVisualObjects.back()->mXWidth[1]/6));
        mPhongVisualObjects.back()->mColliders[0]->move(0,2,0);
        mPhongVisualObjects.back()->mColliders[1]->move(0,0.7f,0);
    }

    // Plasserer trær på noen randome steder
    mPhongVisualObjects[mPhongVisualObjects.size() - 10]->move(8, 0, -3);
    mPhongVisualObjects[mPhongVisualObjects.size() - 10]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 9]->move(-17, 0, -11);
    mPhongVisualObjects[mPhongVisualObjects.size() - 9]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 8]->move(8, 0, -16);
    mPhongVisualObjects[mPhongVisualObjects.size() - 8]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 7]->move(16, 0, 18);
    mPhongVisualObjects[mPhongVisualObjects.size() - 7]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 6]->move(20, 0, -10);
    mPhongVisualObjects[mPhongVisualObjects.size() - 6]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 5]->move(-8, 0, -3);
    mPhongVisualObjects[mPhongVisualObjects.size() - 5]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 4]->move(-12, 0, 9);
    mPhongVisualObjects[mPhongVisualObjects.size() - 4]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 3]->move(-7, 0, 18);
    mPhongVisualObjects[mPhongVisualObjects.size() - 3]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 2]->move(-20, 0, 5);
    mPhongVisualObjects[mPhongVisualObjects.size() - 2]->placeOnTopOfGround(mGround);
    mPhongVisualObjects[mPhongVisualObjects.size() - 1]->move(-11, 0, 18);
    mPhongVisualObjects[mPhongVisualObjects.size() - 1]->placeOnTopOfGround(mGround);


    // Lager et gjerde av steiner rundt trophies
    QVector3D rockPos = QVector3D(-10, 0, -5);
    for(int i = 0; i < 80; ++i) {
        if(i == 4) {
            mPhongVisualObjects.push_back(new InteractiveObject("../GSOpenGL2020/Assets/cube.obj", QVector3D(0.0f, 0.0f, 1.0f)));
            mPhongVisualObjects.back()->mLabel = "PushBox";
            mPushObject = static_cast<InteractiveObject*>(mPhongVisualObjects.back());
            mPushObject->mColliders.push_back(new CubeCollider(mPushObject->mHeight[1], mPushObject->mZDepth[1], mPushObject->mXWidth[1]));
            mPushObject->mColliders[0]->mIsTrigger = true;
            mPushObject->scale(0.5f,0.5f,0.5f);
            mPushObject->mGameStartPos = QVector3D(rockPos.x()+1, rockPos.y(), rockPos.z());
        } else {
            mPhongVisualObjects.push_back(new TriangleSurface("../GSOpenGL2020/Assets/rock.obj", QVector3D(0.6f, 0.6f, 0.6f)));
            mPhongVisualObjects.back()->mLabel = "Rock";
            mPhongVisualObjects.back()->scale(0.7f, 0.7f, 0.7f);
            mPhongVisualObjects.back()->mColliders.push_back(new CubeCollider(mPhongVisualObjects.back()->mHeight[1],
                                                                              mPhongVisualObjects.back()->mZDepth[1],
                                                                              mPhongVisualObjects.back()->mXWidth[1]));
        }

        if(i < 20) {
            rockPos.setX(rockPos.x() + 1);
        } else if(i >= 20 && i < 40) {
            rockPos.setZ(rockPos.z() - 1);
        } else if(i >= 40 && i < 60) {
            rockPos.setX(rockPos.x() - 1);
        } else {
            rockPos.setZ(rockPos.z() + 1);
        }
        mPhongVisualObjects.back()->move(rockPos.x(), 0,rockPos.z());
        mPhongVisualObjects.back()->placeOnTopOfGround(mGround, 0.4f);
    }


    setupTrophies();

    setupEnemy();




    // Lager gress
    for(int i = 0; i < 50; ++i) {
        mBillboardVisualObjects.push_back(new Billboard("../GSOpenGL2020/Assets/grassLeafs.png"));
        mBillboardVisualObjects.back()->move(MiscCalculations::randFloat(0, 40) - 20, 0, MiscCalculations::randFloat(0, 40) - 20);
        mBillboardVisualObjects.back()->placeOnTopOfGround(mGround,0.4f);
    }


    for(int i = 0; i < 5; ++i) {
        std::string path = "../GSOpenGL2020/Assets/trophyUI" + std::to_string(i) + ".png";
        mUIElements.push_back(new UI(path.c_str()));
        mBillboardVisualObjects.push_back(mUIElements.back());
    }

    mWinUI = new UI("../GSOpenGL2020/Assets/WinUI.png");
    mBillboardVisualObjects.push_back(mWinUI);
    mLoseUI = new UI("../GSOpenGL2020/Assets/LoseUI.png");
    mBillboardVisualObjects.push_back(mLoseUI);


    std::vector<std::string> mSkyboxTextures;
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxRight.jpg");
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxLeft.jpg");
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxTop.jpg");
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxBottom.jpg");
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxFront.jpg");
    mSkyboxTextures.push_back("../GSOpenGL2020/Assets/skyboxBack.jpg");
    mSkyboxVisualObject = new Skybox(mSkyboxTextures);


}

void SceneManager::SetupPawn()
{
    mPhongVisualObjects.push_back(new TriangleSurface("../GSOpenGL2020/Assets/suzanne.obj", QVector3D(0.5f,0.3f,0.9f)));
    mPhongVisualObjects.back()->mLabel = "Pawn";
    mPawn = dynamic_cast<TriangleSurface*>(mPhongVisualObjects.back());
    mPawn->mColliders.push_back(new CubeCollider(mPawn->mHeight[0],mPawn->mZDepth[0],mPawn->mXWidth[0]));
    mPawn->rotate(0, 180, 0);
    mPawn->scale(0.25f, 0.25f, 0.25f);

    mCamera.mTarget = mPawn;
}

void SceneManager::setupEnemy()
{
    mPhongVisualObjects.push_back((new TriangleSurface("../GSOpenGL2020/Assets/golem.obj", "../GSOpenGL2020/Assets/concrete.png")));
    mPhongVisualObjects.back()->mLabel = "Enemy";
    mEnemy = static_cast<TriangleSurface*>(mPhongVisualObjects.back());
    mEnemy->mColliders.push_back(new CubeCollider(mEnemy->mHeight[0],mEnemy->mZDepth[0],mEnemy->mXWidth[0]));
    mEnemy->mColliders[0]->mIsTrigger = true;
    mEnemy->scale(0.15f, 0.15f, 0.15f);
    mEnemy->moveTo(mTrophies[0]->mTransformation.translation.x(),
                mTrophies[0]->mTransformation.translation.y(),
                mTrophies[0]->mTransformation.translation.z());

    mRgbVisualObjects.emplace_back(new BezierCurve(mTrophies));
    mRgbVisualObjects.back()->mLabel = "Bezier Curve";
    mCurveToMoveAlong = dynamic_cast<BezierCurve*>(mRgbVisualObjects.back());
}

void SceneManager::setupTrophies()
{
    for(int i = 0; i < 4; ++i) {
        mPhongVisualObjects.push_back((new TriangleSurface("../GSOpenGL2020/Assets/trophy.obj", QVector3D(0.0f, 1.0f, 0.0f))));
        mPhongVisualObjects.back()->mLabel = "Trophy" + std::to_string(i);
        mTrophies.push_back(static_cast<TriangleSurface*>(mPhongVisualObjects.back()));
        mTrophies.back()->mColliders.push_back(new CubeCollider(mTrophies.back()->mHeight[1], mTrophies.back()->mZDepth[1], mTrophies.back()->mXWidth[1]));
        mTrophies.back()->mColliders[0]->mIsTrigger = true;
        mTrophies[i]->moveTo(mTrophyPositions[i].x(),mTrophyPositions[i].y(),mTrophyPositions[i].z());
        mTrophies[i]->placeOnTopOfGround(mGround, mTrophies[i]->mHeight[1]/2);
    }

}

void SceneManager::renderAllHelpObjects(bool render)
{
    mGrid->mVisible = render;
    mCurveToMoveAlong->mVisible = render;
    for(const auto &it : mSceneLights) {
        it->mVisible = render;
    }
    mCameraMesh->mVisible = render;
}

void SceneManager::startScene()
{
    renderAllHelpObjects(false);
    mCamera.setCameraToGameStart();
    mIsPlaying = true;
    resetControls();

    mWinUI->mVisible = false;
    mLoseUI->mVisible = false;
    mFreezeAllControls = false;

    for(int i = 0; i < static_cast<int>(mUIElements.size()); ++i) {
        if(i == mGatheredTrophies) {
            mUIElements[i]->mVisible = true;
        } else {
            mUIElements[i]->mVisible = false;
        }
    }
}

void SceneManager::stopScene()
{
    renderAllHelpObjects(true);
    mIsPlaying = false;

    resetControls();

    for(int i = 0; i < static_cast<int>(mUIElements.size()); ++i) {
        mUIElements[i]->mVisible = false;
    }
}

VisualObject* SceneManager::searchAllObjects(int id)
{
    for(const auto &it : mSceneLights) {
        if(it->mID == id) {
            return it;
        }
    }
    for(const auto &it : mPhongVisualObjects) {
        if(it->mID == id) {
            return it;
        }
    }
    for(const auto &it : mTexVisualObjects) {
        if(it->mID == id) {
            return it;
        }
    }
    for(const auto &it : mRgbVisualObjects) {
        if(it->mID == id) {
            return it;
        }
    }

    for(const auto &it : mBillboardVisualObjects) {
        if(it->mID == id) {
            return it;
        }
    }
    return nullptr;
}

void SceneManager::init(MyStructs::ShaderUniforms rgbShaderUniforms, MyStructs::ShaderUniforms texShaderUniforms,
                        MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms,
                        MyStructs::ShaderUniforms skyboxShaderUniforms)
{
    initializeOpenGLFunctions();

    mPhongShaderUniforms = phongShaderUniforms;
    mBillboardShaderUniforms = billboardShaderUniforms;

    mCamera.init(rgbShaderUniforms, texShaderUniforms, phongShaderUniforms, billboardShaderUniforms, skyboxShaderUniforms);


    // enable the matrixUniform
    for(const auto &it : mSceneLights) {
        it->init(phongShaderUniforms, rgbShaderUniforms, billboardShaderUniforms);
    }
    for(const auto &it : mPhongVisualObjects) {
        it->init(phongShaderUniforms);
    }
    for(const auto &it : mRgbVisualObjects) {
        it->init(rgbShaderUniforms);
    }
    for(const auto &it : mTexVisualObjects) {
        it->init(texShaderUniforms);
    }
    for(const auto &it : mBillboardVisualObjects) {
        it->init(billboardShaderUniforms);
    }

    mCameraMesh->init(mPhongShaderUniforms);
    mSkyboxVisualObject->init(skyboxShaderUniforms);

}

void SceneManager::drawRGBObjects(MainWindow *mainWindow, Shader *shaderProgram)
{
    glUseProgram(shaderProgram->getProgram());
    mCamera.renderCol(mainWindow->width()/mainWindow->height());

    for(const auto &it : mSceneLights) {
        it->draw();
    }

    // actual draw call
    for(const auto &it : mRgbVisualObjects) {
        it->draw();
    }
}

void SceneManager::drawTexObjects(MainWindow *mainWindow, Shader* texShaderProgram)
{
    glUseProgram(texShaderProgram->getProgram());
    mCamera.renderTex(mainWindow->width()/mainWindow->height());

    // actual draw call
    for(const auto &it : mTexVisualObjects) {
         it->draw();
    }


}

void SceneManager::drawSkybox(MainWindow *mainWindow, Shader* skyboxShaderProgram)
{
    glUseProgram(skyboxShaderProgram->getProgram());
    mCamera.renderSkybox(mainWindow->width()/mainWindow->height());

    // actual draw call
    mSkyboxVisualObject->draw();
}

void SceneManager::drawPhongObjects(MainWindow *mainWindow, Shader *phongShaderProgram)
{
    glUseProgram(phongShaderProgram->getProgram());
    mCamera.renderPhong(mainWindow->width()/mainWindow->height());

    // Binder ambient uniforms med riktige verdier satt til scenen
    glUniform1f(mPhongShaderUniforms.mAmbientStrengthUniform, mAmbientStrength);
    glUniform3f(mPhongShaderUniforms.mAmbientColUniform, mAmbientCol.x(), mAmbientCol.y(), mAmbientCol.z());

    for(const auto &it : mSceneLights) {
        it->renderLight();
    }

    // actual draw call
    for(const auto &it : mPhongVisualObjects) {
         it->draw();
    }
    mCameraMesh->draw();
}

void SceneManager::drawBillboardObjects(MainWindow *mainWindow, Shader *billboardShaderProgram)
{
    glUseProgram(billboardShaderProgram->getProgram());
    mCamera.renderBillboard(mainWindow->width()/mainWindow->height());

//    // Binder ambient uniforms med riktige verdier satt til scenen
    glUniform1f(mBillboardShaderUniforms.mAmbientStrengthUniform, mAmbientStrength);
    glUniform3f(mBillboardShaderUniforms.mAmbientColUniform, mAmbientCol.x(), mAmbientCol.y(), mAmbientCol.z());

    for(const auto &it : mSceneLights) {
        it->renderLightBillboard();
    }

    // actual draw call
    for(const auto &it : mBillboardVisualObjects) {
         it->draw();
    }
}

void SceneManager::resetGame()
{
    mPawn->mTransformation.translation = mPawnSpawnLocation.toVector4D();
    mPawn->placeOnTopOfGround(mGround, mPawn->mHeight[1]);
    mPawn->mLastTriangle = nullptr;
    for(const auto &it : mPawn->mColliders) {
        it->mTransformation.translation.setX(mPawnSpawnLocation.x());
        it->placeOnTopOfGround(mGround, it->mHeight[1]);
        it->mTransformation.translation.setZ(mPawnSpawnLocation.z());
        it->move(0,0,0);
    }
    mPawn->move(0,0,0);

    // "Respawner" trophies igjen
    for(unsigned int i = 0; i < mTrophies.size(); ++i) {
        mTrophies[i]->mVisible = true;
        for(const auto &it : mTrophies[i]->mColliders) {
            it->mCollisionOn = true;
        }
        mGatheredTrophies = 0;
    }
    mEnemy->moveTo(mTrophies[0]->mTransformation.translation.x(), mTrophies[0]->mTransformation.translation.y(), mTrophies[0]->mTransformation.translation.z());

    mPushObject->setToGameStart(mGround);

    moveCameraAfterPawn();
    mCamera.setCameraToGameStart();

    mWinUI->mVisible = false;
    mLoseUI->mVisible = false;
    mFreezeAllControls = false;
}

void SceneManager::moveCameraAfterPawn()
{
    mCamera.moveCameraToTarget(mPawn);
    mCamera.updateGameStartTransform(mCamera.mTransformation.translation.toVector3D(), mCamera.mTransformation.rotation.toVector3D());
}

void SceneManager::currentSceneLogic()
{
    playerMovement();
    enemyMovement();

    moveCameraAfterPawn();

    // Enkel rotasjon for trofeene
    for(const auto &it : mTrophies) {
        it->rotate(0, 0.1f, 0);
    }

    // Collision logic for trofeer
    for(unsigned int i = 0; i < mTrophies.size(); ++i) {
        if(mPawn->mColliders[0]->checkTriggerCollision(mTrophies[i])) {
            mTrophies[i]->mVisible = false;
            for(const auto &it : mTrophies[i]->mColliders) {
                it->mCollisionOn = false;
            }
            ++mGatheredTrophies;
            if(mGatheredTrophies >= 4) {
                stopScene();
                mWinUI->mVisible = true;
                mFreezeAllControls = true;
            }
        }
    }

    // Collision logic for enemy
    if(mPawn->mColliders[0]->checkTriggerCollision(mEnemy)) {
        stopScene();
        mLoseUI->mVisible = true;
        mFreezeAllControls = true;

    }


    if(mPushObject->mMoving || mPushObject->mJustPushed) {
        mPushObject->moveBetweenTwoPoints(mPawn->mMoveDirection, mGround, mPhongVisualObjects);
    } else {
        if(mPawn->mColliders[0]->checkTriggerCollision(mPushObject)) {
            mPushObject->mJustPushed = true;
        }
    }

    for(int i = 0; i < static_cast<int>(mUIElements.size()); ++i) {
        if(i == mGatheredTrophies && mIsPlaying) {
            mUIElements[i]->mVisible = true;
        } else {
            mUIElements[i]->mVisible = false;
        }
    }

}

void SceneManager::playerMovement()
{
    QVector3D playerMove;

    if(mPlayerInput.moveRight) {
        playerMove.setX(playerMove.x() + 0.1f);
    }
    if(mPlayerInput.moveLeft) {
        playerMove.setX(playerMove.x() - 0.1f);
    }
    if(mPlayerInput.moveForward) {
        playerMove.setZ(playerMove.z() - 0.1f);
    }
    if(mPlayerInput.moveBack) {
        playerMove.setZ(playerMove.z() + 0.1f);
    }

    if(mGround) {
        mPawn->moveAboveGround(playerMove.x(), playerMove.y(), playerMove.z(), mGround, mPhongVisualObjects);
    } else {
        mPawn->move(playerMove.x(), playerMove.y(), playerMove.z());
    }
}

void SceneManager::cameraMovement()
{
    if(mPlayerInput.moveCamRight) {
        QVector3D moveRight = QVector3D::crossProduct(mCamera.mCameraFront, mCamera.mCameraUp);
        moveRight.normalize();
        mCamera.move(moveRight);
    }
    if(mPlayerInput.moveCamLeft) {
        QVector3D moveLeft = -QVector3D::crossProduct(mCamera.mCameraFront, mCamera.mCameraUp);
        moveLeft.normalize();
        mCamera.move(moveLeft);
    }
    if(mPlayerInput.moveCamForward) {
        mCamera.move(mCamera.mCameraFront);
    }
    if(mPlayerInput.moveCamBack) {
        mCamera.move(-mCamera.mCameraFront);
    }
}

void SceneManager::resetControls()
{
    // Resets player movement
    mPlayerInput.moveRight      = false;
    mPlayerInput.moveLeft       = false;
    mPlayerInput.moveForward    = false;
    mPlayerInput.moveBack       = false;

    // Resets cam movement
    mPlayerInput.moveCamRight   = false;
    mPlayerInput.moveCamLeft    = false;
    mPlayerInput.moveCamForward = false;
    mPlayerInput.moveCamBack    = false;
    mPlayerInput.camRotate      = false;

}

void SceneManager::playerControls(QKeyEvent *event, bool gameIsPlaying)
{
    if(!mFreezeAllControls) {
        if(!gameIsPlaying) {
            // Kamerabevegelse
            if(event->key() == Qt::Key_A) {
                mPlayerInput.moveCamLeft = !mPlayerInput.moveCamLeft;
            }
            if(event->key() == Qt::Key_D) {
                mPlayerInput.moveCamRight = !mPlayerInput.moveCamRight;
            }
            if(event->key() == Qt::Key_W) {
                mPlayerInput.moveCamForward = !mPlayerInput.moveCamForward;
            }
            if(event->key() == Qt::Key_S) {
                mPlayerInput.moveCamBack = !mPlayerInput.moveCamBack;
            }

            if(event->key() == Qt::Key_Shift) {
                mPlayerInput.camRotate = !mPlayerInput.camRotate;
                mCamera.mFirstMove = true;
            }
        } else {
            if(!mPawn) {
                qDebug() << "There's no controllable object in the scene!";
                return;
            }

            // Spillerbevegelse
            if(event->key() == Qt::Key_D) {
                mPlayerInput.moveRight = !mPlayerInput.moveRight;
            }
            if(event->key() == Qt::Key_A) {
                mPlayerInput.moveLeft = !mPlayerInput.moveLeft;
            }
            if(event->key() == Qt::Key_W) {
                mPlayerInput.moveForward = !mPlayerInput.moveForward;
            }
            if(event->key() == Qt::Key_S) {
                mPlayerInput.moveBack = !mPlayerInput.moveBack;
            }
        }
    } else {
        resetControls();
    }
    if(event->key() == Qt::Key_Control) {
        resetControls();
    }
}

void SceneManager::enemyMovement()
{
    static int index = 0;
    static bool moveForward = true;
    if(moveForward) {
        if(index < static_cast<int>(mCurveToMoveAlong->mCurvePoints.size())) {
            index += mEnemyMovementSpeed;
        } else {
            if(index > static_cast<int>(mCurveToMoveAlong->mCurvePoints.size())) {
            index = static_cast<int>(mCurveToMoveAlong->mCurvePoints.size());
            }
            moveForward = false;
        }
    } else {
        if(index > 0) {
            index -= mEnemyMovementSpeed;
        } else {
            if(index < 0) {
            index = 0;
            }
            moveForward = true;
        }
    }

    mEnemy->moveAlongCurve(index, mCurveToMoveAlong->mCurvePoints, mGround, mPhongVisualObjects);
}
