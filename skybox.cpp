#include "skybox.h"

Skybox::Skybox()
{
    mVertices.push_back(Vertex{ 0.5,  0.5,  0.5,      1.f, 0.f, 0.f,    1.0f, 1.0f}); // top right front
    mVertices.push_back(Vertex{ 0.5, -0.5,  0.5,      0.f, 1.f, 0.f,    1.0f, 0.0f}); // lower right front
    mVertices.push_back(Vertex{-0.5,  0.5,  0.5,      0.f, 0.f, 1.f,    0.0f, 1.0f}); // top left front
    mVertices.push_back(Vertex{-0.5, -0.5,  0.5,      1.f, 1.f, 1.f,    0.0f, 0.0f}); // lower left front

    mVertices.push_back(Vertex{ 0.5,  0.5, -0.5,      1.f, 0.f, 0.f,    3.0f, 3.0f}); // top right back
    mVertices.push_back(Vertex{ 0.5, -0.5, -0.5,      0.f, 1.f, 0.f,    3.0f, 2.0f}); // lower right back
    mVertices.push_back(Vertex{-0.5,  0.5, -0.5,      0.f, 0.f, 1.f,    2.0f, 3.0f}); // top left back
    mVertices.push_back(Vertex{-0.5, -0.5, -0.5,      1.f, 1.f, 1.f,    2.0f, 2.0f}); // lower left back



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
    mTransformation.scale = QVector4D(1000, 1000, 1000, 1);
}

Skybox::Skybox(std::vector<std::string> skyboxFaces)
{
    mVertices.push_back(Vertex{ 1,  1,  1,      1.f, 0.f, 0.f,    1.0f, 1.0f}); // top right front
    mVertices.push_back(Vertex{ 1, -1,  1,      0.f, 1.f, 0.f,    1.0f, 0.0f}); // lower right front
    mVertices.push_back(Vertex{-1,  1,  1,      0.f, 0.f, 1.f,    0.0f, 1.0f}); // top left front
    mVertices.push_back(Vertex{-1, -1,  1,      1.f, 1.f, 1.f,    0.0f, 0.0f}); // lower left front

    mVertices.push_back(Vertex{ 1,  1, -1,      1.f, 0.f, 0.f,    1.0f, 1.0f}); // top right back
    mVertices.push_back(Vertex{ 1, -1, -1,      0.f, 1.f, 0.f,    1.0f, 0.0f}); // lower right back
    mVertices.push_back(Vertex{-1,  1, -1,      0.f, 0.f, 1.f,    0.0f, 1.0f}); // top left back
    mVertices.push_back(Vertex{-1, -1, -1,      1.f, 1.f, 1.f,    0.0f, 0.0f}); // lower left back



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

    mTransformation.scale = QVector4D(1000, 1000, 1000, 1);

    mMatrix.setToIdentity();

    for(unsigned int i = 0; i < skyboxFaces.size(); ++i) {
        mTexture.push_back(MyStructs::Texture());
        mTexture.back().path = skyboxFaces[i];
    }

    mLabel = "Skybox";
}

void Skybox::init(MyStructs::ShaderUniforms shaderUniforms)
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


    // enable the matrixUniform
    mShaderUniforms.mObjectSizeUniform = shaderUniforms.mObjectSizeUniform;
    if(!mTexture.empty()) {
        mShaderUniforms.mTextureUniform = shaderUniforms.mTextureUniform;
    }

    glBindVertexArray(0);
}

void Skybox::draw()
{
    if(mVisible) {
        // håndterer textures
        // Dersom det ikke finnes noen texture, så brukes mObjectColor som basis for fargen til objektet
        // Måtte også lage en egen Texture variabel for å håndtere dette. Den bruker textureID her
        if(!mTexture.empty()) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture.at(0).textureID); // Krasjer her
            glUniform1i(mShaderUniforms.mTextureUniform, 0);
        }
        glUniform3f(mShaderUniforms.mObjectSizeUniform, mTransformation.scale.x(), mTransformation.scale.y(), mTransformation.scale.z());

        // Binder vertex
        glBindVertexArray( mVAO );

        // Tegner elementene i mVertices ved å se på koblingene satt i mIndices
        // If testen sjekker om en skal tegne kun linjene eller faktisk facene
        if(!mDrawDebugLines) {
            glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        } else {
            glDrawElements(GL_LINE_LOOP, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, nullptr);
        }

    }
}

void Skybox::generateTexture()
{

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    mTexture.at(0).textureID = textureID;

    // load and generate the texture
    int width, height, nrChannels;

    unsigned char *data = nullptr;

    for(unsigned int i = 0; i < mTexture.size(); ++i){
        stbi_set_flip_vertically_on_load(false);
        data = stbi_load(mTexture.at(i).path.c_str(), &width, &height, &nrChannels, 0);

        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            qDebug() << "Failed to load texture";
            stbi_image_free(data);
        }
    }

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}
