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
#set(Kokkos_ENABLE_CUDA 1)
#set(CMAKE_CXX_COMPILER "${CMAKE_CURRENT_SOURCE_DIR}/lib/kokkos/bin/nvcc_wrapper")

add_executable(project2 src/main.cpp src/utils/fileMaker.cpp src/utils/fileMaker.h src/components/Face.cpp src/components/Face.h src/components/Voxel.cpp src/components/Voxel.h src/utils/VoxelStore.cpp src/utils/VoxelStore.h src/kokkos/task_execution_example.cpp src/kokkos/task_execution_example.h src/utils/ObjectStore.cpp src/utils/ObjectStore.h src/execution/execution.cpp src/execution/execution.h src/algorithm/algorithm.cpp src/algorithm/algorithm.h src/utils/VerticesStore.cpp src/utils/VerticesStore.h)

# kokkos library
add_subdirectory(CMAKE_CURRENT_SOURCE_DIR/../lib/kokkos)
target_link_libraries(project2 Kokkos::kokkos)
