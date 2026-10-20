#include <iostream>
#include <array>
#include <cmath>
#include "rd.h"

int main(int argc, char* argv[]) {
    // initialize
    // preprocess trace
    // analyze trace
    // merage trace

    int trace_length = 16;
    int fragment_length = 4;

    int *trace = new int[trace_length] { 0, 1, 2, 0, 1, 3, 2, 0, 0, 1, 2, 0, 1, 3, 2, 0 };
    trace_length = mdra_pre_process(trace, trace_length);

    int *dist = new int[trace_length]; // -1 -1 -1 -1 2 -1 3 3 3 2 2 2 3 3 3

    mrda_run_reuse_distance_analysis(trace, dist, trace_length, fragment_length);

    for (int i = 0; i < trace_length; i++) {
        std::cout << dist[i] << " ";
    }

    std::cout << "\n";

    delete[] trace;
    delete[] dist;

    return 0;
}
