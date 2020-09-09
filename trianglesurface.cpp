#include "trianglesurface.h"
#include <fstream>
#include <iostream>
#include "meshImporter.h"
#include <chrono>
#include <QVector2D>
#include "MyMath/misccalculations.h"


TriangleSurface::TriangleSurface()
{
    mTransformation.scale = QVector4D(1,1,1,1);
    mMatrix.setToIdentity();
    mLabel = "TriangleSurface";
}

void TriangleSurface::calcInitHeight()
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

void TriangleSurface::generateTexture()
{
    for(unsigned int i = 0; i < mTexture.size(); ++i){
        glGenTextures(1, &mTexture.at(i).textureID);
        glBindTexture(GL_TEXTURE_2D, mTexture.at(i).textureID);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(mTexture.at(i).path.c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            qDebug() << "Failed to load texture";
        }
        stbi_image_free(data);
    }
}

TriangleSurface::TriangleSurface(const char *path, const char *texture)
{
    std::string type = MeshImport::typeInterpreter(path);
    if(type == "obj") {
        MeshImport::loadObj(path, mVertices, mIndices);
    } else if(type == "txt") {
        MeshImport::readFile(path, mVertices, mIndices);
    }

    mTransformation.scale = QVector4D(1,1,1,1);
    mMatrix.setToIdentity(); // Til QMatrix

    calcInitHeight();

    if(texture) {
        mTexture.push_back(MyStructs::Texture());
        mTexture.back().path = texture;
    }
    mLabel = "TriangleSurface";

}

TriangleSurface::TriangleSurface(const char *path, QVector3D color)
{
    std::string type = MeshImport::typeInterpreter(path);
    if(type == "obj") {
        MeshImport::loadObj(path, mVertices, mIndices);
    } else if(type == "txt") {
        MeshImport::readFile(path, mVertices, mIndices);
    }

    mTransformation.scale = QVector4D(1,1,1,1);
    mMatrix.setToIdentity(); // Til QMatrix

    mObjectColor = color;

    calcInitHeight();
    mLabel = "TriangleSurface";

}

TriangleSurface::~TriangleSurface()
{

}


void TriangleSurface::generatePhongColors()
{
    glGenTextures(1, &mColorBasedRenderTexture.textureID);

    GLubyte data[] = { static_cast<GLubyte>(255 * mObjectColor.x()),
                       static_cast<GLubyte>(255 * mObjectColor.y()),
                       static_cast<GLubyte>(255 * mObjectColor.z()), 255 };

    glBindTexture(GL_TEXTURE_2D, mColorBasedRenderTexture.textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void TriangleSurface::renderAllColliders(bool render)
{
    for(auto &it : mColliders) {
        it->mVisible = render;
    }
}

void TriangleSurface::init(MyStructs::ShaderUniforms shaderUniforms)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // Makes textures for the object
    if(!mTexture.empty()) {
        generateTexture();
    } else {
        generatePhongColors();
    }


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

    for(size_t i = 0; i < mColliders.size(); ++i) {
        mColliders[i]->init(shaderUniforms);
    }


    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;
    mShaderUniforms.mObjectColorUniform = shaderUniforms.mObjectColorUniform;
    mShaderUniforms.mSpecularStrengthUniform = shaderUniforms.mSpecularStrengthUniform;
    mShaderUniforms.mSpecularExponentUniform = shaderUniforms.mSpecularExponentUniform;
    if(!mTexture.empty()) {
        mShaderUniforms.mTextureUniform = shaderUniforms.mTextureUniform;
    }

    glBindVertexArray(0);
}

void TriangleSurface::moveAlongCurve(unsigned int increment, std::vector<QVector3D> points, Terrain* ground, std::vector<VisualObject*> otherObjects)
{
    mMatrix.setToIdentity();

    if(increment < points.size()) {
        mTransformation.translation.setX(points.at(increment).x());
        mTransformation.translation.setY(points.at(increment).y());
        mTransformation.translation.setZ(points.at(increment).z());
        for(unsigned int i = 0; i < mColliders.size(); ++i) {
            mColliders[i]->mTransformation.translation.setX(points.at(increment).x());
            mColliders[i]->mTransformation.translation.setY(points.at(increment).y() + mHeight[1]/2);
            mColliders[i]->mTransformation.translation.setZ(points.at(increment).z());
        }

    }

    for(unsigned int i = 0; i < mColliders.size(); ++i) {
        mColliders[i]->move(0, 0, 0);
    }
    moveAboveGround(0,0,0, ground, otherObjects, false);

}

void TriangleSurface::addTranslation(float dx, float dy, float dz)
{
    mTransformation.translation.setX(mTransformation.translation.x() + dx);
    mTransformation.translation.setY(mTransformation.translation.y() + dy);
    mTransformation.translation.setZ(mTransformation.translation.z() + dz);
}

void TriangleSurface::addRotation(float deltaTilt, float deltaYaw, float deltaRoll)
{
    mTransformation.rotation.setX(mTransformation.translation.x() + deltaTilt);
    mTransformation.rotation.setY(mTransformation.translation.y() + deltaYaw);
    mTransformation.rotation.setZ(mTransformation.translation.z() + deltaRoll);
}

void TriangleSurface::addScale(float dx, float dy, float dz)
{
    mTransformation.translation.setX(mTransformation.scale.x() + dx);
    mTransformation.translation.setY(mTransformation.scale.y() + dy);
    mTransformation.translation.setZ(mTransformation.scale.z() + dz);
}

void TriangleSurface::moveAboveGround(float x, float y, float z, Terrain *ground, std::vector<VisualObject *> otherObjects, bool adjustHeight)
{
    if(validateMove(x,y,z, otherObjects)) {
        move(x, y, z);
        float height = 0.0;
        mLastTriangle = MiscCalculations::searchForNextTriangle(ground, this, height);

        mTransformation.translation.setY(height + (mHeight[1]/2));
        for(const auto &it : mColliders){
            if(adjustHeight) {
                it->mTransformation.translation.setY(height + (it->mHeight[1]/2));
            } else {
                it->mTransformation.translation.setY(height);
            }
        }
        move(0, 0, 0);
    }
}

void TriangleSurface::placeOnTopOfGround(Terrain *ground, float addedHeight)
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
            float height = MiscCalculations::calcBarycHeight(p1, p2, p3, ground->mTransformation, u, v, w);
            mTransformation.translation.setY(height + addedHeight); // Hvis objektet ender midt inne i bakken, legg til mHeight[1]/2
            for(const auto &it : mColliders){
                // Hvis feil posisjon på colliders, legg til mHeight[1]/2 istedenfor Y-translation
                it->mTransformation.translation.setY(height + it->mTransformation.translation.y());
            }
            move(0,0,0);
            mLastTriangle = &ground->mTriangles[i];
            break;
        }
    }
}

