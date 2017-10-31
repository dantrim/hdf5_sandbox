cmake_minimum_required(VERSION 3.8)
project(nsw_hd5f_sandbox)

set(CMAKE_CXX_STANDARD 11)

find_package(HDF5 REQUIRED COMPONENTS CXX HL)

message("HDF5_FOUND = ${HDF5_FOUND}")
message("HDF5_INCLUDE_DIR = ${HDF5_INCLUDE_DIR}")
message("HDF5_INCLUDE_DIRS = ${HDF5_INCLUDE_DIRS}")
message("HDF5_VERSION = ${HDF5_VERSION}")

message("HDF5_CXX_DEFINITIONS = ${HDF5_CXX_DEFINITIONS}")
message("HDF5_CXX_LIBRARIES = ${HDF5_CXX_LIBRARIES}")
message("HDF5_CXX_HL_LIBRARIES = ${HDF5_CXX_HL_LIBRARIES}")


include_directories(./ ${HDF5_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS})

set(SOURCE_FILES
    # compound
    src/main_compound.cxx
)

set(SOURCE_FILES_INSPECT
    src/main_inspect.cxx
)

set(HEADER_FILES
)

add_executable(compound_test ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(compound_test ${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES})

add_executable(inspect_test ${SOURCE_FILES_INSPECT})
target_link_libraries(inspect_test ${HDF5_CXX_LIBRARIES} ${HDF5_CXX_HL_LIBRARIES})
