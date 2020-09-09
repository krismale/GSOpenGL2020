#include "billboard.h"
#include <cmath>

Billboard::Billboard()
{
    mVertices.push_back(Vertex{0.5f, 0.5f, 0,   1,0,0,      1,1});
    mVertices.push_back(Vertex{-0.5f, 0.5f, 0,   1,0,0,     0,1});
    mVertices.push_back(Vertex{-0.5f, -0.5f, 0,   1,0,0,    0,0});
    mVertices.push_back(Vertex{0.5f, -0.5f, 0,   1,0,0,     1,0});

    mIndices = {
        0, 1, 2,
        2, 3, 0
    };

    mTransformation.scale = QVector4D(1,1,1,1);

    mMatrix.setToIdentity();
    mLabel = "Billboard";
}

Billboard::Billboard(const char *texture)
{

    mVertices.push_back(Vertex{ 0.5f,  0.5f, 0,   1,0,0,    1,1});
    mVertices.push_back(Vertex{-0.5f,  0.5f, 0,   1,0,0,    0,1});
    mVertices.push_back(Vertex{-0.5f, -0.5f, 0,   1,0,0,    0,0});
    mVertices.push_back(Vertex{ 0.5f, -0.5f, 0,   1,0,0,    1,0});

    mIndices = {
        0, 1, 2,
        2, 3, 0
    };

    mTransformation.scale = QVector4D(1,1,1,1);

    mMatrix.setToIdentity();

    if(texture) {
        mTexture.push_back(MyStructs::Texture());
        mTexture.back().path = texture;
    }

    mLabel = "Billboard";

}

void Billboard::init(MyStructs::ShaderUniforms shaderUniforms)
{
    initializeOpenGLFunctions();

    // Makes textures for the object
    if(!mTexture.empty()) {
        generateTexture();
    }

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

    // 3rd attribute buffer : texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //Indices Buffer Object to connect vertices - IBO
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;
    mShaderUniforms.mObjectColorUniform = shaderUniforms.mObjectColorUniform;
    mShaderUniforms.mSpecularStrengthUniform = shaderUniforms.mSpecularStrengthUniform;
    mShaderUniforms.mSpecularExponentUniform = shaderUniforms.mSpecularExponentUniform;
    mShaderUniforms.mObjectCenterUniform = shaderUniforms.mObjectCenterUniform;
    mShaderUniforms.mObjectSizeUniform = shaderUniforms.mObjectSizeUniform;
    mShaderUniforms.mUiElementUniform = shaderUniforms.mUiElementUniform;
    if(!mTexture.empty()) {
        mShaderUniforms.mTextureUniform = shaderUniforms.mTextureUniform;
    }
}

void Billboard::draw()
{
    if(mVisible) {

        // håndterer textures
        for(unsigned int i = 0; i < mTexture.size(); ++i){
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTexture.at(i).textureID);
            glUniform1i(mShaderUniforms.mTextureUniform, i);
        }

        glUniform1f(mShaderUniforms.mSpecularStrengthUniform, mSpecularStrength);
        glUniform1i(mShaderUniforms.mSpecularExponentUniform, mSpecularExponent);
        glUniform3f(mShaderUniforms.mObjectColorUniform, mObjectColor.x(), mObjectColor.y(), mObjectColor.z());

        glUniform3f(mShaderUniforms.mObjectCenterUniform, mTransformation.translation.x(), mTransformation.translation.y(), mTransformation.translation.z());
        glUniform3f(mShaderUniforms.mObjectSizeUniform, mTransformation.scale.x(), mTransformation.scale.y(), mTransformation.scale.z());
        glUniform1i(mShaderUniforms.mUiElementUniform, mUiElement);
        // Binder vertex
        glBindVertexArray( mVAO );
        glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

        // Tegner elementene i mVertices ved å se på koblingene satt i mIndices
        // If testen sjekker om en skal tegne kun linjene eller faktisk facene
        if(!mDrawDebugLines) {
            glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawElements(GL_LINE_LOOP, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        }

    }
}

void Billboard::move(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();

    setTranslation(dx, dy, dz);

    mMatrix.translate(mTransformation.translation.toVector3D());

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mMatrix.scale(mTransformation.scale.toVector3D());
}

void Billboard::setScale(float dx, float dy, float dz)
{
    mTransformation.scale = QVector4D(dx, dy, dz, 1);
}

void Billboard::scale(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    setScale(dx, dy, dz);

    mMatrix.scale(mTransformation.scale.toVector3D());
}

void Billboard::setTranslation(float x, float y, float z)
{
    mTransformation.translation.setX(mTransformation.translation.x() + x);
    mTransformation.translation.setY(mTransformation.translation.y() + y);
    mTransformation.translation.setZ(mTransformation.translation.z() + z);
}

void Billboard::generateTexture()
{
    for(unsigned int i = 0; i < mTexture.size(); ++i){

        //glEnable( GL_BLEND );
        //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glGenTextures(1, &mTexture.at(i).textureID);
        glBindTexture(GL_TEXTURE_2D, mTexture.at(i).textureID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        int channelCount = 4;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(mTexture.at(i).path.c_str(), &width, &height, &nrChannels, channelCount);

        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            qDebug() << "Failed to load texture";
        }
        stbi_image_free(data);
    }
}

void Billboard::placeOnTopOfGround(Terrain *ground, float addedHeight)
{
    QVector3D xPoint = mTransformation.translation.toVector3D();

    for(size_t i = 0; i < ground->mTriangles.size(); ++i) {
        int p1Indice = ground->mTriangles[i].indices[0];
        int p2Indice = ground->mTriangles[i].indices[1];
        int p3Indice = ground->mTriangles[i].indices[2];
        QVector3D p1 = QVector3D(ground->mVertices[p1Indice].GetX(), ground->mVertices[p1Indice].GetY(), ground->mVertices[p1Indice].GetZ());
        QVector3D p2 = QVector3D(ground->mVertices[p2Indice].GetX(), ground->mVertices[p2Indice].GetY(), ground->mVertices[p2Indice].GetZ());
        QVector3D p3 = QVector3D(ground->mVertices[p3Indice].GetX(), ground->mVertices[p3Indice].GetY(), ground->mVertices[p3Indice].GetZ());
        float u, v, w;
        MiscCalculations::BarycentricCoord(xPoint, p1, p2, p3,ground->mTransformation, u, v, w);

        if(MiscCalculations::ValidatePoint2D(u, v, w)) {
            float height = MiscCalculations::calcBarycHeight(p1,p2,p3,ground->mTransformation, u, v, w);
            mTransformation.translation.setY(height + addedHeight); // Hvis objektet ender midt inne i bakken, legg til mHeight[1]/2
            move(0,0,0);
            break;
        }
    }
}
