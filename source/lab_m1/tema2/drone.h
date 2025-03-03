#pragma once

#ifndef __DRONE_H__
#define __DRONE_H__

#include "components/simple_scene.h"
#include "camera.h"

namespace m1
{
    class DroneClass
    {
    public:
        DroneClass();
        ~DroneClass();

        void Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes);

        void UpdateLocation(WindowObject* window, float deltaTime, glm::mat4 locationMatrix);
        void InputUpdate(WindowObject* window, float deltaTime);

        void OnKeyRelease(int key, int mods);

        static void LoadMeshes(std::unordered_map<std::string, Mesh*>& meshes,
            std::unordered_map<std::string, glm::vec3> mList);

    public:

        std::vector<std::pair<std::string, glm::mat4>> meshList;
        cam::Camera* orientation;
        float radius;

        bool collision;
        bool pCollision;
        bool hasP;

        glm::mat4 transformMatrix;
    private:

        float maxAcc = 80;
        float acc;
        float speedY;
        float speedX;
        float speedZ;
        float gravity;
        float bound;

        float maxAngle;
        float locY;
        float thrust;
        float pitch;
        float yaw;
        float roll;

        float rotateAngle;

        float bodySize;
        float propelerSize;

        glm::vec3 propelerColor = glm::vec3(0, 0, 0);
        glm::vec3 bodyColor = glm::vec3(0.9, 0.9, 0.8);
    };
}   // namespace m1

#endif