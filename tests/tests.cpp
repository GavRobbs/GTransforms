#include "gtest/gtest.h"
#include "glm/glm.hpp"
#include "transform.h"

TEST(TestTransforms, CheckInitialValues){
    gtransforms::Transform transform;

    auto initial_pos = transform.GetLocalPosition();
    EXPECT_FLOAT_EQ(initial_pos.x, 0.0f);
    EXPECT_FLOAT_EQ(initial_pos.y, 0.0f);
    EXPECT_FLOAT_EQ(initial_pos.z, 0.0f);

    auto initial_rot = transform.GetLocalRotation();
    EXPECT_FLOAT_EQ(initial_rot.x, 0.0f);
    EXPECT_FLOAT_EQ(initial_rot.y, 0.0f);
    EXPECT_FLOAT_EQ(initial_rot.z, 0.0f);
    EXPECT_FLOAT_EQ(initial_rot.w, 1.0f);

    auto initial_scale = transform.GetLocalScale();
    EXPECT_FLOAT_EQ(initial_scale.x, 1.0f);
    EXPECT_FLOAT_EQ(initial_scale.y, 1.0f);
    EXPECT_FLOAT_EQ(initial_scale.z, 1.0f);

}

int main(int argc, char **argv){
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}