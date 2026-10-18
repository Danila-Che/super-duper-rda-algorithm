#ifndef MY_RD_ALGORITHM
#define MY_RD_ALGORITHM

#include <string.h>
#include <stdint.h>

typedef enum {
    UNPROCESSED,
    PROCESSING,
    DONE
} mrda_chunk_status;

typedef struct {
    mrda_chunk_status status;
    int32_t owner;
} mrda_chunk;

typedef struct access_info {
    int32_t tid;
    int32_t first;
    int32_t latest;
    struct access_info *next;
} access_info;

typedef struct hash_node {
    int32_t addr;
    access_info *tinfo;
    struct hash_node *next;
} hash_node;

typedef struct {
    int32_t global_length; // trace length, dist length
    int32_t local_length; // local dist length
    int32_t chunk_length;
    int32_t *trace; // full trace
    int32_t *dist; // full reuse distance
    mrda_chunk *chunks;
} mrda_world;

int32_t mdra_pre_process(int32_t *trace, int32_t length) {
    int32_t result_length = 1;
    int32_t current_element = trace[0];

    for (int32_t i = 1; i < length; i++) {
        if (current_element != trace[i]) {
            current_element = trace[i];
            trace[result_length] = trace[i];
            result_length++;
        }
    }

    return result_length;
}

bool mrda_can_processed(mrda_chunk *chunks, int32_t length) {
    for (int32_t i = 0; i < length; i++) {
        if (chunks[i].status == UNPROCESSED) {
            return true;
        }
    }

    return false;
}

int32_t mrda_find_unprocessed_chunk(mrda_chunk *chunks, int32_t length, int32_t chunk_index) {
    for (int32_t i = 0; i < length; i++) {
        if (chunks[i].status == UNPROCESSED) {
            return i;
        }
    }

    return -1;
}

void mrda_calculate_local_rd(int32_t *trace, int32_t *dist, int32_t start, int32_t length) {
    for (int32_t i = start; i < start + length; i++) {
        int32_t current_element = trace[i];
        int32_t last_seen_index = -1;

        for (int32_t j = i - 1; j >= start; j--) {
            if (trace[j] == current_element) {
                last_seen_index = j;
                break;
            }
        }

        if (last_seen_index == -1) {
            dist[i] = -1;
            continue;
        }

        int32_t unique_count = 0;

        for (int32_t j = last_seen_index + 1; j < i; j++) {
            bool is_first_occurrence_in_gap = true;

            for (int32_t k = last_seen_index + 1; k < j; k++) {
                if (trace[k] == trace[j]) {
                    is_first_occurrence_in_gap = false;
                    break;
                }
            }

            if (is_first_occurrence_in_gap) {
                unique_count++;
            }
        }

        dist[i] = unique_count;
    }
}

void mrda_calculate_rd(mrda_world world) {
    int32_t owner = 0;    
    int32_t last_processed_chunk_index = -1;
        
    while (mrda_can_processed(world.chunks, world.chunk_length)) {
        int32_t chunk_index = mrda_find_unprocessed_chunk(
            world.chunks,
            world.chunk_length, last_processed_chunk_index);
        
        if (chunk_index == -1) {
            break;
        }

        world.chunks[chunk_index].status = PROCESSING;
        world.chunks[chunk_index].owner = owner;
        
        int32_t start = chunk_index * world.local_length;
        int32_t length = world.local_length;
        
        if (length > world.global_length - start) {
            length = world.global_length - start;
        }

        mrda_calculate_local_rd(world.trace, world.dist, start, length);

        world.chunks[chunk_index].status = DONE;
        last_processed_chunk_index = chunk_index;
    }
}

#endif // MY_RD_ALGORITHM
