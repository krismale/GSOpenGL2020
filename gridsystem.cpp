#include "gridsystem.h"

GridSystem::GridSystem(int grids)
    : mNumberOfGrids(grids), mOuterGridPosition(0), mGridColor(0.5, 0.5, 0.5)
{
    float startPosition = -1.f;
    float endPosition = 1.f;

    for(int i = 0; i < mNumberOfGrids; i++) {
        int prefix = 1;
        if(mOuterGridPosition == 0.f) {
            endPosition = 0.f;
        } else {
            endPosition = 1.f;
        }
        for(int j = 0; j < 2; j++) {
            // along x-axis
            Vertex xVertexFrom;
            xVertexFrom.SetXYZ(startPosition, 0, prefix * mOuterGridPosition);
            xVertexFrom.SetRGB(mGridColor);
            Vertex xVertexTo;
            xVertexTo.SetXYZ(endPosition, 0, prefix * mOuterGridPosition);
            xVertexTo.SetRGB(mGridColor);
            mVertices.push_back(xVertexFrom);
            mVertices.push_back(xVertexTo);

            // along z-axis
            Vertex zVertexFrom;
            zVertexFrom.SetXYZ(prefix * mOuterGridPosition, 0, startPosition);
            zVertexFrom.SetRGB(mGridColor);
            Vertex zVertexTo;
            zVertexTo.SetXYZ(prefix * mOuterGridPosition, 0, endPosition);
            zVertexTo.SetRGB(mGridColor);
            mVertices.push_back(zVertexFrom);
            mVertices.push_back(zVertexTo);

            prefix = -1;
        }
        mOuterGridPosition += 0.1f;
    }

    mMatrix.setToIdentity(); // Til QMatrix
    //mMatrix.MakeIdentityMatrix(); // Til min egen Matrix4x4 klasse
}

GridSystem::~GridSystem() {

}

void GridSystem::init(MyStructs::ShaderUniforms shaderUniforms)
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
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;

    glBindVertexArray(0);
}

void GridSystem::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, static_cast<int>(mVertices.size()));
}
