#include <iostream>
#include <cstring>
#include "execution/execution.h"

int main(int argc, char *argv[]) {

    // for storing add list wit push back
    // make it as a VerticesStore with method putVertex so no duplicates

//    int a = 5;
//    auto *ptr = &a;
//    std::cout << ptr << "\n";

    // run configuration
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << "\n";
    }

    // TODO: need to change compiler

    algorithm::main(argc, argv);

// Po włączeniu CUDA, kokkos wymaga NVCC.
// W instrukcji podane jest, że trzeba zmienić CMAKE_CXX_COMPILER na nvcc_wrapper z kokkosa.
// Ten wrapper jest skryptem bashowym i nie uruchamia się na windowsie.

// Czy teselacja powinna odbywać się przed voxelizacją w kokosie czy podczas

// Pierwsza wersja bez wielowatkowosci

// Porownania java -> cpp bez multithread
// porownanie cpp -> kokkos (team policy/openMP wg uznania)

// Najpierw zrobic pod windows nastyepnie adaptacja pod CUDA

    return 0;
}


