#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "visualobject.h"
#include "trianglesurface.h"


class BezierCurve : public VisualObject
{
public:
    BezierCurve(std::vector<TriangleSurface*> controlPoints);

    void draw() override;
    void init(MyStructs::ShaderUniforms shaderUniforms) override;

    QVector3D evaluateBezier(int degree, float t);

    QVector3D b[4];
    std::vector<QVector3D> mCurvePoints;

    QVector3D mControlColor = QVector3D(0, 1, 0);
    QVector3D mCurveColor = QVector3D(1, 0, 0);

    void setID(int id) override {
        mID = id;
    }
};

#endif // BEZIERCURVE_H
