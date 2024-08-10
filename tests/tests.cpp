#include "gtest/gtest.h"
#include "glm/glm.hpp"
#include "transform.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#define TEST_VECTOR_EQUALITY(a, b) \
    EXPECT_FLOAT_EQ(a.x, b.x); \
    EXPECT_FLOAT_EQ(a.y, b.y); \
    EXPECT_FLOAT_EQ(a.z, b.z)

#define TEST_VECTOR_EQUALITY_2(a, b) \
    EXPECT_NEAR(a.x, b.x, 1e-6); \
    EXPECT_NEAR(a.y, b.y, 1e-6); \
    EXPECT_NEAR(a.z, b.z, 1e-6)

#define TEST_QUATERNION_EQUALITY(a, b) \
    EXPECT_FLOAT_EQ(a.x, b.x); \
    EXPECT_FLOAT_EQ(a.y, b.y); \
    EXPECT_FLOAT_EQ(a.z, b.z); \
    EXPECT_FLOAT_EQ(a.w, b.w)

#define TEST_DIAGONALS_MAT3(mat, vec) \
    EXPECT_FLOAT_EQ(mat[0][0], vec.x); \
    EXPECT_FLOAT_EQ(mat[1][1], vec.y); \
    EXPECT_FLOAT_EQ(mat[2][2], vec.z)

#define TEST_COLUMN_MAT3(mat, col, vec) \
    EXPECT_FLOAT_EQ(mat[col][0], vec.x); \
    EXPECT_FLOAT_EQ(mat[col][1], vec.y); \
    EXPECT_FLOAT_EQ(mat[col][2], vec.z)

#define TEST_DIAGONALS_MAT4(mat, vec) \
    EXPECT_FLOAT_EQ(mat[0][0], vec.x); \
    EXPECT_FLOAT_EQ(mat[1][1], vec.y); \
    EXPECT_FLOAT_EQ(mat[2][2], vec.z); \
    EXPECT_FLOAT_EQ(mat[3][3], vec.w)

#define TEST_COLUMN_MAT4(mat, col, vec) \
    EXPECT_FLOAT_EQ(mat[col][0], vec.x); \
    EXPECT_FLOAT_EQ(mat[col][1], vec.y); \
    EXPECT_FLOAT_EQ(mat[col][2], vec.z); \
    EXPECT_FLOAT_EQ(mat[col][3], vec.w)

//This one is for when float point starts to get annoying
#define TEST_COLUMN_MAT4_2(mat, col, vec) \
    EXPECT_NEAR(mat[col][0], vec.x, 1e-6); \
    EXPECT_NEAR(mat[col][1], vec.y, 1e-6); \
    EXPECT_NEAR(mat[col][2], vec.z, 1e-6); \
    EXPECT_NEAR(mat[col][3], vec.w, 1e-6)

#define TEST_MAT4_COLUMN_ARRAY(mat, col_array) \
    TEST_COLUMN_MAT4(mat, 0, col_array[0]); \
    TEST_COLUMN_MAT4(mat, 1, col_array[1]); \
    TEST_COLUMN_MAT4(mat, 2, col_array[2]); \
    TEST_COLUMN_MAT4(mat, 3, col_array[3])

#define TEST_MAT4_COLUMN_ARRAY_2(mat, col_array) \
    TEST_COLUMN_MAT4_2(mat, 0, col_array[0]); \
    TEST_COLUMN_MAT4_2(mat, 1, col_array[1]); \
    TEST_COLUMN_MAT4_2(mat, 2, col_array[2]); \
    TEST_COLUMN_MAT4_2(mat, 3, col_array[3])

TEST(TestTransforms, CheckInitialValues){
    gtransforms::Transform transform;

    auto initial_pos = transform.GetLocalPosition();
    glm::vec3 ip_check{0.0f, 0.0f, 0.0f};
    TEST_VECTOR_EQUALITY(initial_pos, ip_check);

    auto initial_rot = transform.GetLocalRotation();
    glm::quat rot_check{1.0f, 0.0f, 0.0f, 0.0f};
    TEST_QUATERNION_EQUALITY(initial_rot, rot_check);

    auto initial_scale = transform.GetLocalScale();
    glm::vec3 expected_scale{1.0f, 1.0f, 1.0f};
    TEST_VECTOR_EQUALITY(initial_scale, expected_scale);
}

