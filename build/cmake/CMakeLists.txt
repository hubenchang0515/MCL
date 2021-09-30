cmake_minimum_required ( VERSION   3.11 )

project ( MCL )

include(CheckLibraryExists)
include(FindPkgConfig)

find_package ( OpenCL REQUIRED )

set (CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -std=c++14" )

aux_source_directory (../../src SRC)

add_library( MCL SHARED ${SRC})

install(DIRECTORY ../../src/ DESTINATION include/MCL
        FILES_MATCHING PATTERN "*.h")

install(TARGETS MCL
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)
