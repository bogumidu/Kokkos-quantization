cmake_minimum_required(VERSION 3.21)
project(project2)

add_subdirectory(src)
add_subdirectory(build)
add_subdirectory(lib)
set(project2_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(project2_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)

set(CMAKE_CXX_STANDARD 17)

set(Kokkos_ENABLE_OPENMP 1)
set(Kokkos_OMP_PROC_BIND 1)
set(Kokkos_ENABLE_DEBUG 1)

add_executable(project2 src/main.cpp src/clean/clean.cpp src/clean/clean.h src/kokkos/kokkos_clean.cpp src/kokkos/kokkos_clean.h src/components/Location.cpp src/components/Location.h src/utils/FileMaker.cpp src/utils/FileMaker.h src/utils/Face.cpp src/utils/Face.h src/components/Voxel.cpp src/components/Voxel.h src/utils/TempFace.cpp src/utils/TempFace.h src/utils/VoxelStore.cpp src/utils/VoxelStore.h src/kokkos/task_execution_example.cpp src/kokkos/task_execution_example.h src/utils/ObjectStore.cpp src/utils/ObjectStore.h)

# kokkos library
add_subdirectory(CMAKE_CURRENT_SOURCE_DIR/../lib/kokkos)
target_link_libraries(project2 Kokkos::kokkos)
