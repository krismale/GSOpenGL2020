#include "xyz.h"
#include "vertex.h"

XYZ::XYZ()
{
    // x-axis
    mVertices.push_back(Vertex{0,0,0, 1,0,0});
    mVertices.push_back(Vertex{1,0,0, 1,0,0});
    // y-axis
    mVertices.push_back(Vertex{0,0,0, 0,1,0});
    mVertices.push_back(Vertex{0,1,0, 0,1,0});
    // z-axis
    mVertices.push_back(Vertex{0,0,0, 0,0,1});
    mVertices.push_back(Vertex{0,0,1, 0,0,1});

    mGrid = new GridSystem(10);

    mMatrix.setToIdentity(); // Til QMatrix
    //mMatrix.MakeIdentityMatrix(); // Til min egen Matrix4x4 klasse
}

XYZ::~XYZ()
{

}

void XYZ::init(MyStructs::ShaderUniforms shaderUniforms)
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;

    mGrid->init(shaderUniforms);
    glBindVertexArray(0);
}

void XYZ::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    if(mVisible) {
        glDrawArrays(GL_LINES, 0, static_cast<int>(mVertices.size()));
        mGrid->draw();
    }
}

