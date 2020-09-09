#include <iostream>
#include <sstream>
#include <cmath>
#include "octahedronball.h"


//! \param n - the recursion level (default is 0 which makes the original Octahedron)
//!
//! The number of (duplicated) vertices are calculated based on the parameter n - the recursion level.
//! Initially we have 8 triangles, each with 3 vertices.
//! A subdivision consists of
//! - compute the midpoint of each edge in a triangel
//! - add these midpoint as vertices
//! - make three new edges by connecting new vertices
//! - preserve orientation
//! - move the three new points on the unit ball's surface by normalizing. Remember the vertex vector
//! is also a normal to the ball's surface.
//! The formula for the number of vertices is
//! m_nVertices = 3 * 8 * 4^n
//! where
//! - 3 comes from the number of vertices in a triangle,
//! - 8 comes from the original number of triangles in a regular Octahedron
//! - n is the recursion level (number of repeated subdivisions)
//!

OctahedronBall::OctahedronBall(unsigned int n)
    : mRekursjoner(n), mIndeks(0)
{
   mVertices.reserve(static_cast<int>(3 * 8 * pow(4, mRekursjoner)));
   oktaederUnitBall();

   scale(1, 1, 1);

   //mMatrix.MakeIdentityMatrix(); // Til min egen Matrix4x4 klasse
    mMatrix.setToIdentity();

}

// Parametre inn: xyz koordinatene til et triangle v1, v2, v3 ccw
// Bevarer orienteringen av hjørnene
//!
//! \brief OctahedronBall::lagTriangel()
//! \param v1 - position on the unit ball for vertex 1
//! \param v2 - position on the unit ball for vertex 2
//! \param v3 - position on the unit ball for vertex 3
//!
//! lagTriangel() creates vertex data for a triangle's 3 vertices. This is done in the
//! final step of recursion.
//!
void OctahedronBall::lagTriangel(Vec3& v1, Vec3& v2, Vec3& v3)
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
void OctahedronBall::subDivide(Vec3 &a, Vec3 &b, Vec3 &c, unsigned int n)
{
   if (n>0) {
       Vec3 v1 = a+b; v1.normalize();
       Vec3 v2 = a+c; v2.normalize();
       Vec3 v3 = c+b; v3.normalize();
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
void OctahedronBall::oktaederUnitBall()
{
   Vec3 v0{0, 0, 1};
   Vec3 v1{1, 0, 0};
   Vec3 v2{0, 1, 0};
   Vec3 v3{-1, 0, 0};
   Vec3 v4{0, -1, 0};
   Vec3 v5{0, 0, -1};

   subDivide(v0, v1, v2, mRekursjoner);
   subDivide(v0, v2, v3, mRekursjoner);
   subDivide(v0, v3, v4, mRekursjoner);
   subDivide(v0, v4, v1, mRekursjoner);
   subDivide(v5, v2, v1, mRekursjoner);
   subDivide(v5, v3, v2, mRekursjoner);
   subDivide(v5, v4, v3, mRekursjoner);
   subDivide(v5, v1, v4, mRekursjoner);
}

//!
//! \brief OctahedronBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//! for using later use of glDrawArrays()
//!
void OctahedronBall::init(MyStructs::ShaderUniforms shaderUniforms)
{
   initializeOpenGLFunctions();

   //Vertex Array Object - VAO
   glGenVertexArrays( 1, &mVAO );
   glBindVertexArray( mVAO );

   //Vertex Buffer Object to hold vertices - VBO
   glGenBuffers( 1, &mVBO );
   glBindBuffer( GL_ARRAY_BUFFER, mVBO );

   glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

   glBindBuffer(GL_ARRAY_BUFFER, mVBO);
   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);

   // enable the matrixUniform
   mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;
   if(!mTexture.empty()) {
       mShaderUniforms.mTextureUniform = shaderUniforms.mTextureUniform;
   }

   glBindVertexArray(0);

}

//!
//! \brief OctahedronBall::draw() draws a ball using glDrawArrays()
//! \param positionAttribute    vertex shader variable for position
//! \param normalAttribute      vertex shader variable for normal or color
//! \param textureAttribute     vertex shader variable for texture coordinates (optional)
//!
//! draw()
//! - glBindBuffer()
//! - glVertexAttribPointer()
//! - glBindTexture()
//! - glVertexAttribPointer()
//! - glDrawArrays() with GL_TRIANGLES
//!
void OctahedronBall::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   if(!mDrawDebugLines) {
       glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mVertices.size()));
   } else {
       glDrawArrays(GL_LINE_LOOP, 0, static_cast<int>(mVertices.size()));
   }

}

void OctahedronBall::move(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();
    mTransformation.translation.setX(mTransformation.translation.x() + dx);
    mTransformation.translation.setY(mTransformation.translation.y() + dy);
    mTransformation.translation.setZ(mTransformation.translation.z() + dz);

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mMatrix.scale(mTransformation.scale.toVector3D());
}

void OctahedronBall::scale(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mTransformation.scale = QVector4D(dx, dy, dz, 1);

    mMatrix.scale(mTransformation.scale.toVector3D());
}

void OctahedronBall::rotate(float dTilt, float dYaw, float dRoll)
{
    mMatrix.setToIdentity();
    mMatrix.translate(mTransformation.translation.toVector3D());

    if(dTilt > 0.f) {
        mTransformation.rotation.setX(mTransformation.rotation.x() + dTilt);
        mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    }
    if(dYaw > 0.f) {
        mTransformation.rotation.setY(mTransformation.rotation.y() + dYaw);
        mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    }
    if(dRoll > 0.f) {
        mTransformation.rotation.setZ(mTransformation.rotation.z() + dRoll);
        mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);
    }

    mMatrix.scale(mTransformation.scale.toVector3D());
}

void OctahedronBall::roll(float speed, bool x, bool y, bool z)
{
    if(x) {
        mTransformation.rotation.setZ(mTransformation.rotation.z() - speed*100);
        mTransformation.translation.setX(mTransformation.translation.x() + speed);
        mMatrix.setToIdentity();
        mMatrix.translate(mTransformation.translation.toVector3D());
        mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);
    }
    if(y) {
        mTransformation.rotation.setY(mTransformation.rotation.y() + speed*100);
        mTransformation.translation.setY(mTransformation.translation.y() + speed);
        mMatrix.setToIdentity();
        mMatrix.translate(mTransformation.translation.toVector3D());
        mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    }
    if(z) {
        mTransformation.rotation.setX(mTransformation.rotation.x() + speed*100);
        mTransformation.translation.setZ(mTransformation.translation.z() + speed);
        mMatrix.setToIdentity();
        mMatrix.translate(mTransformation.translation.toVector3D());
        mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    }

    scale(mTransformation.scale.x(),mTransformation.scale.y(),mTransformation.scale.z());
}





