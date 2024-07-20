#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>


class Transform{
    private:
    //The position of the transform relative to its parent
    glm::vec3 localPosition;

    //The rotation of the transform with respect to its parent
    glm::quat rotation;

    //The scale of the transform - this is combined with the parent transform
    glm::vec3 localScale;

    //This defines the local basis vectors of the object
    //These are changed by local rotations
    glm::vec3 forward_axis;
    glm::vec3 right_axis;
    glm::vec3 up_axis;

    Transform * parent;

    public:
    const glm::vec3 & GetLocalPosition() const;
    const glm::quat & GetRotation() const;
    const glm::vec3 & GetLocalScale() const;

    const glm::vec3 & GetGlobalPosition() const;
    const glm::vec3 & GetGlobalScale() const;

    const glm::mat4 & GetCombinedTransform() const;
    const glm::mat3 & GetLocalBasisVectors() const;

    void Translate_Local(glm::vec3 delta);
    void Translate(glm::vec3 delta);
    void SetPosition_Local(glm::vec3 pos);
    void SetPosition(glm::vec3 pos);

    void SetScale(glm::vec3 scale);
    void Scale(glm::vec3 scale);

    //This sets the rotation of the object around its own axis
    void SetRotation_Local(const glm::quat & rot);

    //This sets the rotation of the object relative to its own parent
    void SetRotation_Global(const glm::quat & rot);

    //This multiplies the current local rotation of the object with the new rotation
    void Rotate_Local(const glm::quat & rot);
    
    //This multiples the current global rotation of the object with a new rotation
    void Rotate_Global(const glm::quat & rot);

    friend std::ostream& operator<<(std::ostream & os, const Transform& transform);
}