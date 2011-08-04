ADD_EXECUTABLE (test_init test_init.c)
ADD_EXECUTABLE (test_pause test_pause.c)

ADD_EXECUTABLE (test_sched test_sched.c)
TARGET_LINK_LIBRARIES (test_sched ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_sem test_sem.c)
TARGET_LINK_LIBRARIES (test_sem ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_clock test_clock.c)
TARGET_LINK_LIBRARIES (test_clock ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_thread_create test_thread_create.c)
TARGET_LINK_LIBRARIES (test_thread_create ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_thread_join test_thread_join.c)
TARGET_LINK_LIBRARIES (test_thread_join ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_tsd test_tsd.c)
TARGET_LINK_LIBRARIES (test_tsd ${LIBPTHREAD_NAME})

# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
ADD_TEST (test_init test_init)
ADD_TEST (test_pause test_pause)

ADD_TEST (test_sched test_sched)
ADD_TEST (test_sem test_sem)
ADD_TEST (test_clock test_clock)
ADD_TEST (test_thread_create test_thread_create)
ADD_TEST (test_thread_join test_thread_join)
ADD_TEST (test_tsd test_tsd)
