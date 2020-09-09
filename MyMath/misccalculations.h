#ifndef MISCCALCULATIONS_H
#define MISCCALCULATIONS_H

#include <QVector3D>
#include <QVector2D>
#include "visualobject.h"
#include "terrain.h"

// Her kan alle kalkulasjoner som ikke nødvendigvis har en klasse hvor den hører hjemme plasseres
namespace MiscCalculations
{

    void getWorldTransform(QVector3D &p, MyStructs::Transformation objectTransform); // Kalkulerer ut en vertex sin WorldPosition
    bool cmpf(float A, float B, float epsilon = 0.005f); // Sikker sammenligning av to floats
    float randFloat(int min, int max); // Genererer en random float mellom to positive verdier
    float clamp(float value, float max, float min); // Passer på at en verdi ikke overstiger max og er under min

    float lerp(float a, float b, float step); // Lineær interpolasjon mellom to float verdier

    float degreesToRadians(float degrees); // Konverterer grader til radianer

    // Funksjonen min for å kalkulere barysentriske koordinater
    void BarycentricCoord(const QVector3D &point, QVector3D &p1, QVector3D &p2, QVector3D &p3,
                          const MyStructs::Transformation triangleTransform,
                          float &u, float &v, float &w);

    // Brukes etter at barysentriske koordinater er kalkulert, til å returnere en høyde som objektet kan ha
    float calcBarycHeight(QVector3D &p1, QVector3D &p2, QVector3D &p3, const MyStructs::Transformation triangleTransform, float u, float v, float w);
    bool ValidatePoint2D(float u, float v, float w);

    MyStructs::Triangles *searchForNextTriangle(Terrain *ground, VisualObject *object, float &height);
};

#endif // MISCCALCULATIONS_H