void TriangleSurface::setColor(QVector3D color)
{
    mObjectColor = color;

}

void TriangleSurface::draw()
{
    if(mVisible) {
        // håndterer textures
        // Dersom det ikke finnes noen texture, så brukes mObjectColor som basis for fargen til objektet
        // Måtte også lage en egen Texture variabel for å håndtere dette. Den bruker textureID her
        if(mTexture.empty()) {
            glBindTexture(GL_TEXTURE_2D, mColorBasedRenderTexture.textureID);
        } else {
            for(unsigned int i = 0; i < mTexture.size(); ++i){
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, mTexture.at(i).textureID);
                glUniform1i(mShaderUniforms.mTextureUniform, i);
            }
        }


        glUniform1f(mShaderUniforms.mSpecularStrengthUniform, mSpecularStrength);
        glUniform1i(mShaderUniforms.mSpecularExponentUniform, mSpecularExponent);
        glUniform3f(mShaderUniforms.mObjectColorUniform, mObjectColor.x(), mObjectColor.y(), mObjectColor.z());

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

    for(size_t i = 0; i < mColliders.size(); ++i) {
        mColliders[i]->draw();
    }
}

void TriangleSurface::setTranslationForAll(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);

    for(const auto &it : mColliders) {
        QVector3D posRelativeToOwner = QVector3D(it->mTransformation.translation.x() - mTransformation.translation.x(),
                                                 it->mTransformation.translation.y() - mTransformation.translation.y(),
                                                 it->mTransformation.translation.z() - mTransformation.translation.z());
        it->mTransformation.translation.setX(posRelativeToOwner.x() + x);
        it->mTransformation.translation.setY(posRelativeToOwner.y() + y);
        it->mTransformation.translation.setZ(posRelativeToOwner.z() + z);
    }
}

void TriangleSurface::setScaleForAll(float x, float y, float z)
{
    setScale(x, y, z);
    for(const auto &it : mColliders) {
        it->setScale(x, y, z);
    }
}

void TriangleSurface::setRotationForAll(float pitch, float yaw, float roll)
{
    mTransformation.rotation = QVector4D(pitch, yaw, roll, 1);

    for(const auto &it : mColliders) {
        it->mTransformation.rotation = QVector4D(pitch, yaw, roll, 1);
    }
}

void TriangleSurface::calcMoveDirection(float dy, float dx, float dz)
{
    if(QVector3D(dx, dy, dz).length() > 0) {
        mMoveDirection = QVector3D(dx, dy, dz);
        mMoveDirection.normalize();
    }
}

