#include "package.h"
#include <math.h>

using namespace std;
using namespace m1;

PackageClass::PackageClass() {}

PackageClass::~PackageClass() {}

void PackageClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location) {

	// load meshes
	{
		vector<VertexFormat> v
		{
			VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f), boxColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), boxColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), boxColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), boxColor),

			VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), boxColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), boxColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), boxColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f),  boxColor)
		};

		vertices = v;

		vector<unsigned int> indices =
		{
			0, 1, 2,
			1, 3, 2,

			2, 3, 7,
			2, 7, 6,

			1, 7, 3,
			1, 5, 7,

			6, 7, 4,
			7, 5, 4,

			0, 4, 1,
			1, 4, 5,

			2, 6, 4,
			0, 2, 4
		};


		meshes["package"] = new Mesh("package");
		meshes["package"]->InitFromData(vertices, indices);
	}

	orientation = new cam::Camera();
	orientation->Set(location, glm::vec3(location.x, location.y, location.z-1), glm::vec3(0, 1, 0));

	bodySize = 2;
	UpdateLocation(window, 0);
}

void PackageClass::UpdateLocation(WindowObject* window, float deltaTime) {

	glm::mat4 locationMatrix = glm::mat4(1);

	locationMatrix[0] = glm::vec4(orientation->right, 0.0f);
	locationMatrix[1] = glm::vec4(orientation->up, 0.0f);
	locationMatrix[2] = glm::vec4(-orientation->forward, 0.0f);
	locationMatrix[3] = glm::vec4(orientation->position, 1.0f);


	pMatrix = locationMatrix;
	pMatrix = glm::scale(pMatrix, glm::vec3(bodySize, bodySize, bodySize));

	box->setAABB(vertices, pMatrix);
}