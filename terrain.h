#ifndef TERRAIN_H
#define TERRAIN_H

#include "trianglesurface.h"
#include "meshImporter.h"

class Terrain : public TriangleSurface
{
public:
    Terrain();
    Terrain(const char *path, const char *texture);

    // Genererer terrenget
    void generateTerrain(const char *path);

    // Kalkulerer høyden til terrenget på et spesifikt punkt i xz-planet
    void getHeight(int height, unsigned char *data, std::vector<float> &terrainHeight, int channelCount, int width);

    // Populerer trianglene og finner alle naboene dens
    void populateTriangles(int p4Indice, int i, int p3Indice, int height, int p1Indice, int width, int p2Indice, int j);

    // Lager et mindre terreng av en enkel funksjon av to variabler
    // Brukes ikke i prosjektet, men er relevant for et terrengobjekt
    void makePlaneOfTwoVariables(float xLength, float zLength);

    // Listen over alle trianglene som terrenget består av
    // Brukes for å utføre et søk som minner om topologisk søk for barysentriske koordinater
    std::vector<MyStructs::Triangles> mTriangles;

private:
    // Variabler for å kontrollere detaljer på tekstur og høyden på terrenget
    const float mHeightMultiplier = 0.04f; // Viser til hvor mye fargene skal ha å si for å definere høyden
    const float mHeightAdjuster = -mHeightMultiplier*100; // pusher ned den generelle høyden til objektet, for å ligge cirka på origo
    const float mTextureDetail = 20; // Hvor detaljert teksturene er



};

#endif // TERRAIN_H
