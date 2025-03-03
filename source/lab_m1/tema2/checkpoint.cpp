#include "checkpoint.h"
#include <math.h>

using namespace std;
using namespace m1;

CheckClass::CheckClass() {}

CheckClass::~CheckClass() {}

void CheckClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location) {

	// load meshes
	{   
        std::vector<unsigned int> indices;
		int seg_size = 40;

        vertices.push_back(VertexFormat(glm::vec3(0.0f, 0.5f, 0.0f), checkColor));
        vertices.push_back(VertexFormat(glm::vec3(0.0f, -0.5f, 0.0f), checkColor));

		for (float i = 0; i <= 360; i += seg_size) {
			float x = glm::cos(glm::pi<float>() * 2 * i / 360);
			float z = glm::sin(glm::pi<float>() * 2 * i / 360);
			vertices.push_back(VertexFormat(glm::vec3(x, 0.5f, z), checkColor));
		}

		for (unsigned int i = 2; i < vertices.size()-1; i++) {
			indices.insert(indices.end(), { 0, i, i + 1 });
		}

		for (unsigned int i = 2; i < vertices.size()-1; i++) {
			indices.insert(indices.end(), { 1, i, i + 1 });
		}

		meshes["checkpoint"] = new Mesh("checkpoint");
		meshes["checkpoint"]->InitFromData(vertices, indices);
	}

	orientation = new cam::Camera();
	orientation->Set(location, glm::vec3(location.x, location.y, location.z - 1), glm::vec3(0, 1, 0));

	bodySize = 5;
	UpdateLocation(window, 0);
}

void CheckClass::UpdateLocation(WindowObject* window, float deltaTime) {

	glm::mat4 locationMatrix = glm::mat4(1);

	locationMatrix[0] = glm::vec4(orientation->right, 0.0f);
	locationMatrix[1] = glm::vec4(orientation->up, 0.0f);
	locationMatrix[2] = glm::vec4(-orientation->forward, 0.0f);
	locationMatrix[3] = glm::vec4(orientation->position, 1.0f);


	cMatrix = locationMatrix;
	cMatrix = glm::scale(cMatrix, glm::vec3(bodySize, 1, bodySize));

}