TEST(TestTransforms, CheckMatrixGeneration){

    gtransforms::Transform transform1;
    transform1.SetPosition(glm::vec3{0.0f, 0.0f, -2.0f});

    glm::mat4 t1_global_mat = transform1.GetGlobalTransform();
    glm::vec4 t1_pos = t1_global_mat[3];
    glm::vec4 col_expected{0.0f, 0.0f, -2.0f, 1.0f};

    TEST_QUATERNION_EQUALITY(t1_pos, col_expected);

    gtransforms::Transform transform2;
    transform2.SetScale(glm::vec3{1.0f, 5.0f, -1.0f});
    glm::mat4 t2_global_mat = transform2.GetGlobalTransform();
    EXPECT_FLOAT_EQ(t2_global_mat[0][0], 1.0f);
    EXPECT_FLOAT_EQ(t2_global_mat[1][1], 5.0f);
    EXPECT_FLOAT_EQ(t2_global_mat[2][2], -1.0f);
    EXPECT_FLOAT_EQ(t2_global_mat[3][3], 1.0f);

    gtransforms::Transform transform3;
    transform3.SetRotation(glm::angleAxis(glm::radians(60.0f), glm::vec3{1.0f, 0.0f, 0.0f}));
    glm::quat x_rot = transform3.GetGlobalRotation();
    glm::vec4 expected_t3_values{0.5f, 0.0f, 0.0f, 0.8660254f};
    TEST_QUATERNION_EQUALITY(x_rot, expected_t3_values);

    glm::mat4 t3_globalMatrix = transform3.GetGlobalTransform();
    glm::vec4 t3_expected_columns[4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.8660254f, 0.0f},
        {0.0f, -0.8660254f, 0.5f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    TEST_MAT4_COLUMN_ARRAY(t3_globalMatrix, t3_expected_columns);

    gtransforms::Transform transform4;
    transform4.SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3{0.0f, 1.0f, 0.0f}));
    glm::quat y_rot = transform4.GetGlobalRotation();
    glm::vec4 expected_t4_values{0.0f, 0.7071068f, 0.0f, 0.7071068f};
    TEST_QUATERNION_EQUALITY(y_rot, expected_t4_values);

    glm::mat4 t4_globalMatrix = transform4.GetGlobalTransform();
    glm::vec4 t4_expected_columns[4] = {
        {0.0f, 0.0f, -1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    TEST_MAT4_COLUMN_ARRAY_2(t4_globalMatrix, t4_expected_columns);

    gtransforms::Transform transform5;
    transform5.SetRotation(glm::angleAxis(glm::radians(45.0f), glm::vec3{0.0f, 0.0f, 1.0f}));
    glm::quat z_rot = transform5.GetGlobalRotation();
    glm::vec4 expected_t5_values{0.0f, 0.0f, 0.3826834f, 0.9238795f};
    TEST_QUATERNION_EQUALITY(z_rot, expected_t5_values);

    glm::mat4 t5_globalMatrix = transform5.GetGlobalTransform();
    glm::vec4 t5_expected_columns[4] = {
        {0.7071068f, 0.7071068f, 0.0f, 0.0f},
        {-0.7071068f, 0.7071068f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    TEST_MAT4_COLUMN_ARRAY_2(t5_globalMatrix, t5_expected_columns);

}

TEST(TestTransforms, SpaceConversions){

    gtransforms::Transform transform1;
    transform1.SetPosition(glm::vec3{0.0f, 0.0f, 6.0f});
    glm::vec3 ls_point = transform1.WorldToLocal(glm::vec3{0.0f, 0.0f, 0.0f});
    glm::vec3 expected_ls_point{0.0f, 0.0f, -6.0f};
    TEST_VECTOR_EQUALITY(ls_point, expected_ls_point);

    gtransforms::Transform transform2;
    transform2.SetPosition(glm::vec3{0.0f, 0.0f, 6.0f});
    glm::vec3 ws_point = transform2.LocalToWorld(glm::vec3{0.0f, 2.0f, 0.0f});
    glm::vec3 expected_ws_point{0.0f, 2.0f, 6.0f};
    TEST_VECTOR_EQUALITY(ws_point, expected_ws_point);

    transform2.SetParent(&transform1, true);
    glm::vec3 t2_new_lp = transform2.GetLocalPosition();
    glm::vec3 t2_expected_lp{0.0f, 0.0f, 0.0f};
    TEST_VECTOR_EQUALITY(t2_new_lp, t2_expected_lp);
}

TEST(TestTransforms, ParentChild){
    gtransforms::Transform transform1{};
    gtransforms::Transform transform2{};

    transform2.SetPosition(glm::vec3{1.0f, 0.0f, 0.0f});
    transform2.SetParent(&transform1, true);

    transform2.RotateAroundParent(
        glm::angleAxis(
            glm::radians(90.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
        )
    );

    glm::vec3 t2_rot_pos = transform2.GetGlobalPosition();
    glm::vec3 t2_expected_rot_pos{0.0f, 0.0f, -1.0f};
    TEST_VECTOR_EQUALITY_2(t2_rot_pos, t2_expected_rot_pos);
    //Floating point is beating my ass

    transform2.RotateAroundPoint(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::angleAxis(
            glm::radians(90.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
        )
    );
    glm::vec3 t2_rot_pos_2 = transform2.GetGlobalPosition();
    glm::vec3 t2_expected_rot_pos_2{-1.0f, 0.0f, 0.0f};
    TEST_VECTOR_EQUALITY_2(t2_rot_pos_2, t2_expected_rot_pos_2);
}

int main(int argc, char **argv){
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}