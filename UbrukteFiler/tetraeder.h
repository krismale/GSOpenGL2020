#ifndef TETRAEDER_H
#define TETRAEDER_H

#include "visualobject.h"

class Tetraeder : public VisualObject
{
public:
    Tetraeder();

    void init(GLint matrixUniform) override;
    void draw() override;
    void createTextFile();
};

#endif // TETRAEDER_H
