#include "lab_m1/tema2/ground.h"

using namespace std;
using namespace m1;

GroundClass::GroundClass() {}

GroundClass::~GroundClass() {}

void GroundClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, int size) {
   
    size = size;

    //init mesh
    //terrain vertex and index data
    {
        std::vector<VertexFormat>vertices;

        for (int i = -size; i <= size; i++) {
            for (int j = -size; j <= size; j++) {
                vertices.push_back(VertexFormat(glm::vec3((float) j, 0, (float) i), terrainColor));
            }
        }

        vector<unsigned int> indices;

        int numVerticesPerRow = 2 * size + 1;
        for (int i = 0; i < 2*size; i++) {
            for (int j = 0; j < 2*size; j++) {

                unsigned int topLeft = i * numVerticesPerRow + j;
                unsigned int topRight = i * numVerticesPerRow + j + 1;
                unsigned int bottomLeft = (i + 1) * numVerticesPerRow + j;
                unsigned int bottomRight = (i + 1) * numVerticesPerRow + j + 1;

                indices.push_back(topLeft);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);

                indices.push_back(topRight);
                indices.push_back(bottomRight);
                indices.push_back(bottomLeft);
            }
        }

        meshes["terrain"] = new Mesh("terrain");
        meshes["terrain"]->InitFromData(vertices, indices);
    }
}
