ADD_LIBRARY(pthread SHARED libpthread.def version.rc
        sched.c
        sem.c)
SET_TARGET_PROPERTIES(pthread PROPERTIES VERSION 0.1)

IF(CMAKE_COMPILER_IS_GNUCC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_LINK_LIBRARIES(pthread gcov)
ENDIF()

INSTALL(TARGETS pthread
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

ADD_LIBRARY(pthread_static STATIC
        sched.c
        sem.c)
SET_TARGET_PROPERTIES(pthread_static PROPERTIES OUTPUT_NAME "pthread")
INSTALL(TARGETS pthread_static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
