#ifndef GRIDSYSTEM_H
#define GRIDSYSTEM_H

#include "visualobject.h"

class GridSystem : public VisualObject
{
public:
    GridSystem(int grids);
    ~GridSystem() override;
    void init(MyStructs::ShaderUniforms shaderUniforms) override;
    void draw() override;

    std::vector<Vertex*> mGridLines; // Blir det samme som mVertices

    int mNumberOfGrids; // Hvor mange gridlinjer som skal tegnes
    float mOuterGridPosition; // Hvor langt griden strekker seg i begge dimensjoner (x og z)
    QVector3D mGridColor; // Fargen på strekene

    // Brukes for å sette IDen til objektet i outlineren
    void setID(int id) override {
        mID = id;
    }
};

#endif // GRIDSYSTEM_H
