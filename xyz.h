#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "visualobject.h"
#include "gridsystem.h"

// Klassen skal brukes til å lage et origo referansepunkt
class XYZ : public VisualObject
{
private:

public:
   XYZ();
   ~XYZ() override;
   //void setContext(QOpenGLContext *context);
   void init(MyStructs::ShaderUniforms shaderUniforms) override;
   void draw() override;

   GridSystem *mGrid; // En referanse til komponenten som er de grå linjene rundt xyz referansen
};

#endif // XYZ_H

