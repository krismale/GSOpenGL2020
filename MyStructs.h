#ifndef MYSTRUCTS_H
#define MYSTRUCTS_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector4D>
#include "vertex.h"

namespace MyStructs {

    // Struct for å organisere nødvendige shader uniforms
    // Det er ikke alle shaderne som bruker alle disse, men jeg har valgt å ha en felles struct for
    // å holde det ryddig
    struct ShaderUniforms {
        GLint mPMatrixUniform;
        GLint mVMatrixUniform;

        GLint mMatrixUniform;
        GLint mTextureUniform;
        GLint mObjectColorUniform;
        GLint mSpecularStrengthUniform;
        GLint mSpecularExponentUniform;

        GLint mLightPosUniform;
        GLint mLightStrengthUniform;
        GLint mLightPowerUniform;

        GLint mAmbientStrengthUniform;
        GLint mAmbientColUniform;

        GLint mObjectCenterUniform;
        GLint mCameraCenterUniform;
        GLint mObjectSizeUniform;
        GLint mUiElementUniform;
    };

    // Dette er en struct som holder på bools som skrur av og på bevege med kamera og pawn
    struct PlayerInput {
        // Pawn movement
        bool moveForward    = false;
        bool moveBack       = false;
        bool moveLeft       = false;
        bool moveRight      = false;

        // Camera movement
        bool moveCamForward = false;
        bool moveCamBack    = false;
        bool moveCamLeft    = false;
        bool moveCamRight   = false;
        bool camRotate      = false;
    };

    // Transformasjonsstructen som brukes for alle objekter
    struct Transformation {
        QVector4D translation;
        QVector4D rotation;
        QVector4D scale;
    };

    // En struct som samler informasjon om triangles
    // Denne kunne ha blitt brukt i alle objekter, men brukes egentlig kun til Terrain
    // Med denne structen blir det mye lettere å utføre finne riktig triangel for å kjøre barysentriske koordinater på
    struct Triangles {
        unsigned int indices[3];
        std::vector<unsigned int> neighborTrianglesIndices;
        int id;
    };

    struct Texture {
        unsigned int textureID;
        std::string path;
    };

}

#endif // MYSTRUCTS_H
