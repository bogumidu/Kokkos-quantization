//
// Created by bogum on 10.04.2022.
//

#include "Kokkos_Core.hpp"
#include "kokkos_clean.h"

struct hello_world {

    KOKKOS_INLINE_FUNCTION
    void operator()(const int i) const {
#ifndef __SYCL_DEVICE_ONLY__
        printf("Hello from i = %i\n", i);
#else
        (void)i;
#endif
    }
};

int main_kokkos(int argc, char *argv[]) {
    Kokkos::initialize(argc, argv);

    printf("Hello World on Kokkos execution space %s\n",
           typeid(Kokkos::DefaultExecutionSpace).name());

    Kokkos::parallel_for("HelloWorld", 15, hello_world());

    Kokkos::finalize();

    return 0;
}