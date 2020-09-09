#include "terrain.h"

Terrain::Terrain() : TriangleSurface()
{

}

Terrain::Terrain(const char *path, const char *texture) : TriangleSurface(path, texture)
{
    if(MeshImport::typeInterpreter(path) == "png") {
        generateTerrain(path);
    }
}

void Terrain::getHeight(int height, unsigned char *data, std::vector<float> &terrainHeight, int channelCount, int width)
{
    for(int i = 0; i < height * width * channelCount; ++i) {
        float r = static_cast<float>(data[i]);
        i += 3; //Hopper over channels jeg ikke trenger her. Bruker kun r siden r, g, og b er helt like

        float height = (r * mHeightMultiplier) + mHeightAdjuster;
        terrainHeight.push_back(height);
    }
}

void Terrain::populateTriangles(int p4Indice, int i, int p3Indice, int height, int p1Indice, int width, int p2Indice, int j)
{
    MyStructs::Triangles t1;
    t1.indices[0] = p1Indice;
    t1.indices[1] = p2Indice;
    t1.indices[2] = p3Indice;
    t1.id = static_cast<int>(mTriangles.size());

    MyStructs::Triangles t2;
    t2.indices[0] = p1Indice;
    t2.indices[1] = p4Indice;
    t2.indices[2] = p3Indice;
    t2.id = static_cast<int>(mTriangles.size()+1);

    t1.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()) + 1);
    t2.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()));


    if(i > 0) {
        t1.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()) - ((width-1)*2) + 1);
    }

    if(i + width < (height * width) - width) {
        t2.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()) + ((width-1)*2));
    }

    if(j > 0) {
        t1.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()) - 1);
    }
    if(j+1 < width-1) {
        t2.neighborTrianglesIndices.push_back(static_cast<int>(mTriangles.size()) + 2);
    }

    mTriangles.push_back(t1);
    mTriangles.push_back(t2);
}

void Terrain::generateTerrain(const char *path)
{
    int width, height, nrChannels;
    int channelCount = 4;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, channelCount);

    std::vector<float> terrainHeight;
    getHeight(height, data, terrainHeight, channelCount, width);

    int terrainHeightIndex = 0;
    for(float i = 0; i < height; ++i) {
        for(float j = 0; j < width; ++j) {
            mVertices.push_back(Vertex{i - (height/2),terrainHeight[terrainHeightIndex],j - (width/2), 0, 0, 0, i/mTextureDetail, j/mTextureDetail});
            ++terrainHeightIndex;
        }
    }

    for(int i = 0; i < (height * width) - width; i += width) {
        for(int j = 0; j < width - 1; ++j) {
            int p1Indice = i + j + width;
            int p2Indice = i + j;
            int p3Indice = i + j + 1;

            int p4Indice = i + j + width + 1;

            mIndices.push_back(p1Indice);
            mIndices.push_back(p2Indice);
            mIndices.push_back(p3Indice);

            mIndices.push_back(p1Indice);
            mIndices.push_back(p4Indice);
            mIndices.push_back(p3Indice);

            populateTriangles(p4Indice, i, p3Indice, height, p1Indice, width, p2Indice, j);

        }
    }
    calcNormals();

}

void Terrain::makePlaneOfTwoVariables(float xLength, float zLength)
{

    float x = 0;
    while(x < xLength) {
        float z = 0;
        while(z < zLength) {
            //TODO: Push inn fire vertexer i mVertices
            float y = sinf(x)/4 + sinf(z)/4;
            mVertices.push_back(Vertex{x,y,z, 1, y*2, 0});

            x += 0.1f;
            y = sinf(x)/4 + sinf(z)/4;
            mVertices.push_back(Vertex{x,y,z, 1, y*2, 0});

            if(mVertices.size() >= 4) {
                mIndices.emplace_back(mVertices.size()-4);
                mIndices.emplace_back(mVertices.size()-3);
                mIndices.emplace_back(mVertices.size()-2);
                mIndices.emplace_back(mVertices.size()-3);
                mIndices.emplace_back(mVertices.size()-2);
                mIndices.emplace_back(mVertices.size()-1);
            }
            z += 0.1f;
            x -= 0.1f;

        }
        x += 0.1f;
    }
    scale(1, 1, 1);
    move(-xLength/2, 0, -zLength/2);

}
