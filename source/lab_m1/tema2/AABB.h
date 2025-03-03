#pragma once

#ifndef __AABB_H__
#define __AABB_H__

#include "components/simple_scene.h"

namespace m1
{
    class BoxClass
    {
    public:
        BoxClass() {};
        ~BoxClass() {};

    public:
        float minX;
        float minY;
        float minZ;

        float maxX;
        float maxY;
        float maxZ;


        void setAABB(std::vector<VertexFormat> vertices, glm::mat4 Matrix) {
            
            glm::vec4 v = Matrix * glm::vec4(vertices[0].position, 1);

            minX = v.x;
            maxX = v.x;

            minY = v.y;
            maxY = v.y;

            minZ = v.z;
            maxZ = v.z;

            for (int i = 0; i < vertices.size(); i++) {
                glm::vec4 v = Matrix* glm::vec4(vertices[i].position, 1);

                if (minX > v.x) {
                    minX = v.x;
                }
                if (maxX < v.x) {
                    maxX = v.x;
                }

                if (minY > v.y) {
                    minY = v.y;
                }
                if (maxY < v.y) {
                    maxY = v.y;
                }

                if (minZ > v.z) {
                    minZ = v.z;
                }
                if (maxZ < v.z) {
                    maxZ = v.z;
                }
            }
        }

    };
}   // namespace m1

#endif