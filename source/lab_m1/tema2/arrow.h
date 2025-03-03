#pragma once

#ifndef __ARROW_H__
#define __ARROW_H__

#include "components/simple_scene.h"
#include "camera.h"
#include <math.h>

namespace m1
{
    class ArrowClass
    {
    public:
        ArrowClass();
        ~ArrowClass();

        void ArrowClass::Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::vec3 location);
        void UpdateLocation(float deltaTime);
        void PointTo(glm::vec3 locToPoint);


    public:
        glm::mat4 aMatrix;
        cam::Camera* orientation;
        float bodySize;

        glm::vec3 arrowColor = glm::vec3(0.5, 0.5, 0.5);
    };
}   // namespace m1

#endif