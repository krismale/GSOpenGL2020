#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H

#include "visualobject.h"
#include <QVector3D>

class Vertex;
typedef QVector3D Vec3; // Gjør bare om navnet på Vector3d til Vec3d. Gjør det lettere å kopiere koden til Dag
class OctahedronBall : public VisualObject {
    public:
       OctahedronBall(unsigned int n=0);
       ~OctahedronBall() = default;

       void init(MyStructs::ShaderUniforms shaderUniforms) override;
       void draw() override;

       // Vanlige transformasjonsfunksjoner for ballen
       void move(float dx, float dy, float dz) override;
       void scale(float dx, float dy, float dz) override;
       void rotate(float dTilt, float dYaw, float dRoll) override;

       // Får ballen til å rulle
       void roll(float speed = 1.f, bool x = true, bool y = false, bool z = false);
       float mRollSpeed = 0.05f; // Hvor raskt ballen ruller

       // Setter IDen til objektet for outlineren
       // Jeg har ingen baller i scenen så det vil ikke være noen OctahedronBall som settes i outlineren
       void setID(int id) override {
           mID = id;
       }

    private:
       void lagTriangel(Vec3& v1, Vec3& v2, Vec3& v3);
       void subDivide(Vec3& a, Vec3& b, Vec3& c, unsigned int n);
       void oktaederUnitBall();

       unsigned int mRekursjoner; // Bestemmer hvor detaljert ballen blir
       int mIndeks;               // brukes i rekursjon, til å bygge m_vertices
};


#endif // OCTAHEDRONBALL_H
