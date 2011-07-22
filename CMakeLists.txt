#
# http://www.cmake.org/Wiki/CMake
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
# LDFLAGS="-m64 -s -static" \
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
# signtool sign /t http://timestamp.verisign.com/scripts/timstamp.dll libpthread.dll
#

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

PROJECT(Libpthread)
SET(Libpthread_VERSION_MAJOR 0)
SET(Libpthread_VERSION_MINOR 1)
SET(Libpthread_VERSION_PATCH 0)

if (MSVC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
    ADD_DEFINITIONS("/DWIN32_LEAN_AND_MEAN /D_WIN32_WINNT=0x0500 /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS")
ELSEIF(CMAKE_COMPILER_IS_GNUCC)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        # perform coverage test by gcov
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -Wall -Wextra -Wformat-security -fprofile-arcs -ftest-coverage")
    ELSE()
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -Wall")
    ENDIF()

    ADD_DEFINITIONS("-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0500")
ENDIF()

# INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

# ADD_SUBDIRECTORY(src)
# ADD_SUBDIRECTORY(test)

# http://www.cmake.org/Wiki/CMake_Testing_With_CTest
INCLUDE(CTest)
ENABLE_TESTING()
# ADD_TEST(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/SimpleTest Hello)

# CMAKE_INSTALL_PREFIX
# INSTALL(FILES "${PROJECT_BINARY_DIR}/include/bkpublic.h" DESTINATION include)

# http://www.cmake.org/Wiki/CMake:CPackConfiguration
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_NAME Libpthread)
SET(CPACK_PACKAGE_VERSION_MAJOR "${Libpthread_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${Libpthread_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${Libpthread_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${Libpthread_VERSION_MAJOR}-${Libpthread_VERSION_MINOR}")
INCLUDE(CPack)
