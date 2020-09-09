#include "misccalculations.h"
#include <cfloat>
#include <cmath>
#include "MyMath/mathconstants.h"

void MiscCalculations::getWorldTransform(QVector3D &p, MyStructs::Transformation objectTransform)
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    QVector3D objectTranslation = objectTransform.translation.toVector3D();
    QVector3D objectRotation =  objectTransform.rotation.toVector3D();
    QVector3D objectScaling =  objectTransform.scale.toVector3D();

    matrix.translate(objectTranslation);
    matrix.rotate(objectRotation.x(),1,0,0);
    matrix.rotate(objectRotation.y(),0,1,0);
    matrix.rotate(objectRotation.z(),0,0,1);
    matrix.scale(objectScaling);
    p = matrix * p;
}

bool MiscCalculations::cmpf(float A, float B, float epsilon)
{
    return (fabsf(A - B) < epsilon);
}

float MiscCalculations::randFloat(int min, int max) {
    return static_cast<float>(rand() % max + min);
}

float MiscCalculations::clamp(float value, float max, float min)
{
    return std::max(min, std::min(max, value));
}

float MiscCalculations::degreesToRadians(float degrees)
{
    return 2 * MiscCalculations::PIf * (degrees / 360);
}

void MiscCalculations::BarycentricCoord(const QVector3D &point, QVector3D &p1, QVector3D &p2, QVector3D &p3, const MyStructs::Transformation triangleTransform,
                      float &u, float &v, float &w)
{
    MiscCalculations::getWorldTransform(p1, triangleTransform);
    MiscCalculations::getWorldTransform(p2, triangleTransform);
    MiscCalculations::getWorldTransform(p3, triangleTransform);

    // Surface axis: x and z.
    QVector2D p{point.x(), point.z()};
    QVector2D a{p1.x(), p1.z()};
    QVector2D b{p2.x(), p2.z()};
    QVector2D c{p3.x(), p3.z()};

    QVector2D v0 = b - a;
    QVector2D v1 = c - a;
    QVector2D v2 = p - a;

    float d00 = QVector2D::dotProduct(v0, v0);
    float d01 = QVector2D::dotProduct(v0, v1);
    float d11 = QVector2D::dotProduct(v1, v1);
    float d20 = QVector2D::dotProduct(v2, v0);
    float d21 = QVector2D::dotProduct(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    v = (d11 * d20 - d01 * d21) * invDenom;
    w = (d00 * d21 - d01 * d20) * invDenom;
    u = 1.0f - v - w;
}

// Skal brukes istedenfor lineært søk i MoveOnTopOfGround
bool barycTestTriangle(VisualObject *object, QVector3D target, MyStructs::Triangles triangle, Terrain *ground, float &u, float &v, float &w, float &height)
{
    int p1Indice = triangle.indices[0];
    int p2Indice = triangle.indices[1];
    int p3Indice = triangle.indices[2];
    QVector3D p1 = QVector3D(ground->mVertices[p1Indice].GetX(), ground->mVertices[p1Indice].GetY(), ground->mVertices[p1Indice].GetZ());
    QVector3D p2 = QVector3D(ground->mVertices[p2Indice].GetX(), ground->mVertices[p2Indice].GetY(), ground->mVertices[p2Indice].GetZ());
    QVector3D p3 = QVector3D(ground->mVertices[p3Indice].GetX(), ground->mVertices[p3Indice].GetY(), ground->mVertices[p3Indice].GetZ());

    MiscCalculations::BarycentricCoord(target, p1, p2, p3,ground->mTransformation, u, v, w);

    if(MiscCalculations::ValidatePoint2D(u, v, w)) {
        height = MiscCalculations::calcBarycHeight(p1, p2, p3, ground->mTransformation, u, v, w);
        return true;
    } else {
        return false;
    }
}

MyStructs::Triangles *checkNeighborTriangles(VisualObject *object, QVector3D target, MyStructs::Triangles *currTriangle, MyStructs::Triangles *neighborTriangle, Terrain *ground, float &u, float &v, float &w, float &height) {
    for(unsigned int i = 0; i < currTriangle->neighborTrianglesIndices.size(); ++i) {
        neighborTriangle = &ground->mTriangles[currTriangle->neighborTrianglesIndices[i]];
        if(barycTestTriangle(object, target, *neighborTriangle, ground, u, v, w, height)) {
            return neighborTriangle;
        } else {
            for(unsigned int j = 0; j < neighborTriangle->neighborTrianglesIndices.size(); ++j) {
                MyStructs::Triangles* nx2Triangle = &ground->mTriangles[neighborTriangle->neighborTrianglesIndices[j]];
                if(nx2Triangle != currTriangle)
                    if(barycTestTriangle(object, target, *nx2Triangle, ground, u, v, w, height)) {
                        return nx2Triangle;
                    } else {
                        for(unsigned int k = 0; k < nx2Triangle->neighborTrianglesIndices.size(); ++k) {
                            MyStructs::Triangles* nx3Triangle = &ground->mTriangles[nx2Triangle->neighborTrianglesIndices[k]];
                            if(nx2Triangle != nx3Triangle && barycTestTriangle(object, target, *nx3Triangle, ground, u, v, w, height)) {
                                return nx3Triangle;
                            }
                        }
                    }
                }
            }
        }
}

MyStructs::Triangles *MiscCalculations::searchForNextTriangle(Terrain *ground, VisualObject *object, float &height)
{
    MyStructs::Triangles* currTriangle = static_cast<TriangleSurface*>(object)->mLastTriangle;
    QVector3D target = QVector3D(object->mTransformation.translation.x(), 0, object->mTransformation.translation.z());

    if(currTriangle) {
        float u, v, w;

        if(barycTestTriangle(object, target, *currTriangle, ground, u, v, w, height)) {
            return currTriangle;
        }

        MyStructs::Triangles* neighborTriangle = checkNeighborTriangles(object, target, currTriangle, neighborTriangle, ground, u, v, w, height);
        return neighborTriangle;
    }

    // NB! Dette er en backup løsning, i tilfelle programmet ikke finner riktig trekant.
    for(size_t i = 0; i < ground->mTriangles.size(); ++i) {
        int p1Indice = ground->mTriangles[i].indices[0];
        int p2Indice = ground->mTriangles[i].indices[1];
        int p3Indice = ground->mTriangles[i].indices[2];
        QVector3D p1 = QVector3D(ground->mVertices[p1Indice].GetX(), ground->mVertices[p1Indice].GetY(), ground->mVertices[p1Indice].GetZ());
        QVector3D p2 = QVector3D(ground->mVertices[p2Indice].GetX(), ground->mVertices[p2Indice].GetY(), ground->mVertices[p2Indice].GetZ());
        QVector3D p3 = QVector3D(ground->mVertices[p3Indice].GetX(), ground->mVertices[p3Indice].GetY(), ground->mVertices[p3Indice].GetZ());
        float u, v, w;
        MiscCalculations::BarycentricCoord(target, p1, p2, p3,ground->mTransformation, u, v, w);

        if(MiscCalculations::ValidatePoint2D(u, v, w)) {
            height = calcBarycHeight(p1, p2, p3, ground->mTransformation,u, v, w);
            return &ground->mTriangles[i];
        }
    }

    return currTriangle;
}

bool MiscCalculations::ValidatePoint2D(float u, float v, float w)
{
    float sum = u+v+w;
    if(u >= 0.0f && v >= 0.0f && w >= 0.0f && sum == 1.0f) {
        return true;
    }else {
        return false;
    }
}

float MiscCalculations::lerp(float a, float b, float step)
{
    return a * (1-step) + b * step;
}

float MiscCalculations::calcBarycHeight(QVector3D &p1, QVector3D &p2, QVector3D &p3,
                                       const MyStructs::Transformation triangleTransform,
                                       float u, float v, float w)
{
    MiscCalculations::getWorldTransform(p1, triangleTransform);
    MiscCalculations::getWorldTransform(p2, triangleTransform);
    MiscCalculations::getWorldTransform(p3, triangleTransform);

    return (p1.y() * u + p2.y() * v + p3.y() * w);
}
