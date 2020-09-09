#include "tetraeder.h"

Tetraeder::Tetraeder()
{
    mVertices.push_back(Vertex{ 0.5, 0.0, -0.5,   0.0, 0.0, 0.5}); // Triangle 1, lower right
    mVertices.push_back(Vertex{-0.5, 0.0, -0.5,   0.0, 0.0, 0.5}); // Triangle 1, lower left
    mVertices.push_back(Vertex{ 0.0, 0.5,  0.0,   0.0, 0.0, 0.5}); // Triangle 1, top

    // Samme som triangle 1, lower left
    mVertices.push_back(Vertex{-0.5, 0.0, -0.5,   0.5, 0.0, 0.0}); // Triangle 2, lower right
    // Samme som triangle 3, lower right
    mVertices.push_back(Vertex{ 0.0, 0.0,  0.5,   0.5, 0.0, 0.0}); // Triangle 2, lower left
    mVertices.push_back(Vertex{ 0.0, 0.5,  0.0,   0.5, 0.0, 0.0}); // Triangle 2, top

    // Samme som triangle 1, lower right
    mVertices.push_back(Vertex{ 0.5, 0.0, -0.5,   0.0, 0.5, 0.0}); // Triangle 3, lower left
    mVertices.push_back(Vertex{ 0.0, 0.0,  0.5,   0.0, 0.5, 0.0}); // Triangle 3, lower right
    mVertices.push_back(Vertex{ 0.0, 0.5,  0.0,   0.0, 0.5, 0.0}); // Triangle 3, top


    createTextFile();

    //mMatrix.setToIdentity(); // Til QMatrix
    mMatrix.MakeIdentityMatrix(); // Til min egen Matrix4x4 klasse
}

void Tetraeder::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

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

    // enable the matrixUniform
    //mMatrixUniform = glGetUniformLocation(matrixUniform, "matrix");

    glBindVertexArray(0);
}

void Tetraeder::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    //just to make the triangle rotate:
    //             degree, x,   y,   z -axis
    //mMatrix.rotate(2.f, 0.f, 1.0, 0.f); // Til QMatrix
    //mMatrix.Rotate(2.f, 1.f, 0.f, 0.f); // Til min egen Matrix4x4 klasse
}

void Tetraeder::createTextFile()
{
    std::ofstream outFile("tetraeder.txt", std::ofstream::out);
    outFile << mVertices.size() << std::endl;
    for(auto it : mVertices) {
        outFile << it << std::endl;
    }
}
