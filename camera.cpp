#include "camera.h"
#include <cmath>
#include "MyMath/mathconstants.h"
#include "MyMath/misccalculations.h"


Camera::Camera()
{
    mViewMatrix = new QMatrix4x4;
    mPMatrix = new Matrix4x4;

    mTransformation.rotation = QVector4D(0,0,0,1);
    mTransformation.translation = QVector4D(0, 5, 15, 1);
    mTransformation.scale = QVector4D(1,1,1,1);


    updateGameStartTransform(QVector3D(0, 3.25f, 10), QVector3D(345, 0,0));
    mGameStartTransformation.scale = QVector4D(1,1,1,1);


    mPMatrix->setToIdentity();
    mViewMatrix->setToIdentity();

}

void Camera::init(MyStructs::ShaderUniforms rgbShaderUniforms, MyStructs::ShaderUniforms texShaderUniforms, MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms, MyStructs::ShaderUniforms skyboxShaderUniforms)
{
    initializeOpenGLFunctions();

    mColorShaderUniforms.mPMatrixUniform = rgbShaderUniforms.mPMatrixUniform;
    mColorShaderUniforms.mVMatrixUniform = rgbShaderUniforms.mVMatrixUniform;
    mTextureShaderUniforms.mPMatrixUniform = texShaderUniforms.mPMatrixUniform;
    mTextureShaderUniforms.mVMatrixUniform = texShaderUniforms.mVMatrixUniform;
    mPhongShaderUniforms.mPMatrixUniform = phongShaderUniforms.mPMatrixUniform;
    mPhongShaderUniforms.mVMatrixUniform = phongShaderUniforms.mVMatrixUniform;
    mBillboardShaderUniforms.mPMatrixUniform = billboardShaderUniforms.mPMatrixUniform;
    mBillboardShaderUniforms.mVMatrixUniform = billboardShaderUniforms.mVMatrixUniform;
    mBillboardShaderUniforms.mCameraCenterUniform = billboardShaderUniforms.mCameraCenterUniform;
    mSkyboxShaderUniforms.mPMatrixUniform = skyboxShaderUniforms.mPMatrixUniform;
    mSkyboxShaderUniforms.mVMatrixUniform = skyboxShaderUniforms.mVMatrixUniform;
    mSkyboxShaderUniforms.mCameraCenterUniform = skyboxShaderUniforms.mCameraCenterUniform;

    mViewMatrix->lookAt(mTransformation.translation.toVector3D(), QVector3D(0,0,0), mCameraUp);
}

void Camera::renderCol(float windowAspectRatio)
{

    mPMatrix->perspective(60, windowAspectRatio, mViewDist.x(), mViewDist.y());

    glUniformMatrix4fv( mColorShaderUniforms.mPMatrixUniform, 1, GL_TRUE, mPMatrix->constData());
    glUniformMatrix4fv( mColorShaderUniforms.mVMatrixUniform, 1, GL_FALSE, mViewMatrix->constData());
}

void Camera::renderTex(float windowAspectRatio)
{

    mPMatrix->perspective(60, windowAspectRatio, mViewDist.x(), mViewDist.y());

    glUniformMatrix4fv( mTextureShaderUniforms.mPMatrixUniform, 1, GL_TRUE, mPMatrix->constData());
    glUniformMatrix4fv( mTextureShaderUniforms.mVMatrixUniform, 1, GL_FALSE, mViewMatrix->constData());
}

void Camera::renderSkybox(float windowAspectRatio)
{

    mPMatrix->perspective(60, windowAspectRatio, mSkyboxViewDist.x(), mSkyboxViewDist.y());

    glUniformMatrix4fv( mSkyboxShaderUniforms.mPMatrixUniform, 1, GL_TRUE, mPMatrix->constData());
    glUniformMatrix4fv( mSkyboxShaderUniforms.mVMatrixUniform, 1, GL_FALSE, mViewMatrix->constData());
    glUniform3f(mSkyboxShaderUniforms.mCameraCenterUniform, mTransformation.translation.x(), mTransformation.translation.y(), mTransformation.translation.z());

}

void Camera::renderPhong(float windowAspectRatio)
{
    mPMatrix->perspective(60, windowAspectRatio, mViewDist.x(), mViewDist.y());

    glUniformMatrix4fv( mPhongShaderUniforms.mPMatrixUniform, 1, GL_TRUE, mPMatrix->constData());
    glUniformMatrix4fv( mPhongShaderUniforms.mVMatrixUniform, 1, GL_FALSE, mViewMatrix->constData());
}

void Camera::renderBillboard(float windowAspectRatio)
{
    mPMatrix->perspective(60, windowAspectRatio, mViewDist.x(), mViewDist.y());

    glUniformMatrix4fv( mBillboardShaderUniforms.mPMatrixUniform, 1, GL_TRUE, mPMatrix->constData());
    glUniformMatrix4fv( mBillboardShaderUniforms.mVMatrixUniform, 1, GL_FALSE, mViewMatrix->constData());
    glUniform3f(mBillboardShaderUniforms.mCameraCenterUniform, mTransformation.translation.x(), mTransformation.translation.y(), mTransformation.translation.z());
}

void Camera::moveCamera(QVector3D cameraPosition, QVector3D cameraLookAt, QVector3D cameraUpVector)
{
    mViewMatrix->setToIdentity();

    mTransformation.translation = cameraPosition;

    mViewMatrix->lookAt(cameraPosition, cameraLookAt, cameraUpVector);
}

