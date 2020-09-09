#include "meshImporter.h"
#include "MyMath/misccalculations.h"
#include <fstream>

// Hjelpefunksjon for loadObj. Brukes til å duplisere vertekser med samme posisjon
// for å gi de unike normaler
void duplicatePopulateVertices(unsigned int normalIndex[], unsigned int uvIndex[], std::vector<Vertex> &mVertices, std::vector<Vertex> tempST, std::vector<Vertex> tempXYZ, std::vector<unsigned int> &mIndices, std::vector<Vertex> tempNormal, unsigned int vertexIndex[])
{
    for(int i = 0; i < 3; ++i) {
        Vertex vertex;
        vertex.SetXYZ(tempXYZ.at(vertexIndex[i]-1).GetX(),
                      tempXYZ.at(vertexIndex[i]-1).GetY(),
                      tempXYZ.at(vertexIndex[i]-1).GetZ());
        vertex.SetRGB(tempNormal.at(normalIndex[i]-1).GetR(),
                      tempNormal.at(normalIndex[i]-1).GetG(),
                      tempNormal.at(normalIndex[i]-1).GetB());
        vertex.SetST(tempST.at(uvIndex[i]-1).GetU(), tempST.at(uvIndex[i]-1).GetV());
        mVertices.push_back(vertex);
        mIndices.push_back(static_cast<int>(mVertices.size()-1));
    }
}

bool MeshImport::loadObj(const char *path, std::vector<Vertex> &mVertices, std::vector<unsigned int> &mIndices)
{
    FILE* file = fopen(path, "r");
    if(file == NULL) {
        printf("Impossible to open the file!\n");
        return false;
    }

    std::vector<Vertex> tempST;
    std::vector<Vertex> tempNormal;
    std::vector<Vertex> tempXYZ;
    while(1) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if(res == EOF) {
            break; // EOF = End Of FIle. Quit loop
        }

        if(strcmp(lineHeader, "v") == 0) {
            Vertex vertex;
            fscanf(file, "%f %f %f\n", &vertex.m_xyz[0], &vertex.m_xyz[1], &vertex.m_xyz[2]);
            tempXYZ.push_back(vertex);

            // Do something with textures
        } else if(strcmp(lineHeader, "vt") == 0) {
            Vertex uv;
            fscanf(file, "%f %f\n", &uv.m_st[0], &uv.m_st[1]);
            tempST.push_back(uv);

            // Do something with normals
        } else if(strcmp(lineHeader, "vn") == 0) {
            Vertex normal;
            fscanf(file, "%f %f %f\n", &normal.m_normal[0], &normal.m_normal[1], &normal.m_normal[2]);
            tempNormal.push_back(normal);

            // Do something with indices
        } else if(strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file,
                                 "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            // sjekker om det er definert flere koblinger enn tre (XYZ, UV og Normal koordinater),
            // eller at en ikke prøver å tegne trekanter, men heller oktahedron for eksempel
            if(matches != 9) {
                printf("File can't be read by our simple parser : (Try exporting with other options\n");
                return false;
            }

            duplicatePopulateVertices(normalIndex, uvIndex, mVertices, tempST, tempXYZ, mIndices, tempNormal, vertexIndex);
        }
    }
    return true;
}

std::string MeshImport::typeInterpreter(const char *path)
{
    const char* p = path;
    std::string type;
    while(*p != '\0') {
        if(*p == '.') {
            for(int i = 0; i < 3; i++) {
                ++p;
                if(*p == '.' || *p == '/') {
                    i = 3;
                } else {
                    type.push_back(*p);
                }

            }
        }
        ++p;
    }
    return type;
}

// Jeg har gjort noen endringer på readFile funksjonen, slik at den passer til min arkitektur
// Funksjonen må lese en indices vector, som skal koble sammen vektorene og tegne trekanter basert på de
// Da må også txt-fila ha et sett med tall som representerer indekser i vektoren vertices
// Disse må skrives i txt fila etter at posisjonene er satt
// Under tallet som oppgir hvor mange vertekser som finnes må det også skrives inn antall indeksposisjoner
// Eks format:
// 4
// 6
// (-2.000000, 0.000000, -8.000000) (0.000000, 0.500000, 0.000000) (0.000000, 0.000000)
// (2.000000, 0.000000, -8.000000) (0.000000, 0.500000, 0.000000) (0.000000, 0.000000)
// (2.000000, 0.000000, 2.000000) (0.000000, 0.700000, 0.000000) (0.000000, 0.000000)
// (-2.000000, 0.000000, 2.000000) (0.000000, 0.700000, 0.000000) (0.000000, 0.000000)
// 0
// 1
// 2
// 0
// 2
// 3
void MeshImport::readFile(const char *path, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
{
    const char* p = path;
    std::string stdStringPath;
    while(*p != '\0') {
        stdStringPath.push_back(*p);
        ++p;
    }

    std::ifstream inn;
       inn.open(stdStringPath.c_str());

       if (inn.is_open()) {
           int numberOfVertices;
           int numberOfIndices;
           Vertex vertex;
           inn >> numberOfVertices;
           inn >> numberOfIndices;
           vertices.reserve(numberOfVertices);
           for (int i=0; i<numberOfVertices; i++) {
                inn >> vertex;
                vertices.push_back(vertex);
           }
           unsigned int index;
           for(int i=0;i<numberOfIndices; i++) {
               inn >> index;
               indices.push_back(index);
           }
           inn.close();
       }
}
