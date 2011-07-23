ADD_LIBRARY(pthread SHARED libpthread.def version.rc
        sched.c
        sem.c)

IF(CMAKE_COMPILER_IS_GNUCC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_LINK_LIBRARIES(gcov)
ENDIF()

INSTALL(TARGETS pthread
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
