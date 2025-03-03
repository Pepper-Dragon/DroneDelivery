#pragma once

#ifndef __CHECK_H__
#define __CHECK_H__

#include "components/simple_scene.h"
#include "camera.h"

namespace m1
{
    class CheckClass
    {
    public:
        CheckClass();
        ~CheckClass();

        void Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location);

        void UpdateLocation(WindowObject* window, float deltaTime);

    public:

        std::vector<VertexFormat> vertices;
        cam::Camera* orientation;
        glm::mat4 cMatrix;
        float bodySize;

    private:
        glm::vec3 checkColor = glm::vec3(0.8, 0.8, 0);
    };
}   // namespace m1

#endif