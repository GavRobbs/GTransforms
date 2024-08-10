#include "transform.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace gtransforms;

Transform::Transform(){
    /* We set our defaults here, with everything being 0 or an identity transformation */
    localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    localScale = glm::vec3(1.0f, 1.0f, 1.0f);
    localRotation = glm::identity<glm::quat>();

    right = glm::vec3(1.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, 1.0f);

    parent = NULL;
}

Transform::~Transform(){

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
        return parent->GetGlobalRotation() * localRotation;
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

void Transform::Translate(const glm::vec3 &delta, const Space &space)
{
    if(space == Space::LOCAL){
        localPosition += right * delta.x + up * delta.y + forward * delta.z;
    } else{
        localPosition += delta;
    }

}
void Transform::Rotate(const glm::quat &rot, const Space &space)
{
    if(space == Space::GLOBAL){
        auto global_rotation = GetGlobalRotation();
        auto new_gr = rot * global_rotation;

        if(parent == nullptr){
            localRotation = new_gr;
        } else{
            localRotation = glm::inverse(parent->GetGlobalRotation()) * new_gr;
        }

    } else{
        localRotation = localRotation * rot;
    }

    forward = (localRotation * glm::vec3(0.0f, 0.0f, 1.0f)) * localScale;
    right = (localRotation * glm::vec3(1.0f, 0.0f, 0.0f)) * localScale;
    up = (localRotation * glm::vec3(0.0f, 1.0f, 0.0f)) * localScale;
}

void Transform::Scale(const glm::vec3 &scale, const Space &space)
{
    if(space == Space::LOCAL){
        localScale *= scale;
    } else{
        glm::vec3 globalScale = GetGlobalScale();
        glm::vec3 invglobalScale = glm::vec3(1.0f/globalScale.x, 1.0f/globalScale.y, 1.0f/globalScale.z);
        localScale = scale * localScale * invglobalScale;

        localScale.x = scale.x * localScale.x/globalScale.x;
        localScale.y = scale.y * localScale.y/globalScale.y;
        localScale.z = scale.z * localScale.z/globalScale.z;

        forward *= invglobalScale * scale;
        right *= invglobalScale * scale;
        up *= invglobalScale * scale;
    }
}

void Transform::SetPosition(const glm::vec3 &pos, const Space &space)
{
    if(space == Space::LOCAL){
        localPosition = pos;
    } else{

        if(parent == nullptr){
            localPosition = pos;
        } else{
            localPosition = parent->WorldToLocal(pos);
        }
    }
}

void Transform::SetRotation(const glm::quat &rot, const Space &space)
{
    if(space == Space::GLOBAL){
        glm::quat global_rot; 

        if(parent != NULL){
            global_rot = parent->GetGlobalRotation();
        } else{
            global_rot = glm::identity<glm::quat>();
        }

        localRotation = glm::inverse(global_rot) * rot;
    } else{
        localRotation = rot;
    }

    forward = localRotation * glm::vec3(0.0f, 0.0f, 1.0f);
    right = localRotation * glm::vec3(1.0f, 0.0f, 0.0f);
    up = localRotation * glm::vec3(0.0f, 1.0f, 0.0f);

}

void Transform::SetScale(const glm::vec3 &scale, const Space &space)
{
    if(space == Space::LOCAL){
        localScale = scale;
    } 
    else{
        if(parent == nullptr){
            localScale = scale;
        } else{
            glm::vec3 globalScale = parent->GetGlobalScale();
            localScale.x = scale.x/globalScale.x;
            localScale.y = scale.y/globalScale.y;
            localScale.z = scale.z/globalScale.z;
        }
    }
}

glm::mat3 Transform::GetLocalBasisVectors(){

    auto right_n = glm::normalize(right);
    auto up_n = glm::normalize(up);
    auto forward_n = glm::normalize(forward);

    basis = glm::mat3(right_n.x, right_n.y, right_n.z, up_n.x, up_n.y, up_n.z, forward_n.x, forward_n.y, forward_n.z);
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
        localRotation = glm::inverse(parent->GetGlobalRotation()) * localRotation;

        glm::vec3 sv = parent->GetGlobalScale();
        sv.x = 1.0f/sv.x;
        sv.y = 1.0f/sv.y;
        sv.z = 1.0f/sv.z;

        localScale = localScale * sv;
    }     
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

    if(parent != nullptr){
        localPosition = parent->GetInverseGlobalTransform() * glm::vec4(rot_to_global, 1.0f);
    } else{
        localPosition = rot_to_global;
    }
}

void Transform::RotateAroundParent(const glm::quat & rot){

    //Rotate the transform around a point around an arbitary axis
    //First we get the global position of the transform
    //Then we put it in the point's coordinate system
    //Then we perform the rotation
    //Then we put it back in the global coordinate system
    //Then we put it back in the local coordinate system by multiplying by the inverse of the global transform

    glm::vec3 parent_pos;
    if(parent == nullptr){
        parent_pos = glm::vec3{0.0f, 0.0f, 0.0f};
    } else{
        parent_pos = parent->GetGlobalPosition();
    }

    glm::vec3 global_pos = GetGlobalPosition();

    glm::vec3 relative_to_point = global_pos - parent_pos;

    glm::vec3 rotated = rot * relative_to_point;

    glm::vec3 rot_to_global = rotated + parent_pos;

     if(parent != nullptr){
        localPosition = parent->GetInverseGlobalTransform() * glm::vec4(rot_to_global, 1.0f);
    } else{
        localPosition = rot_to_global;
    }

}

glm::vec3 Transform::LocalToWorld(const glm::vec3 & local_point){
    return glm::vec3(GetGlobalTransform() * glm::vec4(local_point, 1.0f));
}

glm::vec3 Transform::WorldToLocal(const glm::vec3 & world_point){
    return glm::vec3(GetInverseGlobalTransform() * glm::vec4(world_point, 1.0f));
}
