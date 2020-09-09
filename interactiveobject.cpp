#include "interactiveobject.h"
#include "MyMath/misccalculations.h"

InteractiveObject::InteractiveObject()
{

}

InteractiveObject::InteractiveObject(const char *path, const char *texture) : TriangleSurface(path, texture)
{

}

InteractiveObject::InteractiveObject(const char *path, QVector3D color) : TriangleSurface(path, color)
{

}

void InteractiveObject::setToGameStart(Terrain* ground)
{
    setTranslationForAll(mGameStartPos.x(), mGameStartPos.y(), mGameStartPos.z());

    move(0,0,0);
    placeOnTopOfGround(ground, 0.4f);
}

void InteractiveObject::setTranslationForAll(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);

    for(const auto &it : mColliders) {
        it->mTransformation.translation = QVector4D(x, y, z, 1);
    }
}

void InteractiveObject::moveBetweenTwoPoints(QVector3D pushedFrom, Terrain *ground, std::vector<VisualObject *> otherObjects)
{
    if(mJustPushed) {
        mInitPos = mTransformation.translation.toVector3D();
        mMoveTo = mInitPos + (pushedFrom * mPushDistance);
        mStep = mSpeed/(pushedFrom * mPushDistance).length();
        mStepIncrement = mStep;
        mMoving = true;
        mJustPushed = false;
    }


    if(mMoving) {
        mStep += mStepIncrement;
        mTransformation.translation.setX(MiscCalculations::lerp(mInitPos.x(), mMoveTo.x(), mStep));
        mTransformation.translation.setZ(MiscCalculations::lerp(mInitPos.z(), mMoveTo.z(), mStep));

        for(const auto &it : mColliders) {
            it->mTransformation.translation.setX(MiscCalculations::lerp(mInitPos.x(), mMoveTo.x(), mStep));
            it->mTransformation.translation.setZ(MiscCalculations::lerp(mInitPos.z(), mMoveTo.z(), mStep));
        }
        moveAboveGround(0,0,0, ground, otherObjects);
        if(mStep > 1) {
            mMoving = false;
        }
    }
}


