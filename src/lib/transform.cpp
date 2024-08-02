#include "transform.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform(const std::string &n) : name(n){
    /* We set our defaults here, with everything being 0 or an identity transformation */
    localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    localScale = glm::vec3(1.0f, 1.0f, 1.0f);
    localRotation = glm::identity<glm::quat>();

    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);

    parent = NULL;
}

const glm::vec3 & Transform::GetLocalPosition() const{
    return localPosition;
}
   
const glm::quat & Transform::GetLocalRotation() const{
    return localRotation;
}

const glm::vec3 & Transform::GetLocalScale() const{
    return localScale;
}

glm::vec3 Transform::GetGlobalPosition(){
    glm::mat4 gt = GetGlobalTransform();

    return glm::vec3(gt * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

glm::quat Transform::GetGlobalRotation(){
    if(parent == NULL){
        return localRotation;
    } else{
        return localRotation * parent->localRotation;
    }
}

glm::vec3 Transform::GetGlobalScale(){

    if(parent == NULL){
        return localScale;
    }

    return parent->GetGlobalScale() * localScale;
}

glm::mat4 Transform::GetLocalTransform(){
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(transform, localPosition);
    transform *= glm::toMat4(localRotation);
    transform = glm::scale(transform, localScale);

    return transform;
}

glm::mat4 Transform::GetGlobalTransform(){

    glm::mat4 transform = GetLocalTransform();    

    if(parent == NULL){
        transform_matrix = transform;
    } else{
        transform_matrix = parent->GetGlobalTransform() * transform;
    }

    inverse_transform_matrix = glm::inverse(transform_matrix);
    return transform_matrix;
}

glm::mat4 Transform::GetInverseGlobalTransform(){

    /* This automatically recalculates our global transform and its inverse, so we'll use it to repopulate the stored value*/
    GetGlobalTransform();
    return inverse_transform_matrix;

}

void Transform::SetPosition_Local(const glm::vec3 & pos){
    //Directly sets the local position, not basis aware
    localPosition = pos;
}

void Transform::SetPosition(const glm::vec3 & pos){
    /* This takes a world position and sets the local position, which is why we use the inverse
    Note that bases aren't affected by translation or scaling as we want to keep them as unit vectors
    to avoid chaos */

    if(parent == nullptr){
        localPosition = pos;
        return;
    }

    glm::mat4 gt_i = parent->GetInverseGlobalTransform();
    localPosition = gt_i * glm::vec4(pos.x, pos.y, pos.z, 1.0f);

}

void Transform::Translate_Local(const glm::vec3 & delta){
    //This moves the transform along its local axes
    localPosition += right * delta.x + up * delta.y + forward * delta.z;
}

void Transform::Translate(const glm::vec3 & delta){
    /* This function moves the transform in a basis-unaware manner, using the global axes*/
    localPosition += delta;
}

void Transform::SetRotation_Local(const glm::quat & rot){
    //A local rotation changes the basis vectors
    //This is a set operation, so it wipes it clean, which means the basis vectors
    //start at hte default and are multiplied appropriately
    localRotation = rot;
    forward = rot * glm::vec3(0.0f, 0.0f, -1.0f);
    right = rot * glm::vec3(1.0f, 0.0f, 0.0f);
    up = rot * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::SetRotation_Global(const glm::quat & rot){
    //This sets the rotation of the object around a global axis

    glm::quat global_rot; 

    if(parent != NULL){
        global_rot = parent->GetGlobalRotation();
    } else{
        global_rot = glm::identity<glm::quat>();
    }

    localRotation = rot * glm::inverse(global_rot);

    forward = rot * glm::vec3(0.0f, 0.0f, -1.0f);
    right = rot * glm::vec3(1.0f, 0.0f, 0.0f);
    up = rot * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Transform::Rotate_Local(const glm::vec3 & angle_axis_degrees){
    //This multiplies the current local rotation of the object with the new rotation
    //Note that this changes the basis
    //and note also that the axes uses are the local axes of the transform

    glm::quat combined_quat = 
    glm::angleAxis(
        glm::radians(angle_axis_degrees.x), 
        glm::vec3(1.0f, 0.0f, 0.0f)
        ) * 
    glm::angleAxis(
        glm::radians(angle_axis_degrees.y), 
        glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::angleAxis(
        glm::radians(angle_axis_degrees.z), 
        glm::vec3(0.0f, 0.0f, -1.0f)
        );

    localRotation = combined_quat * localRotation; 
    forward = combined_quat * forward;
    right = combined_quat * right;
    up = combined_quat * up;
}

glm::mat3 Transform::GetLocalBasisVectors(){

    basis = glm::mat3(right.x, right.y, right.z, up.x, up.y, up.z, forward.x, forward.y, forward.z);
    inverse_basis = glm::inverse(basis);
    //This gets our basis vectors for our local coordinate system
    return basis;
}

glm::mat3 Transform::GetInverseBasisVectors(){
    
    GetLocalBasisVectors();
    return inverse_basis;
}

void Transform::Display(){

    glm::vec3 gp = GetGlobalPosition();

    std::cout << "Transform: " << name << std::endl;
    if(parent == NULL){
        std::cout << "Parent: NONE" << std::endl;
    } else{
        std::cout << "Parent: " << parent->name << std::endl;
    }
    std::cout << "Local Position (relative to parent): " << glm::to_string(localPosition) << std::endl;
    std::cout << "Local Rotation Quaternion: " << glm::to_string(localRotation) << std::endl;
    std::cout << "Global Rotation Quaternion: " << glm::to_string(GetGlobalRotation()) << std::endl;
    std::cout << "Local Scale (relative to parent): " << glm::to_string(localScale) << std::endl;
    std::cout << "World Scale (relative to world origin): " << glm::to_string(GetGlobalScale()) << std::endl;
    std::cout << "Basis Matrix: " << glm::to_string(GetLocalBasisVectors()) << std::endl;
    std::cout << "World Position: " << glm::to_string(GetGlobalPosition()) << std::endl;
    std::cout << std::endl;

}

void Transform::SetParent(Transform * p, bool keepWorldPosition = true){
  
    parent = p;
   
    if(p == NULL){
        return;
    }

    if(keepWorldPosition){        
        localPosition =  parent->GetInverseGlobalTransform() * glm::vec4(localPosition, 1.0f);
        localRotation = localRotation * glm::inverse(parent->GetGlobalRotation());
        localScale = localScale;
    }     
}

void Transform::SetScale(const glm::vec3 &scale){

    //This changes the local scale
    localScale = scale;
}

void Transform::RotateAroundPoint(const glm::vec3 & point, const glm::quat & rot){

    //Rotate the transform around a point around an arbitary axis
    //First we get the global position of the transform
    //Then we put it in the point's coordinate system
    //Then we perform the rotation
    //Then we put it back in the global coordinate system
    //Then we put it back in the local coordinate system by multiplying by the inverse of the global transform

    glm::vec3 global_pos = GetGlobalPosition();
    glm::vec3 relative_to_point = global_pos - point;
    glm::vec3 rotated = rot * relative_to_point;
    glm::vec3 rot_to_global = rotated + point;
    localPosition = GetInverseGlobalTransform() * glm::vec4(rot_to_global, 1.0f);
}

glm::vec3 Transform::LocalToWorld(const glm::vec3 & local_point){
    return glm::vec3(GetGlobalTransform() * glm::vec4(local_point, 1.0f));
}

glm::vec3 Transform::WorldToLocal(const glm::vec3 & world_point){
    return glm::vec3(GetInverseGlobalTransform() * glm::vec4(world_point, 1.0f));
}