void Camera::moveCameraToTarget(VisualObject *target)
{
    mViewMatrix->setToIdentity();
    if(mTarget != target) {
        mTarget = target;
    }

    if(mTarget) {

        // Sørger for at kameraets rotasjon også blir tatt hesnyn til når forflytningen skjer
        float x = mTarget->mTransformation.translation.x() + mPosRelativeToTarget.x();
        float y = mTarget->mTransformation.translation.y() + mPosRelativeToTarget.y();
        float z = mTarget->mTransformation.translation.z();
        if(mTransformation.rotation.y() > 0) {
            x += (sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.y())) * mPosRelativeToTarget.z());
            z += (cosf(MiscCalculations::degreesToRadians(mTransformation.rotation.y())) * mPosRelativeToTarget.z());
        }
        if(mTransformation.rotation.x() > 0) {
            y += (sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.x())) * mPosRelativeToTarget.z());
            z += (cosf(MiscCalculations::degreesToRadians(mTransformation.rotation.x())) * mPosRelativeToTarget.z());
        } else {
            z += mPosRelativeToTarget.z();
        }


        mTransformation.translation.setX(x);
        mTransformation.translation.setY(y);
        mTransformation.translation.setZ(z);

        moveCamera(mTransformation.translation.toVector3D(),
                   QVector3D(mTarget->mTransformation.translation.x(),
                             mTarget->mTransformation.translation.y(),
                             mTarget->mTransformation.translation.z()), mCameraUp);
    }
}

// Denne kan skape bugs nå, etter at jeg har implementert rotateWithmouse()
void Camera::rotateCameraAroundTarget(float pitch, float yaw)
{
    mViewMatrix->setToIdentity();

    float radius = mPosRelativeToTarget.z(); // Hvor langt unna objektet en skal rotere

    if(pitch > 0) {
        // Pitch rotasjon
        mTransformation.rotation.setX(mTransformation.rotation.x() + pitch);

        mTransformation.translation.setX(mTransformation.translation.x());
        mTransformation.translation.setY(sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.x())) * radius);
        mTransformation.translation.setZ(cosf(MiscCalculations::degreesToRadians(mTransformation.rotation.x())) * radius);
    } else if(yaw > 0) {
        //Yaw rotasjon
        mTransformation.rotation.setY(mTransformation.rotation.y() + yaw);

        mTransformation.translation.setX(sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.y())) * radius);
        mTransformation.translation.setY(mPosRelativeToTarget.y());
        mTransformation.translation.setZ(cosf(MiscCalculations::degreesToRadians(mTransformation.rotation.y())) * radius);
    }

    setCameraFront();

    // Ser på objektet, med ny rotasjon lagt til i mTransformation.translation
    mViewMatrix->lookAt(mTransformation.translation.toVector3D(),
                        mTarget->mTransformation.translation.toVector3D(), mCameraUp);

    // Flytter kameraet til objektet, etter at rotasjonen har blitt gjort
    moveCameraToTarget(mTarget);
}

void Camera::setCameraFront()
{
    QVector3D direction;
    direction.setX(cosf(MiscCalculations::degreesToRadians(mTransformation.rotation.y()))); // Note that we convert the angle to radians first
    direction.setY(-sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.x())));
    direction.setZ(sinf(MiscCalculations::degreesToRadians(mTransformation.rotation.y())));
    direction.normalize();
    mCameraFront = direction;
}

void Camera::setCameraToGameStart()
{
    mTransformation.translation = mGameStartTransformation.translation;
    mTransformation.rotation = QVector4D(0,0,0,1);
}

void Camera::rotateWithMouse(float xPos, float yPos)
{

    mViewMatrix->setToIdentity();
    if(mFirstMove) {
        mLastX = xPos;
        mLastY = yPos;
        mFirstMove = false;
    }

    float xoffset = xPos - mLastX;
    float yoffset = yPos - mLastY;
    mLastX = xPos;
    mLastY = yPos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;



    mTransformation.rotation.setY(mTransformation.rotation.y() + xoffset);
    mTransformation.rotation.setX(mTransformation.rotation.x() + yoffset);

    if(mTransformation.rotation.x() > 89.0f) {
        mTransformation.rotation.setX(89.0f);
    }

    if(mTransformation.rotation.x() < -89.0f) {
        mTransformation.rotation.setX(-89.0f);
    }


    setCameraFront();

    mViewMatrix->lookAt(mTransformation.translation.toVector3D(), mTransformation.translation.toVector3D() + mCameraFront, mCameraUp);
}

void Camera::updateGameStartTransform(QVector3D pos, QVector3D rot)
{
    mGameStartTransformation.translation = pos;
    mGameStartTransformation.rotation = rot;
    if(mCameraMesh) {
        mCameraMesh->setTranslationForAll(pos.x() - (mCameraFront.x() * 1.2f), pos.y() - (mCameraFront.y() * 1.2f), pos.z() - (mCameraFront.z() * 1.2f));
        mCameraMesh->move(0, 0, 0);
        mCameraMesh->setRotationForAll(mGameStartTransformation.rotation.x(), mGameStartTransformation.rotation.y(), 0);
        mCameraMesh->rotate(0,0,0);
    }
    if(mTarget) {
        mPosRelativeToTarget = mGameStartTransformation.translation.toVector3D() - mTarget->mTransformation.translation.toVector3D();
    }
}

void Camera::move(float dx, float dy, float dz)
{
    mViewMatrix->setToIdentity();

    mTransformation.translation.setX(mTransformation.translation.x() + dx);
    mTransformation.translation.setY(mTransformation.translation.y() + dy);
    mTransformation.translation.setZ(mTransformation.translation.z() + dz);

    mViewMatrix->lookAt(mTransformation.translation.toVector3D(), mTransformation.translation.toVector3D() + mCameraFront, mCameraUp);

}

void Camera::move(QVector3D deltaMove) {
    deltaMove *= mCamSpeed;
    move(deltaMove.x(), deltaMove.y(), deltaMove.z());

}



