#include "arrow.h"
#include <math.h>

using namespace std;
using namespace m1;

ArrowClass::ArrowClass() {}

ArrowClass::~ArrowClass() {}

void ArrowClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location){

	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -0.5f, -1), arrowColor),
			VertexFormat(glm::vec3(1, -0.5f, -1), arrowColor),
			VertexFormat(glm::vec3(1, -0.5f, 1), arrowColor),
			VertexFormat(glm::vec3(-1, -0.5f, 1), arrowColor),

			VertexFormat(glm::vec3(0, 0.5f, 0), arrowColor),
		};

		vector<unsigned int> indices =
		{
			0, 1, 2,
			0, 2, 3,

			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4
		};


		meshes["arrow"] = new Mesh("arrow");
		meshes["arrow"]->InitFromData(vertices, indices);

	}

	orientation = new cam::Camera();
	orientation->Set(location, glm::vec3(location.x, location.y, location.z - 1), glm::vec3(0, 1, 0));

	bodySize = 2;
	UpdateLocation(0);
}

void ArrowClass::UpdateLocation( float deltaTime) {

	glm::mat4 locationMatrix = glm::mat4(1);

	locationMatrix[0] = glm::vec4(orientation->right, 0.0f);
	locationMatrix[1] = glm::vec4(orientation->up, 0.0f);
	locationMatrix[2] = glm::vec4(-orientation->forward, 0.0f);
	locationMatrix[3] = glm::vec4(orientation->position, 1.0f);


	aMatrix = locationMatrix;
	aMatrix = glm::rotate(aMatrix,-glm::pi<float>()/2 ,glm::vec3(1, 0, 0));
	aMatrix = glm::scale(aMatrix, glm::vec3(bodySize, bodySize*3, bodySize));
}

void ArrowClass::PointTo(glm::vec3 locToPoint) {
	glm::vec3 target = glm::vec3(locToPoint.x, orientation->position.y, locToPoint.z);
	orientation->Set(orientation->position, target , glm::vec3(0, 1, 0));
}