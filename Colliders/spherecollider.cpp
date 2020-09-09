#include "spherecollider.h"
#include <cmath>

SphereCollider::SphereCollider(float radius) : mRadius(radius)
{
    mVertices.reserve(static_cast<int>(3 * 8 * pow(4, mRekursjoner)));
    oktaederUnitBall();

    scale(1, 1, 1);

    mMatrix.setToIdentity();

    mColliderType = ColliderType::SPHERECOLLIDER;
}

SphereCollider::SphereCollider() {

}

void SphereCollider::init(MyStructs::ShaderUniforms shaderUniforms)
{
    Collider::init(shaderUniforms);
    scale(mRadius, mRadius, mRadius);
}

void SphereCollider::draw()
{
    Collider::draw();

    if(!mVisible) {
        return;
    }
    if(!mDrawDebugLines) {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mIndices.size()));
    } else {
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(mIndices.size()));
    }

}

void SphereCollider::lagTriangel(QVector3D& v1, QVector3D& v2, QVector3D& v3)
{
   Vertex v;
   v.SetXYZ(v1.x(), v1.y(), v1.z());		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
   v.SetRGB(v1.x(), v1.y(), v1.z());       // rgb
   v.SetST(0.0f, 0.0f);			// kan utelates
   mVertices.push_back(v);

   Vertex vv;
   vv.SetXYZ(v2.x(), v2.y(), v2.z());		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
   vv.SetRGB(v2.x(), v2.y(), v2.z());
   vv.SetST(1.0f, 0.0f);
   mVertices.push_back(vv);

   Vertex vvv;
   vvv.SetXYZ(v3.x(), v3.y(), v3.z());		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
   vvv.SetRGB(v3.x(), v3.y(), v3.z());
   vvv.SetST(0.5f, 1.0f);
   mVertices.push_back(vvv);

}

// Rekursiv subdivisjon av triangel
//!
//! \brief OctahedronBall::subDivide() recursive subdivision of a triangel
//! \param a coordinates for vertex a
//! \param b coordinates for vertex b
//! \param c coordinates for vertex c
//! \param n level of recursion
//!
//! The function tests
//! (if n>0)
//! - three new edge midpoints are computed and normalized,
//! - four new calls to subDivide() is done - one for each new triangle, preserving orientation
//! and with decreased parameter n
//! else
//! - call lagTriangel(a, b, c)
//!
void SphereCollider::subDivide(QVector3D &a, QVector3D &b, QVector3D &c, unsigned int n)
{
   if (n>0) {
       QVector3D v1 = a+b; v1.normalize();
       QVector3D v2 = a+c; v2.normalize();
       QVector3D v3 = c+b; v3.normalize();
       subDivide(a, v1, v2, n-1);
       subDivide(c, v2, v3, n-1);
       subDivide(b, v3, v1, n-1);
       subDivide(v3, v2, v1, n-1);
   } else {
       lagTriangel(a, b, c);
   }
}

//!
//! \brief OctahedronBall::oktaederUnitBall() creates 8 unit ball vertices and call subDivide()
//!
//! If the parameter n of the constructor OctahedronBall() is zero, the result will be the
//! original octahedron consisting of 8 triangles with duplicated vertices.
//!
void SphereCollider::oktaederUnitBall()
{
   QVector3D v0{0, 0, 1};
   QVector3D v1{1, 0, 0};
   QVector3D v2{0, 1, 0};
   QVector3D v3{-1, 0, 0};
   QVector3D v4{0, -1, 0};
   QVector3D v5{0, 0, -1};

   subDivide(v0, v1, v2, mRekursjoner);
   subDivide(v0, v2, v3, mRekursjoner);
   subDivide(v0, v3, v4, mRekursjoner);
   subDivide(v0, v4, v1, mRekursjoner);
   subDivide(v5, v2, v1, mRekursjoner);
   subDivide(v5, v3, v2, mRekursjoner);
   subDivide(v5, v4, v3, mRekursjoner);
   subDivide(v5, v1, v4, mRekursjoner);
}
