#include "collider.h"

Collider::Collider()
{
    mDrawDebugLines = true;
    mCollisionOn = true;
    mIsTrigger = false;
    mVisible = false;

    scale(1,1,1);
}

void Collider::init(MyStructs::ShaderUniforms shaderUniforms)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    generatePhongColors();

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
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;

    glBindVertexArray(0);
}

void Collider::generatePhongColors()
{
    glGenTextures(1, &mColorBasedRenderTexture.textureID);

    GLubyte data[] = { 255, 0, 0, 255 };

    glBindTexture(GL_TEXTURE_2D, mColorBasedRenderTexture.textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Collider::draw()
{
    glBindTexture(GL_TEXTURE_2D, mColorBasedRenderTexture.textureID);

    // Binder vertex
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

}

void Collider::move(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();
    setTranslation(dx, dy, dz);

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mMatrix.scale(mTransformation.scale.toVector3D());

}

void Collider::moveTo(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);

    move(0,0,0);

}

void Collider::setTranslation(float x, float y, float z)
{
    mTransformation.translation.setX(mTransformation.translation.x() + x);
    mTransformation.translation.setY(mTransformation.translation.y() + y);
    mTransformation.translation.setZ(mTransformation.translation.z() + z);
}

void Collider::setScale(float dx, float dy, float dz)
{
    mTransformation.scale = QVector4D(dx, dy, dz, 1);
}

void Collider::scale(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    setScale(dx, dy, dz);

    mMatrix.scale(mTransformation.scale.toVector3D());

    mHeight[1] = mHeight[0] * dy;
    mXWidth[1] = mXWidth[0] * dx;
    mZDepth[1] = mZDepth[0] * dz;
}

// TODO: Oppdater denne til å være en OOBB (eller OBB), noe som vil si at boksen alltid former seg etter
// vertexene til objektet som eier boksen
// Dette kangjøres ved hjelp av SAT, eller Separating Axis Theorem
void Collider::rotate(float dTilt, float dYaw, float dRoll)
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

void Collider::calcInitHeight()
{
    float topVertexYPos = 0;
    float bottomVertexYPos = 0;
    float topVertexXPos = 0;
    float bottomVertexXPos = 0;
    float topVertexZPos = 0;
    float bottomVertexZPos = 0;
    for(unsigned int i = 0; i < mVertices.size(); ++i) {
        // Finner den største og minste verdien i Y aksen, for å finne høyden
        if(mVertices[i].GetY() > topVertexYPos) {
            topVertexYPos = mVertices[i].GetY();
        } else if(mVertices[i].GetY() < bottomVertexYPos) {
            bottomVertexYPos = mVertices[i].GetY();
        }

        // Finner den største og minste verdien i X aksen, for å finne bredden
        if(mVertices[i].GetX() > topVertexXPos) {
            topVertexXPos = mVertices[i].GetX();
        } else if(mVertices[i].GetX() < bottomVertexXPos) {
            bottomVertexXPos = mVertices[i].GetX();
        }

        // Finner den største og minste verdien i Z aksen, for å finne dybden
        if(mVertices[i].GetZ() > topVertexZPos) {
            topVertexZPos = mVertices[i].GetZ();
        } else if(mVertices[i].GetZ() < bottomVertexZPos) {
            bottomVertexZPos = mVertices[i].GetZ();
        }

        if(i+1 >= mVertices.size()) {
            mHeight[0] = topVertexYPos - bottomVertexYPos;
            mXWidth[0] = topVertexXPos - bottomVertexXPos;
            mZDepth[0] = topVertexZPos - bottomVertexZPos;
            mHeight[1] = topVertexYPos - bottomVertexYPos;
            mXWidth[1] = topVertexXPos - bottomVertexXPos;
            mZDepth[1] = topVertexZPos - bottomVertexZPos;
        }
    }
}
