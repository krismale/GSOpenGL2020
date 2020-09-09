#include "cubecollider.h"
#include "MyMath/misccalculations.h"
#include "spherecollider.h"


CubeCollider::CubeCollider(float height, float depth, float width)
{
    mVertices.push_back(Vertex{ (width/2),  (height/2),  (depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{ (width/2), -(height/2),  (depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{-(width/2),  (height/2),  (depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{-(width/2), -(height/2),  (depth/2),      0.f, 1.f, 0.f});

    mVertices.push_back(Vertex{ (width/2),  (height/2), -(depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{ (width/2), -(height/2), -(depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{-(width/2),  (height/2), -(depth/2),      0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{-(width/2), -(height/2), -(depth/2),      0.f, 1.f, 0.f});


    mIndices = {
        0, 1, 2,
        1, 2, 3, //front

        4, 5, 6,
        5, 6, 7, //back

        7, 2, 3,
        6, 2, 7, //left

        0, 4, 1,
        1, 5, 4,  //right

        0, 2, 6,
        0, 4, 6, //Top

        1, 3, 7,
        1, 5, 7
    };
    mMatrix.setToIdentity();
    calcInitHeight();

    mColliderType = ColliderType::CUBECOLLIDER;
}

void CubeCollider::init(MyStructs::ShaderUniforms shaderUniforms)
{
    Collider::init(shaderUniforms);
}



void CubeCollider::draw()
{
    Collider::draw();


    if(!mVisible) {
        return;
    }
    // Tegner elementene i mVertices ved å se på koblingene satt i mIndices
    // If testen sjekker om en skal tegne kun linjene eller faktisk facene
    if(!mDrawDebugLines) {
        glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawElements(GL_LINE_LOOP, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
    }
}

bool CubeCollider::checkBlockCollision(VisualObject *otherObject)
{

    // Sjekker alle de andre colliderne til det andre objektet
    for(const auto &it : otherObject->mColliders) {
        if(it->mCollisionOn && !it->mIsTrigger){
            if(it->mColliderType == ColliderType::CUBECOLLIDER) {
                bool collisionX = checkAxisCollision(mTransformation.translation.x(), mXWidth[1],
                                                        it->mTransformation.translation.x(), it->mXWidth[1]);

                bool collisionY = checkAxisCollision(mTransformation.translation.y(), mHeight[1],
                                                        it->mTransformation.translation.y(), it->mHeight[1]);

                bool collisionZ = checkAxisCollision(mTransformation.translation.z(), mZDepth[1],
                                                        it->mTransformation.translation.z(), it->mZDepth[1]);

                // Returnerer true hvis alle dimensjonene er trigret
                if(collisionX && collisionY && collisionZ) {
                    return collisionX && collisionY && collisionZ;
                }
            } else if(it->mColliderType == ColliderType::SPHERECOLLIDER) {
                // TODO: Sjekk om denne fungerer nå. Prøvde med ny compiler, kan hende den ikke gjør om
                // sirkelen til en firkant nå
                SphereCollider* collider = static_cast<SphereCollider*>(it);
                return checkCollidingWithSphere(collider);
            }

        }
    }
    // Kommer vi oss hit så returnerer vi false, fordi vi ikke har kollidert
    return false;

}

bool CubeCollider::checkTriggerCollision(VisualObject *otherObject)
{
    // Sjekker alle de andre colliderne til det andre objektet
    for(const auto &it : otherObject->mColliders) {
        if(it->mCollisionOn && it->mIsTrigger){
            if(it->mColliderType == ColliderType::CUBECOLLIDER) {
                bool collisionX = checkAxisCollision(mTransformation.translation.x(), mXWidth[1],
                                                        it->mTransformation.translation.x(), it->mXWidth[1]);

                bool collisionY = checkAxisCollision(mTransformation.translation.y(), mHeight[1],
                                                        it->mTransformation.translation.y(), it->mHeight[1]);

                bool collisionZ = checkAxisCollision(mTransformation.translation.z(), mZDepth[1],
                                                        it->mTransformation.translation.z(), it->mZDepth[1]);

                // Returnerer true hvis alle dimensjonene er trigret
                if(collisionX && collisionY && collisionZ) {
                    return collisionX && collisionY && collisionZ;
                }
            } else if(it->mColliderType == ColliderType::SPHERECOLLIDER) {
                // TODO: Sjekk om denne fungerer nå. Prøvde med ny compiler, kan hende den ikke gjør om
                // sirkelen til en firkant nå
                SphereCollider* collider = static_cast<SphereCollider*>(it);
                return checkCollidingWithSphere(collider);
            }

        }
    }
    // Kommer vi oss hit så returnerer vi false, fordi vi ikke har kollidert
    return false;
}

bool CubeCollider::checkAxisCollision(float thisDimensionPosition, float thisDimensionMeasurement, float otherDimensionPosition, float otherDimensionMeasurement)
{
    return ((thisDimensionPosition + (thisDimensionMeasurement/2)) >= (otherDimensionPosition - (otherDimensionMeasurement/2)) &&
            (otherDimensionPosition + (otherDimensionMeasurement/2)) >= (thisDimensionPosition - (thisDimensionMeasurement/2))) ||
            ((thisDimensionPosition - (thisDimensionMeasurement/2)) >= (otherDimensionPosition + (otherDimensionMeasurement/2)) &&
            (otherDimensionPosition - (otherDimensionMeasurement/2)) >= (thisDimensionPosition + (thisDimensionMeasurement/2))) ||

            (((thisDimensionPosition + (thisDimensionMeasurement/2)) <= (otherDimensionPosition - (otherDimensionMeasurement/2)) &&
            (otherDimensionPosition + (otherDimensionMeasurement/2)) <= (thisDimensionPosition - (thisDimensionMeasurement/2))) ||
            ((thisDimensionPosition - (thisDimensionMeasurement/2)) <= (otherDimensionPosition + (otherDimensionMeasurement/2)) &&
             (otherDimensionPosition - (otherDimensionMeasurement/2)) <= (thisDimensionPosition + (thisDimensionMeasurement/2))));
}

bool CubeCollider::checkCollidingWithSphere(SphereCollider *otherCollider)
{
    // Get center point circle first
    QVector3D center = otherCollider->mTransformation.translation.toVector3D();
            center.setX(center.x()+otherCollider->mRadius);
            center.setY(center.y()+otherCollider->mRadius);
            center.setZ(center.z()+otherCollider->mRadius);
    // Calculate AABB info (center, half-extents)
    QVector3D aabb_half_extents(mTransformation.translation.x()/2,
                                mTransformation.translation.y()/2,
                                mTransformation.translation.z()/2);
    QVector3D aabb_center(mTransformation.translation.x() + aabb_half_extents.x(),
                          mTransformation.translation.y() + aabb_half_extents.y(),
                          mTransformation.translation.z() + aabb_half_extents.z());
    // Get difference vector between both centers
    QVector3D difference = center - aabb_center;
    QVector3D clamped;
    clamped.setX(MiscCalculations::clamp(difference.x(), -aabb_half_extents.x(), aabb_half_extents.x()));
    clamped.setY(MiscCalculations::clamp(difference.y(), -aabb_half_extents.y(), aabb_half_extents.y()));
    clamped.setZ(MiscCalculations::clamp(difference.z(), -aabb_half_extents.z(), aabb_half_extents.z()));
    // Add clamped value to AABB_center and we get the value of box closest to circle
    QVector3D closest = aabb_center + clamped;
    // Retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    return difference.length() < otherCollider->mRadius;
}
