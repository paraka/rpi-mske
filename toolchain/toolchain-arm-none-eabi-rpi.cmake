#
# cmake -DCMAKE_TOOLCHAIN_FILE=path_to set this file toolchain stuff
#

include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR BCM2835)
 
set(TC_PATH "/usr/bin/")

# The toolchain prefix for all toolchain executables
set(CROSS_COMPILE arm-none-eabi-)

CMAKE_FORCE_C_COMPILER(${TC_PATH}${CROSS_COMPILE}gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${TC_PATH}${CROSS_COMPILE}g++ GNU)

set(CMAKE_AR           "${TC_PATH}${CROSS_COMPILE}ar"      CACHE FILEPATH "ar")
set(CMAKE_LINKER       "${TC_PATH}${CROSS_COMPILE}ld"      CACHE FILEPATH "ld")
set(CMAKE_NM           "${TC_PATH}${CROSS_COMPILE}nm"      CACHE FILEPATH "nm")
set(CMAKE_OBJCOPY      "${TC_PATH}${CROSS_COMPILE}objcopy" CACHE FILEPATH "objcopy")
set(CMAKE_OBJDUMP      "${TC_PATH}${CROSS_COMPILE}objdump" CACHE FILEPATH "objdump")
set(CMAKE_STRIP        "${TC_PATH}${CROSS_COMPILE}strip"   CACHE FILEPATH "strip")
set(CMAKE_RANLIB       "${TC_PATH}${CROSS_COMPILE}ranlib"  CACHE FILEPATH "ranlib")

# Target environment
set(CMAKE_FIND_ROOT_PATH  "/usr/lib/arm-none-eabi")
 
# Do not search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
 
# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
