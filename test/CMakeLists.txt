ADD_EXECUTABLE (test_init test_init.c)
ADD_EXECUTABLE (test_pause test_pause.c)

ADD_EXECUTABLE (test_sched test_sched.c)
TARGET_LINK_LIBRARIES (test_sched ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_sem test_sem.c)
TARGET_LINK_LIBRARIES (test_sem ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_clock test_clock.c)
TARGET_LINK_LIBRARIES (test_clock ${LIBPTHREAD_NAME})

MESSAGE ("CTEST name: ${CTEST_BUILD_NAME}")

#SET(LD_VARNAME "PATH")
#SET(LD_PATH "${PROJECT_BINARY_DIR}/test;${PROJECT_BINARY_DIR}/src;$ENV{PATH}")
#STRING(REPLACE ";" "\\;" LD_PATH "${LD_PATH}")
#SET_TESTS_PROPERTIES(SomeTest PROPERTIES ENVIRONMENT "${LD_VARNAME}=${LD_PATH}")

# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
ADD_TEST (test_init test_init)
ADD_TEST (test_pause test_pause)
ADD_TEST (test_sched test_sched)
ADD_TEST (test_sem test_sem)
ADD_TEST (test_clock test_clock)
