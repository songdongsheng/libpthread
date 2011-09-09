ADD_EXECUTABLE (test_init test_init.c)
ADD_EXECUTABLE (test_int64 test_int64.c)
ADD_EXECUTABLE (test_max_key test_max_key.c)
ADD_EXECUTABLE (test_pause test_pause.c)
ADD_EXECUTABLE (test_realtime test_realtime.c)
ADD_EXECUTABLE (test_size test_size.c)
ADD_EXECUTABLE (test_sleep test_sleep.c)

ADD_EXECUTABLE (test_clock_getres test_clock_getres.c)
TARGET_LINK_LIBRARIES (test_clock_getres ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_clock_gettime test_clock_gettime.c)
TARGET_LINK_LIBRARIES (test_clock_gettime ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_clock_nanosleep test_clock_nanosleep.c)
TARGET_LINK_LIBRARIES (test_clock_nanosleep ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_clock_settime test_clock_settime.c)
TARGET_LINK_LIBRARIES (test_clock_settime ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_key test_key.c)
TARGET_LINK_LIBRARIES (test_key ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_mutex test_mutex.c)
TARGET_LINK_LIBRARIES (test_mutex ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_nanosleep test_nanosleep.c)
TARGET_LINK_LIBRARIES (test_nanosleep ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_once test_once.c)
TARGET_LINK_LIBRARIES (test_once ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_sched test_sched.c)
TARGET_LINK_LIBRARIES (test_sched ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_sem test_sem.c)
TARGET_LINK_LIBRARIES (test_sem ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_speed test_speed.c)
TARGET_LINK_LIBRARIES (test_speed ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_spin test_spin.c)
TARGET_LINK_LIBRARIES (test_spin ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_spin_rwlock test_spin_rwlock.c)
TARGET_LINK_LIBRARIES (test_spin_rwlock ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_thread_create test_thread_create.c)
TARGET_LINK_LIBRARIES (test_thread_create ${LIBPTHREAD_NAME})

ADD_EXECUTABLE (test_thread_join test_thread_join.c)
TARGET_LINK_LIBRARIES (test_thread_join ${LIBPTHREAD_NAME})
# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
#ADD_TEST (test_init test_init)
#ADD_TEST (test_int64 test_int64)
#ADD_TEST (test_max_key test_max_key)
#ADD_TEST (test_pause test_pause)
#ADD_TEST (test_realtime test_realtime)
#ADD_TEST (test_size test_size)
#ADD_TEST (test_sleep test_sleep)

ADD_TEST (test_clock_getres test_clock_getres)
ADD_TEST (test_clock_gettime test_clock_gettime)
ADD_TEST (test_clock_nanosleep test_clock_nanosleep)
#ADD_TEST (test_clock_settime test_clock_settime)
ADD_TEST (test_key test_key)
ADD_TEST (test_mutex test_mutex)
ADD_TEST (test_nanosleep test_nanosleep)
ADD_TEST (test_once test_once)
ADD_TEST (test_sched test_sched)
ADD_TEST (test_sem test_sem)
#ADD_TEST (test_speed test_speed)
ADD_TEST (test_spin test_spin)
ADD_TEST (test_spin_rwlock test_spin_rwlock)
ADD_TEST (test_thread_create test_thread_create)
ADD_TEST (test_thread_join test_thread_join)
