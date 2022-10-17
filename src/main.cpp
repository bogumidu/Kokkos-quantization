#include <cstdio>
#include <iostream>
#include "execution/execution.h"

int main(int argc, char *argv[]) {
    int a = 5;
    auto* ptr = &a;
    std::cout << ptr << "\n";

    // TODO: need to change compiler

//    voxelization::main(argc, argv);

// Po włączeniu CUDA, kokkos wymaga NVCC.
// W instrukcji podane jest, że trzeba zmienić CMAKE_CXX_COMPILER na nvcc_wrapper z kokkosa.
// Ten wrapper jest skryptem bashowym i nie uruchamia się na windowsie.

// Czy teselacja powinna odbywać się przed voxelizacją w kokosie czy podczas

    return 0;
}


