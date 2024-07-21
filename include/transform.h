#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <string>


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

    //To help with tagging for demonstration purposes
    std::string name;

    glm::mat4 transform_matrix;
    glm::mat4 inverse_transform_matrix;

    glm::mat3 basis;
    glm::mat3 inverse_basis;

    public:

    Transform(const std::string &n);
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

    void Translate_Local(const glm::vec3 &delta);
    void Translate(const glm::vec3 &delta);
    void SetPosition_Local(const glm::vec3 & pos);
    void SetPosition(const glm::vec3 &pos);

    void SetScale(const glm::vec3 & scale);

    //This sets the rotation of the object around its own axis
    void SetRotation_Local(const glm::quat & rot);

    //This sets the rotation of the object relative to its own parent
    void SetRotation_Global(const glm::quat & rot);

    //This multiplies the current local rotation of the object with the new rotation
    void Rotate_Local(const glm::vec3 & angle_axis_degrees);

    //Rotates this transform around a point around a specified axis
    void RotateAroundPoint(const glm::vec3 & point, const glm::quat & rot);

    //Converts a point from local space to world space
    glm::vec3 LocalToWorld(const glm::vec3 & local_point);

    //Converts a point from world space to local space
    glm::vec3 WorldToLocal(const glm::vec3 & world_point);

    void Display();

    void SetParent(Transform * p, bool keepWorldPosition);
};