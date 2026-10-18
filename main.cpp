#include <iostream>
#include <array>
#include <cmath>
#include "rd.h"

int main(int argc, char* argv[]) {
    // initialize
    // preprocess trace
    // analyze trace
    // merage trace

    int global_length = 8;
    int local_length = 4;

    int *trace = new int[global_length] { 0, 1, 2, 0, 1, 3, 2, 0 };
    int *dist = new int[global_length];

    int chunk_length = std::ceil((float)global_length / local_length);

    mrda_chunk *chunks = new mrda_chunk[chunk_length];

    mrda_world world = {
        .global_length = global_length,
        .local_length = local_length,
        .chunk_length = chunk_length,
        .trace = trace,
        .dist = dist,
        .chunks = chunks
    };

    mrda_calculate_rd(world);

    for (int i = 0; i < global_length; i++) {
        std::cout << dist[i] << " ";
    }

    std::cout << "\n";

    delete[] trace;
    delete[] dist;
    delete[] chunks;

    return 0;
}
