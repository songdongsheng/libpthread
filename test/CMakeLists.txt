ADD_EXECUTABLE(test_init test_init.c)
ADD_EXECUTABLE(test_pause test_pause.c)

#TARGET_LINK_LIBRARIES(test_sched pthread)

INCLUDE(CTest)
ADD_TEST(test_init test_init)
ADD_TEST(test_pause test_pause)

MESSAGE("CTEST name: ${CTEST_BUILD_NAME}")

#SET(LD_VARNAME "PATH")
#SET(LD_PATH "${PROJECT_BINARY_DIR}/test;${PROJECT_BINARY_DIR}/src;$ENV{PATH}")
#STRING(REPLACE ";" "\\;" LD_PATH "${LD_PATH}")
#SET_TESTS_PROPERTIES(SomeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
ADD_TEST(test_init test_init)
ADD_TEST(test_pause test_pause)
