#pragma once

#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "components/simple_scene.h"
#include "AABB.h"
#include "camera.h"

namespace m1
{
    class PackageClass
    {
    public:
        PackageClass();
        ~PackageClass();

        void Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location);

        void UpdateLocation(WindowObject* window, float deltaTime);

    public:

        std::vector<VertexFormat> vertices;
        glm::mat4 pMatrix;
        cam::Camera* orientation;

        BoxClass* box = new BoxClass();
    private:

        float bodySize;

        glm::vec3 boxColor = glm::vec3(0.5, 0.5, 0.5);
    };
}   // namespace m1

#endif