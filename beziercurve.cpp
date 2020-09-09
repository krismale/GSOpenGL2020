#include "beziercurve.h"
#include "meshImporter.h"

BezierCurve::BezierCurve(std::vector<TriangleSurface*> controlPoints)
{
    b[0] = controlPoints[0]->mTransformation.translation.toVector3D();
    b[1] = controlPoints[1]->mTransformation.translation.toVector3D();
    b[2] = controlPoints[2]->mTransformation.translation.toVector3D();
    b[3] = controlPoints[3]->mTransformation.translation.toVector3D();

    for(int i = 0; i < 4; ++i) {
        mVertices.emplace_back(Vertex{b[i].x(), b[i].y(), b[i].z(),
                                     mControlColor.x(), mControlColor.y(), mControlColor.z()});
    }

    float t = 0;
    while(t < 1) {
        QVector3D v = evaluateBezier(3, t);
        mCurvePoints.emplace_back(v);
        mVertices.emplace_back(Vertex{v.x(), v.y(), v.z(),
                                     mCurveColor.x(), mCurveColor.y(), mCurveColor.z()});
        t += 0.001f;
    }

    mMatrix.setToIdentity();
    mVisible = true;

}

void BezierCurve::init(MyStructs::ShaderUniforms shaderUniforms)
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

    // enable the matrixUniform
    mShaderUniforms.mMatrixUniform = shaderUniforms.mMatrixUniform;

    glBindVertexArray(0);
}

QVector3D BezierCurve::evaluateBezier(int degree, float t)
{
    QVector3D c[4];
    for(int i = 0; i < 4; i++) {
        c[i] = b[i];
    }

    for(int k = 1; k <= degree; k++) {
        for(int i = 0; i < degree-k+1; i++) {
            c[i] = c[i] * (1-t) + c[i+1] * t;
        }
    }
    return c[0];
}

void BezierCurve::draw() {
    glBindVertexArray( mVAO );
    glUniformMatrix4fv(mShaderUniforms.mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    if(mVisible) {
        glDrawArrays(GL_LINES, 0, static_cast<int>(mVertices.size()));
    }

}
