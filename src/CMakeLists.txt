ADD_LIBRARY (${LIBPTHREAD_NAME} SHARED libpthread.def version.rc
        attr.c
        clock.c
        key.c
        mutex.c
        pthread.c
        sched.c
        sem.c
        init_shared.c)
SET_TARGET_PROPERTIES (${LIBPTHREAD_NAME} PROPERTIES VERSION ${libpthread_VERSION_MAJOR}.${libpthread_VERSION_MINOR})

IF (CMAKE_COMPILER_IS_GNUCC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    TARGET_LINK_LIBRARIES(${LIBPTHREAD_NAME} gcov)
ENDIF()

ADD_CUSTOM_COMMAND(TARGET ${LIBPTHREAD_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/src/libpthread.dll ${PROJECT_BINARY_DIR}/test/libpthread.dll)

INSTALL (TARGETS ${LIBPTHREAD_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#
# It's complex and expensive in static libray automatically initialized.
# Therefore, I do not intend to support static library in the near future.
#
#ADD_LIBRARY (pthread_static STATIC
#        clock.c
#        sched.c
#        sem.c
#        thread.c
#        init_static.c)
#SET_TARGET_PROPERTIES (pthread_static PROPERTIES OUTPUT_NAME "pthread")
#INSTALL (TARGETS pthread_static
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)
