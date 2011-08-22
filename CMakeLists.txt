#
# http://www.cmake.org/Wiki/CMake
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
# cmake --help
# cmake --help-command-list
#
# cmake -G "Unix Makefiles"
# cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=gmake
#
# If you want to use 32bit gcc which is capable of multilib support to generate 64bit objects:
# SET CFLAGS="-m64", SET CXXFLAGS="-m64", SET RCFLAGS="-F pe-x86-64", SET LDFLAGS="-m64 -s -static"
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=gmake
#
# http://www.cmake.org/Wiki/CMake_Cross_Compiling
# cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc
#
# If you want to use 32bit gcc which is capable of multilib support to generate 64bit objects:
# CFLAGS="-m64" CXXFLAGS="-m64" LDFLAGS="-m64" RCFLAGS="-F pe-x86-64" \
# cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc
#
# http://www.cmake.org/Wiki/CMake:CPackPackageGenerators
# cpack -C CPackConfig.cmake -G ZIP
# cpack -C CPackConfig.cmake
#
# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
# ctest -D Experimental
# ctest -D Help
#
# export PATH=${HOME}/gcc-4.6-windows-linux/bin:/usr/sbin:/usr/bin:/sbin:/bin
# i686-w64-mingw32-gcc -march=x86-64 -mtune=generic -dM -E -  < /dev/null | sort
# make VERBOSE=1
# signtool sign /t http://timestamp.verisign.com/scripts/timstamp.dll libpthread.dll
#
# GIT_SSL_NO_VERIFY=true GIT_CURL_VERBOSE=1 git clone -v https://github.com/songdongsheng/libpthread
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (libpthread C)
SET (PACKAGE_NAME libpthread)
SET (libpthread_VERSION_MAJOR 0)
SET (libpthread_VERSION_MINOR 1)
SET (libpthread_VERSION_PATCH 0)
INCLUDE(CTest)
ENABLE_TESTING ()

IF (NOT CMAKE_BUILD_TYPE)
    SET (CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)


# SET (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS})

if (MSVC)
    SET (LIBPTHREAD_NAME "libpthread")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
    ADD_DEFINITIONS ("/DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0502 /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS")
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS ("/DLIBPTHREAD_DEBUG")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GS /RTCcsu")
    ENDIF()
ELSEIF (CMAKE_COMPILER_IS_GNUCC)
    SET (LIBPTHREAD_NAME "pthread")
    ADD_DEFINITIONS ("-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0502")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_DEFINITIONS ("/DLIBPTHREAD_DEBUG")
        SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wformat-security --coverage -fstack-protector-all")
        SET (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} "-Wl,-static")
    ELSE()
        SET (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} "-Wl,-static,--strip-all")
    ENDIF()
ENDIF()

INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/include")

# CMAKE_INSTALL_PREFIX
INSTALL (FILES "${PROJECT_SOURCE_DIR}/include/pthread.h"        DESTINATION include)
INSTALL (FILES "${PROJECT_SOURCE_DIR}/include/pthread_clock.h"  DESTINATION include)
INSTALL (FILES "${PROJECT_SOURCE_DIR}/include/pthread_types.h"  DESTINATION include)
INSTALL (FILES "${PROJECT_SOURCE_DIR}/include/sched.h"          DESTINATION include)
INSTALL (FILES "${PROJECT_SOURCE_DIR}/include/semaphore.h"      DESTINATION include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

# http://www.cmake.org/Wiki/CMake:CPackConfiguration
INCLUDE (InstallRequiredSystemLibraries)
SET (CPACK_PACKAGE_NAME libpthread)
SET (CPACK_PACKAGE_VERSION_MAJOR "${libpthread_VERSION_MAJOR}")
SET (CPACK_PACKAGE_VERSION_MINOR "${libpthread_VERSION_MINOR}")
SET (CPACK_PACKAGE_VERSION_PATCH "${libpthread_VERSION_PATCH}")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${libpthread_VERSION_MAJOR}-${libpthread_VERSION_MINOR}")
INCLUDE (CPack)
