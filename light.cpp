#include "light.h"
#include "meshImporter.h"

Light::Light()
{
    MeshImport::loadObj("../GSOpenGL2020/Assets/lightBulb.obj", mVertices, mIndices);

    for(unsigned int i = 0; i < mVertices.size(); ++i) {
        mVertices[i].m_normal[0] = 1;
        mVertices[i].m_normal[1] = 1;
        mVertices[i].m_normal[2] = 0;
    }

    mTransformation.scale = QVector4D(1,1,1,1);
    mMatrix.setToIdentity();
}

void Light::init(MyStructs::ShaderUniforms phongShaderUniforms, MyStructs::ShaderUniforms meshShaderUniforms, MyStructs::ShaderUniforms billboardShaderUniforms)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //Indices Buffer Object to connect vertices - IBO
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform               = meshShaderUniforms.mMatrixUniform;
    mPhongShaderUniforms.mMatrixUniform          = phongShaderUniforms.mMatrixUniform;
    mPhongShaderUniforms.mLightPosUniform        = phongShaderUniforms.mLightPosUniform;
    mPhongShaderUniforms.mLightStrengthUniform   = phongShaderUniforms.mLightStrengthUniform;
    mPhongShaderUniforms.mLightPowerUniform      = phongShaderUniforms.mLightPowerUniform;

    mBillboardShaderUniforms.mMatrixUniform          = billboardShaderUniforms.mMatrixUniform;
    mBillboardShaderUniforms.mLightPosUniform        = billboardShaderUniforms.mLightPosUniform;
    mBillboardShaderUniforms.mLightStrengthUniform   = billboardShaderUniforms.mLightStrengthUniform;
    mBillboardShaderUniforms.mLightPowerUniform      = billboardShaderUniforms.mLightPowerUniform;


    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    glBindVertexArray(0);
}


void Light::draw()
{
    // Binder vertex
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    if(mVisible) {
        // Tegner elementene i mVertices ved å se på koblingne satt i mIndices
        glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
    }

}

void Light::renderLight()
{
    // Binder vertex
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mPhongShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    // Bind uniforms
    glUniform3f(mPhongShaderUniforms.mLightPosUniform,
                mTransformation.translation.x(), mTransformation.translation.y(), mTransformation.translation.z());
    glUniform1f(mPhongShaderUniforms.mLightPowerUniform, mLightPower);
    glUniform1f(mPhongShaderUniforms.mLightStrengthUniform, mLightStrength);
}

void Light::renderLightBillboard()
{
    // Binder vertex
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mBillboardShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

    // Bind uniforms
    glUniform3f(mBillboardShaderUniforms.mLightPosUniform,
                mTransformation.translation.x(), mTransformation.translation.y(), mTransformation.translation.z());
    glUniform1f(mBillboardShaderUniforms.mLightPowerUniform, mLightPower);
    glUniform1f(mBillboardShaderUniforms.mLightStrengthUniform, mLightStrength);
}

void Light::setTranslation(float x, float y, float z)
{
    mTransformation.translation.setX(mTransformation.translation.x() + x);
    mTransformation.translation.setY(mTransformation.translation.y() + y);
    mTransformation.translation.setZ(mTransformation.translation.z() + z);
}

void Light::setTranslationForAll(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);
}

void Light::setScaleForAll(float x, float y, float z)
{
    setScale(x, y, z);
}

void Light::setRotationForAll(float roll, float yaw, float pitch)
{
    mTransformation.rotation = QVector4D(roll, yaw, pitch, 1);
}

void Light::setScale(float dx, float dy, float dz)
{
    mTransformation.scale = QVector4D(dx, dy, dz, 1);
}

void Light::move(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();
    setTranslation(dx, dy, dz);

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mMatrix.scale(mTransformation.scale.toVector3D());
}