void TriangleSurface::move(float dx, float dy, float dz)
{
    mMatrix.setToIdentity();

    calcMoveDirection(dy, dx, dz);

    addTranslation(dx, dy, dz);

    mMatrix.translate(mTransformation.translation.toVector3D()); // Til min egen Matrix4x4 klasse

    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);

    mMatrix.scale(mTransformation.scale.toVector3D());

    for(unsigned int i = 0; i < mColliders.size(); ++i) {
        mColliders[i]->move(dx, dy, dz);
    }

}

void TriangleSurface::moveTo(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);

    for(const auto &it : mColliders) {
        it->moveTo(x, y, z);
    }
    move(0,0,0);

}

void TriangleSurface::rotate(float dTilt, float dYaw, float dRoll)
{
    mMatrix.setToIdentity();
    mMatrix.translate(mTransformation.translation.toVector3D());

    if(dTilt > 0.f) {
        mTransformation.rotation.setX(mTransformation.rotation.x() + dTilt);
    }
    mMatrix.rotate(mTransformation.rotation.x(), 1,0,0);
    if(dYaw > 0.f) {
        mTransformation.rotation.setY(mTransformation.rotation.y() + dYaw);

    }
    mMatrix.rotate(mTransformation.rotation.y(), 0,1,0);
    if(dRoll > 0.f) {
        mTransformation.rotation.setZ(mTransformation.rotation.z() + dRoll);
    }
    mMatrix.rotate(mTransformation.rotation.z(), 0,0,1);


    mMatrix.scale(mTransformation.scale.toVector3D());

    for(const auto &it : mColliders) {
        it->rotate(dTilt, dYaw, dRoll);
    }
}

void TriangleSurface::setScale(float x, float y, float z)
{
    mTransformation.scale = QVector4D(x, y, z, 1);
}

void TriangleSurface::setTranslation(float x, float y, float z)
{
    mTransformation.translation = QVector4D(x, y, z, 1);
}

void TriangleSurface::setRotation(float tilt, float yaw, float roll)
{
    mTransformation.rotation = QVector4D(tilt, yaw, roll, 1);
}

bool TriangleSurface::validateMove(float x, float y, float z, std::vector<VisualObject *> otherObjects)
{
    for(const auto &itThisCollider : mColliders) {
        itThisCollider->setTranslation(x,y,z);
        for(const auto &itObj : otherObjects) {
            if(itObj == this || itObj->mColliders.empty()) {
                continue;
            } else {
                if(itThisCollider->checkBlockCollision(itObj)) {
                    itThisCollider->setTranslation(-x,-y,-z);
                    return false;
               } else {
               }
            }
        }
        itThisCollider->setTranslation(-x,-y,-z);
    }
    return true;
}

// Brukes kun til terrain.
void TriangleSurface::calcNormals()
{
    for(unsigned int i = 0; i < mIndices.size(); ++i) {
        int p1Indice = i;
        int p2Indice = ++i;
        int p3Indice = ++i;

        QVector3D p2ToP1 = QVector3D(mVertices[mIndices[p1Indice]].m_xyz[0],
                                    mVertices[mIndices[p1Indice]].m_xyz[1],
                                    mVertices[mIndices[p1Indice]].m_xyz[2]) -
                           QVector3D(mVertices[mIndices[p2Indice]].m_xyz[0],
                                    mVertices[mIndices[p2Indice]].m_xyz[1],
                                    mVertices[mIndices[p2Indice]].m_xyz[2]);
        QVector3D p2ToP3 = QVector3D(mVertices[mIndices[p3Indice]].m_xyz[0],
                                    mVertices[mIndices[p3Indice]].m_xyz[1],
                                    mVertices[mIndices[p3Indice]].m_xyz[2]) -
                           QVector3D(mVertices[mIndices[p2Indice]].m_xyz[0],
                                    mVertices[mIndices[p2Indice]].m_xyz[1],
                                    mVertices[mIndices[p2Indice]].m_xyz[2]);

        QVector3D normals = QVector3D::crossProduct(p2ToP3, p2ToP1);
        normals.normalize();

        mVertices[mIndices[p1Indice]].SetRGB(normals.x(), normals.y(), normals.z());
        mVertices[mIndices[p2Indice]].SetRGB(normals.x(), normals.y(), normals.z());
        mVertices[mIndices[p3Indice]].SetRGB(normals.x(), normals.y(), normals.z());
    }
}

void TriangleSurface::scale(float dx, float dy, float dz)
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

    for(const auto &it : mColliders) {
        it->scale(dx, dy, dz);
    }
}
