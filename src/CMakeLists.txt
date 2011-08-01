ADD_LIBRARY(pthread SHARED libpthread.def version.rc
        clock.c
        sched.c
        sem.c
        init_shared.c)
SET_TARGET_PROPERTIES(pthread PROPERTIES VERSION 0.1)

IF(CMAKE_COMPILER_IS_GNUCC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_LINK_LIBRARIES(pthread gcov)
ENDIF()

INSTALL(TARGETS pthread
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#
# It's complex and expensive in static libray automatically initialized.
# Therefore, I do not intend to support static library in the near future.
#
#ADD_LIBRARY(pthread_static STATIC
#        clock.c
#        sched.c
#        sem.c
#        init_static.c)
#SET_TARGET_PROPERTIES(pthread_static PROPERTIES OUTPUT_NAME "pthread")
#INSTALL(TARGETS pthread_static
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)
