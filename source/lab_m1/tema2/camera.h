#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace cam
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position = position + glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            position = position + glm::normalize(up) * distance;
        }
        void MoveUpward(float distance)
        {
            position = position + glm::vec3(0,1,0)* distance;
        }
        void TranslateRight(float distance)
        {
            position = position + glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::vec4 f = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(f));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 f = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(f));

            glm::vec4 r = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(r));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

       

        void RotateFirstPerson_OZ(float angle)
        {
            glm::vec4 r = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
            right = glm::normalize(r);

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        void RotateObj_OX(float angle)
        {
            glm::vec4 f = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(f));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        void RotateObj_OY(float angle)
        {
            glm::vec4 f = glm::rotate(glm::mat4(1.0f), angle, up) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(f));

            glm::vec4 r = glm::rotate(glm::mat4(1.0f), angle, up) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(r));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }


        void RotateObj_OZ(float angle)
        {
            glm::vec4 r = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(r));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_Obj(float angle, glm::vec3 ObjV) {
            TranslateForward(distanceToTarget);
            RotateByObjV(angle, ObjV);
            TranslateForward(-distanceToTarget);
        }
        void RotateByObjV(float angle, glm::vec3 ObjV) {
            glm::vec4 f = glm::rotate(glm::mat4(1.0f), angle, ObjV) * glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(f));

            glm::vec4 r = glm::rotate(glm::mat4(1.0f), angle, ObjV) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(r));

            up = glm::cross(right, forward);
            up = glm::normalize(up);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
            //return position + glm::vec3(forward.x,forward.y -0.1f, forward.z) * distanceToTarget;

        }

        void GetSetCameraPosition(glm::vec3 objPos)
        {
            position = objPos - forward * distanceToTarget;
        }

        void SetPackagePosition(glm::vec3 objPos)
        {
            position = objPos - glm::vec3(0,1.5f,0);
        }


    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
