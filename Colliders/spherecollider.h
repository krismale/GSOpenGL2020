#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "Colliders/collider.h"

class SphereCollider : public Collider
{
public:
    SphereCollider(float radius);
    SphereCollider();
    void init(MyStructs::ShaderUniforms shaderUniforms) override;
    void draw() override;

    // Funksjoner for å lage ballen som collideren skal formes som
    void oktaederUnitBall();
    void subDivide(QVector3D &a, QVector3D &b, QVector3D &c, unsigned int n);
    void lagTriangel(QVector3D& v1, QVector3D& v2, QVector3D& v3);


    unsigned int mRekursjoner = 3; // Bestemmer hvor detaljert ballen blir
    int mIndeks = 0;               // brukes i rekursjon, til å bygge m_vertices
    float mRadius = 0;             // Bestemmer radiusen til collideren. Skulle blitt brukt for collision testing
};

#endif // SPHERECOLLIDER_H
