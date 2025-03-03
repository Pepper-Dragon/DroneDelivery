#include "rock.h"
#include <math.h>

using namespace std;
using namespace m1;

RockClass::RockClass() {}

RockClass::~RockClass() {}

void RockClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::mat4 locationMatrix) {

	// load meshes
	{
		vector<VertexFormat> v1
		{
			VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f), bodyColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), bodyColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), bodyColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), bodyColor),

			VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), bodyColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), bodyColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), bodyColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), bodyColor)
		};

		vertices1 = v1;

		vector<unsigned int> indices1 =
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

		vector<VertexFormat> v2
		{
			VertexFormat(glm::vec3(-1, -0.5f, -1), bodyColor),
			VertexFormat(glm::vec3(1, -0.5f, -1), bodyColor),
			VertexFormat(glm::vec3(1, -0.5f, 1), bodyColor),
			VertexFormat(glm::vec3(-1, -0.5f, 1), bodyColor),

			VertexFormat(glm::vec3(0, 0.5f, 0), bodyColor),
		};

		vertices2 = v2;

		vector<unsigned int> indices2 =
		{
			0, 1, 2,
			0, 2, 3,

			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4
		};


		meshes["rockBody"] = new Mesh("rockBody");
		meshes["rockBody"]->InitFromData(vertices1, indices1);

		meshes["rockTop"] = new Mesh("rockTop");
		meshes["rockTop"]->InitFromData(vertices2, indices2);
	}

	bodySize = (rand() % 3 + 1)+2;
	UpdateLocation(window, 0, locationMatrix);
}

void RockClass::UpdateLocation(WindowObject* window, float deltaTime, glm::mat4 locationMatrix) {

	glm::mat4 bodyMatrix = locationMatrix;
	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(0, bodySize*3, 0));
	bodyMatrix = glm::scale(bodyMatrix, glm::vec3(bodySize*2,bodySize*6,bodySize*2));


	glm::mat4 topMatrix = locationMatrix;
	topMatrix = glm::translate(topMatrix, glm::vec3(0, bodySize*7, 0));
	topMatrix = glm::scale(topMatrix, glm::vec3(bodySize, bodySize*2, bodySize));


	bodyBox->setAABB(vertices1, bodyMatrix);
	topBox->setAABB(vertices2, topMatrix);

	meshList = { {"rockBody", bodyMatrix},
				{"rockTop", topMatrix} };
}