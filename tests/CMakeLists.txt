add_subdirectory(gtest)

#if (MSVC)
#    set_target_properties(gtest PROPERTIES 
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
#        POSITION_INDEPENDENT_CODE ON
#    )
#
#    set_target_properties(gtest_main PROPERTIES 
#        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
#        POSITION_INDEPENDENT_CODE ON
#    )
#else()
#    set_target_properties(gtest PROPERTIES 
#        POSITION_INDEPENDENT_CODE ON
#    )
#
#    set_target_properties(gtest_main PROPERTIES 
#        POSITION_INDEPENDENT_CODE ON
#    )
#endif()



add_executable(GTransformsTests tests.cpp)

target_link_libraries(GTransformsTests gtest gtest_main GTransforms)
#target_compile_options(GTransformsTests PRIVATE ${CMAKE_CXX_FLAGS_DEBUG})

add_test(NAME TestTransforms COMMAND GTransformsTests)