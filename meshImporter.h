#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include <iostream>
#include "vertex.h"
#include <random>
#include <vector>
#include <string.h>
#include <ctime>

namespace MeshImport {

    // Tolker typen som leses inn
    // Støtter for tiden .txt og .obj filer
    std::string typeInterpreter(const char *path);

    // Leser inn .obj filer og lager et objekt ut fra datasettet
    bool loadObj(const char *path, std::vector<Vertex> &mVertices, std::vector<unsigned int> &mIndices);

    // Leser inn .txt filer og lager et objekt ut fra datasettet
    void readFile(const char *path, std::vector<Vertex> &mVertices, std::vector<unsigned int> &mIndices);

}


#endif // MESHIMPORTER_H
