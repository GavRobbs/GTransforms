#ifndef GTRANSFORMS_H
#define GTRANSFORMS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <string>

namespace gtransforms{

    enum Space {LOCAL, GLOBAL};

    class Transform{
        private:
        //The position of the transform relative to its parent
        glm::vec3 localPosition;

        //This is the rotation of the object around its own axes
        glm::quat localRotation;

        //The scale of the transform - this is combined with the parent transform
        glm::vec3 localScale;

        //This defines the local basis vectors of the object
        //These are changed by local rotations
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        Transform * parent;

        glm::mat4 transform_matrix;
        glm::mat4 inverse_transform_matrix;

        glm::mat3 basis;
        glm::mat3 inverse_basis;

        public:

        Transform();
        virtual ~Transform();
        const glm::vec3 & GetLocalPosition() const;
        const glm::quat & GetLocalRotation() const;
        const glm::vec3 & GetLocalScale() const;

        glm::vec3 GetGlobalPosition();
        glm::quat GetGlobalRotation();
        glm::vec3 GetGlobalScale() ;

        glm::mat4 GetGlobalTransform();
        glm::mat4 GetLocalTransform();
        glm::mat4 GetInverseGlobalTransform();
        glm::mat3 GetLocalBasisVectors();
        glm::mat3 GetInverseBasisVectors();


        void Translate(const glm::vec3 &delta, const Space &space = Space::LOCAL);
        void Rotate(const glm::quat &rot, const Space &space = Space::LOCAL);
        void Scale(const glm::vec3 &scale, const Space &space = Space::LOCAL);

        void SetPosition(const glm::vec3 &pos, const Space &space = Space::LOCAL);
        void SetRotation(const glm::quat &rot, const Space &space = Space::LOCAL);
        void SetScale(const glm::vec3 &scale, const Space &space = Space::LOCAL);

        //Rotates this transform around a point around a specified axis, both in world space
        void RotateAroundPoint(const glm::vec3 & point, const glm::quat & rot);
        void RotateAroundParent(const glm::quat & rot);

        //Converts a point from local space to world space
        glm::vec3 LocalToWorld(const glm::vec3 & local_point);

        //Converts a point from world space to local space
        glm::vec3 WorldToLocal(const glm::vec3 & world_point);

        void Display();

        void SetParent(Transform * p, bool keepWorldPosition);
};

}


#endif