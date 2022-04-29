#include <cstdio>
#include <iostream>
#include "clean/clean.h"
#include "kokkos/kokkos_clean.h"


bool kokkos_on = true;

int main(int argc, char *argv[]) {
    if (!kokkos_on) {
        main_cpp();
    } else {
        main_kokkos(argc, argv);
    }
    return 0;
}


