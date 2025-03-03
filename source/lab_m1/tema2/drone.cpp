#include "drone.h"
#include "utils.h"
#include <math.h>

using namespace std;
using namespace m1;

DroneClass::DroneClass() {}

DroneClass::~DroneClass() {}

void DroneClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes) {

	// load meshes

	{
		vector<VertexFormat> vertices1
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

		vector<VertexFormat> vertices2
		{
			VertexFormat(glm::vec3(-0.5f, -0.5f, 0.5f), propelerColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, 0.5f), propelerColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, 0.5f), propelerColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, 0.5f), propelerColor),

			VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), propelerColor),
			VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), propelerColor),
			VertexFormat(glm::vec3(-0.5f, 0.5f, -0.5f), propelerColor),
			VertexFormat(glm::vec3(0.5f, 0.5f, -0.5f), propelerColor)
		};

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

		meshes["droneBody"] = new Mesh("droneBody");
		meshes["droneBody"]->InitFromData(vertices1, indices);


		meshes["dronePropeler"] = new Mesh("dronePropeler");
		meshes["dronePropeler"]->InitFromData(vertices2, indices);

	}

	orientation = new cam::Camera();
	orientation->Set(glm::vec3(0,0,0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	hasP = false;
	speedY = 0;
	speedX = 0;
	speedZ = 0;

	locY = 3;
	gravity = 5;
	bound = 0.5;


	maxAngle = glm::pi<float>() / 12;
	thrust = 0;
	pitch = 0;
	yaw = 0;
	roll = 0;

	rotateAngle = 0;
	bodySize = 0.09;
	radius = bodySize * 5;

	UpdateLocation(window, 0, glm::mat4(1));
}

void DroneClass::UpdateLocation(WindowObject* window, float deltaTime, glm::mat4 locationMatrix) {

	if (orientation->position.y > 50) {
		thrust = 0;
	}

	locY = deltaTime * speedY;
	speedY += (orientation->up.y * thrust - gravity) * deltaTime;

	float locX = deltaTime * speedX;
	speedX += orientation->up.x * thrust * deltaTime;

	float locZ = deltaTime * speedZ;
	speedZ += orientation->up.z * thrust * deltaTime;

	if (collision || pCollision) {
		speedX = -0.3*speedX;
		speedZ = -0.3*speedZ;
		locX = -locX;

		if (speedY < 0) {
			speedY = 0;
		}

		if (locY < 0) {
			locY = 0;
		}
		locZ = -locZ;
	}

	if (hasP) {
		bound = 2;
		radius = bodySize * 5 + 2;
	}
	else {
		bound = 0.5;
		radius = bodySize * 5;
	}

	if (orientation->position.y >= 0) {
		orientation->position += glm::vec3(locX, 0, locZ);
	}

	if (orientation->position.y >= 0 || locY > 0) {
		orientation->MoveUpward(locY);
	}

	if (orientation->position.y < bound) {
		if (speedY < 0) {
			speedY = 0;
		}
		speedX = 0;
		speedZ = 0;
		orientation->position.y = bound;
	}

	if (thrust != 0 ) {
		rotateAngle += deltaTime * 20;
	}
	if (rotateAngle > glm::pi<float>() * 2) {
		rotateAngle = 0;
	}

	locationMatrix = glm::mat4(1);

	locationMatrix[0] = glm::vec4(orientation->right, 0.0f);
	locationMatrix[1] = glm::vec4(orientation->up, 0.0f);
	locationMatrix[2] = glm::vec4(-orientation->forward, 0.0f);
	locationMatrix[3] = glm::vec4(orientation->position, 1.0f);
	transformMatrix = locationMatrix;

	glm::mat4 bodyMatrix1 = locationMatrix;
	bodyMatrix1 = glm::rotate(bodyMatrix1, glm::pi<float>()/4, glm::vec3(0,1,0));
	bodyMatrix1 = glm::scale(bodyMatrix1, glm::vec3(bodySize, bodySize, bodySize*10));

	glm::mat4 bodyMatrix2 = locationMatrix;
	bodyMatrix2 = glm::rotate(bodyMatrix2, -glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	bodyMatrix2 = glm::scale(bodyMatrix2, glm::vec3(bodySize, bodySize, bodySize * 10));

	glm::mat4 nubMatrix11 = locationMatrix;
	nubMatrix11 = glm::rotate(nubMatrix11, -glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	nubMatrix11 = glm::translate(nubMatrix11, glm::vec3(0, bodySize, bodySize*5 - bodySize/2));
	nubMatrix11 = glm::scale(nubMatrix11, glm::vec3(bodySize, bodySize, bodySize));

	glm::mat4 nubMatrix12 = locationMatrix;
	nubMatrix12 = glm::rotate(nubMatrix12, -glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	nubMatrix12 = glm::translate(nubMatrix12, glm::vec3(0, bodySize, -bodySize * 5 + bodySize / 2));
	nubMatrix12 = glm::scale(nubMatrix12, glm::vec3(bodySize, bodySize, bodySize));

	glm::mat4 nubMatrix21 = locationMatrix;
	nubMatrix21 = glm::rotate(nubMatrix21, glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	nubMatrix21 = glm::translate(nubMatrix21, glm::vec3(0, bodySize, bodySize * 5 - bodySize / 2));
	nubMatrix21 = glm::scale(nubMatrix21, glm::vec3(bodySize, bodySize, bodySize));

	glm::mat4 nubMatrix22 = locationMatrix;
	nubMatrix22 = glm::rotate(nubMatrix22, glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	nubMatrix22 = glm::translate(nubMatrix22, glm::vec3(0, bodySize, -bodySize * 5 + bodySize / 2));
	nubMatrix22 = glm::scale(nubMatrix22, glm::vec3(bodySize, bodySize, bodySize));

	glm::mat4 propMatrix1 = locationMatrix;
	propMatrix1 = glm::rotate(propMatrix1, glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	propMatrix1 = glm::translate(propMatrix1, glm::vec3(0, bodySize + bodySize/2 + bodySize / 8, -bodySize * 5 + bodySize / 2));
	propMatrix1 = glm::rotate(propMatrix1, rotateAngle, glm::vec3(0, 1, 0));
	propMatrix1 = glm::scale(propMatrix1, glm::vec3(bodySize/4, bodySize/4, bodySize*3));

	glm::mat4 propMatrix2 = locationMatrix;
	propMatrix2 = glm::rotate(propMatrix2, glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	propMatrix2 = glm::translate(propMatrix2, glm::vec3(0, bodySize + bodySize / 2 + bodySize / 8, bodySize * 5 - bodySize / 2));
	propMatrix2 = glm::rotate(propMatrix2, rotateAngle, glm::vec3(0, 1, 0));
	propMatrix2 = glm::scale(propMatrix2, glm::vec3(bodySize/4, bodySize / 4, bodySize * 3));

	glm::mat4 propMatrix3 = locationMatrix;
	propMatrix3 = glm::rotate(propMatrix3, -glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	propMatrix3 = glm::translate(propMatrix3, glm::vec3(0, bodySize + bodySize / 2 + bodySize / 8, -bodySize * 5 + bodySize / 2));
	propMatrix3 = glm::rotate(propMatrix3, rotateAngle, glm::vec3(0, 1, 0));
	propMatrix3 = glm::scale(propMatrix3, glm::vec3(bodySize/4, bodySize / 4, bodySize * 3));

	glm::mat4 propMatrix4 = locationMatrix;
	propMatrix4 = glm::rotate(propMatrix4, -glm::pi<float>() / 4, glm::vec3(0, 1, 0));
	propMatrix4 = glm::translate(propMatrix4, glm::vec3(0, bodySize + bodySize / 2 + bodySize / 8, bodySize * 5 - bodySize / 2));
	propMatrix4 = glm::rotate(propMatrix4, rotateAngle, glm::vec3(0, 1, 0));
	propMatrix4 = glm::scale(propMatrix4, glm::vec3(bodySize/4, bodySize / 4, bodySize * 3));

	meshList = {{"droneBody", bodyMatrix1},
				{"droneBody", bodyMatrix2},
				{"droneBody", nubMatrix11},
				{"droneBody", nubMatrix12},
				{"droneBody", nubMatrix21},
				{"droneBody", nubMatrix22},
				{"dronePropeler", propMatrix1},
				{"dronePropeler", propMatrix2},
				{"dronePropeler", propMatrix3},
				{"dronePropeler", propMatrix4}};
}

void DroneClass::InputUpdate(WindowObject* window, float deltaTime) {
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		//up thrust
		thrust = 10;
	}

	if (window->KeyHold(GLFW_KEY_W)) {
	
			orientation->RotateObj_OX(-deltaTime*2);
		
	}

	if (window->KeyHold(GLFW_KEY_S)) {
	
			orientation->RotateObj_OX(deltaTime * 2);

	}

	if (window->KeyHold(GLFW_KEY_D)) {
		
			orientation->RotateObj_OZ(deltaTime * 2);
		
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		
			orientation->RotateObj_OZ(-deltaTime * 2);

	}

	if (window->KeyHold(GLFW_KEY_Q)) {
	
		orientation->RotateObj_OY(deltaTime * 2);
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		orientation->RotateObj_OY(-deltaTime * 2);
	}

}

void DroneClass::OnKeyRelease(int key, int mods) {
	if (key == GLFW_KEY_SPACE) {
		//up thrust
		thrust = 0;
	}
}