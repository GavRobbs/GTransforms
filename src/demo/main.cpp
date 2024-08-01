#include <iostream>
#include "transform.h"

int main(int argc, char **argv){

    Transform first_transform{"first transform"};
    Transform child{"child 1"};

    /* If the rotations work correctly, the local/global quaternion for the first transform should be 60 degrees around the y-axis
    while, the global transform for the child should be 90 degrees around the y-axis, with the local transform being 30 (so 60
    from the parent + 30 from the child make up the 90 degrees)*/
    std::cout << "Created the root transform " << std::endl;
    first_transform.SetPosition(glm::vec3(1.5f, 0.0f, 3.0f));
    first_transform.SetRotation_Global(glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    first_transform.Display();

    child.SetParent(&first_transform, true);

    child.SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    child.SetRotation_Global(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    child.Display();

    /* This rotates a point centered at the origin around another point at (1, 1, 0) at 45 degrees around the X axis*/

    Transform second_transform{"second transform"};
    second_transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    second_transform.RotateAroundPoint(
        glm::vec3(1.0f, 1.0f, 0.0f), 
        glm::angleAxis(
            glm::radians(45.0f), 
            glm::vec3(1.0f, 0.0f, 0.0f)
            )
        );
    second_transform.Display();


    return 0;
}