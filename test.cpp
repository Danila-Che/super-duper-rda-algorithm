#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdint.h>
#include "rd.h"

// test cases:
// - preprocessing
// - analyzing
//   - fragmets table
//   - hash table
//   - local array
// - meraging
// - MPI
// - CUDA

TEST(preprocessing, pre_process) {
    std::array<int, 6> trace = { 0, 1, 2, 2, 2, 0  };
    std::array<int, 4> expected_trace = { 0, 1, 2, 0 };
    
    int length = mdra_pre_process(trace.data(), 6);

    ASSERT_EQ(length, 4);
    EXPECT_THAT(expected_trace, ::testing::ElementsAreArray(trace.data(), length));
}

TEST(analyzing, calculate_node) {
    std::array<int, 4> trace = { 0, 1, 2, 0 };
    // addr | first | latest
    // 0    | 0     | 0
    // 1    | 1     | 2
    // 2    | 2     | 1
}

TEST(analyzing, can_processed_when_all_unprocessed) {
    std::array<mrda_chunk, 2> gst; 
    gst[0] = { .status = UNPROCESSED, .owner = 0 };
    gst[1] = { .status = UNPROCESSED, .owner = 1 };

    EXPECT_TRUE(mrda_can_processed(gst.data(), 2));
}

TEST(analyzing, can_processed_when_at_least_one_unprocessed) {
    std::array<mrda_chunk, 2> gst; 
    gst[0] = { .status = DONE, .owner = 0 };
    gst[1] = { .status = UNPROCESSED, .owner = 1 };

    EXPECT_TRUE(mrda_can_processed(gst.data(), 2));
}

TEST(analyzing, can_processed_when_all_done) {
    std::array<mrda_chunk, 2> gst;
    gst[0] = { .status = DONE, .owner = 0 };
    gst[1] = { .status = DONE, .owner = 1 };

    EXPECT_FALSE(mrda_can_processed(gst.data(), 2));
}

TEST(analyzing, find_unprocessed_chunk_when_all_unprocessed) {
    std::array<mrda_chunk, 2> gst;
    gst[0] = { .status = UNPROCESSED, .owner = 0 };
    gst[1] = { .status = UNPROCESSED, .owner = 1 };

    int32_t selected_chunk_index = mrda_find_unprocessed_chunk(gst.data(), 2, 0);

    EXPECT_EQ(selected_chunk_index, 0);
}

TEST(analyzing, find_unprocessed_chunk_when_at_least_one_unprocessed) {
    std::array<mrda_chunk, 2> gst;
    gst[0] = { .status = DONE, .owner = 0 };
    gst[1] = { .status = UNPROCESSED, .owner = 1 };

    int32_t selected_chunk_index = mrda_find_unprocessed_chunk(gst.data(), 2, 0);

    EXPECT_EQ(selected_chunk_index, 1);
}

TEST(analyzing, find_unprocessed_chunk_when_all_done) {
    std::array<mrda_chunk, 2> gst;
    gst[0] = { .status = DONE, .owner = 0 };
    gst[1] = { .status = DONE, .owner = 1 };

    int32_t selected_chunk_index = mrda_find_unprocessed_chunk(gst.data(), 2, 0);

    EXPECT_EQ(selected_chunk_index, -1);
}

TEST(analyzing, calculate_local_rd_case_0) {
    std::array<int, 4> trace = { 0, 1, 2, 0 };
    std::array<int, 4> ref_dist = { -1, -1, -1, 2 };
    std::array<int, 4> dist = { 0, 0, 0, 0 };

    mrda_calculate_local_rd(trace.data(), dist.data(), 0, 4);

    EXPECT_EQ(dist, ref_dist);
}

TEST(analyzing, calculate_local_rd_case_1) {
    std::array<int, 8> trace = { 0, 1, 2, 0, 1, 3, 2, 0 };
    std::array<int, 8> ref_dist = { -1, -1, -1, 2, 2, -1, 3, 3 };
    std::array<int, 8> dist = { 0, 0, 0, 0, 0, 0, 0, 0 };

    mrda_calculate_local_rd(trace.data(), dist.data(), 0, 8);

    EXPECT_EQ(dist, ref_dist);
}

TEST(analyzing, calculate_local_rd_case_2) {
    std::array<int, 8> trace = { 1, 3, 2, 0, 0, 1, 2, 0 };
    std::array<int, 8> ref_dist = { 0, 0, 0, 0, -1, -1, -1, 2 };
    std::array<int, 8> dist = { 0, 0, 0, 0, 0, 0, 0, 0 };

    mrda_calculate_local_rd(trace.data(), dist.data(), 4, 4);

    EXPECT_EQ(dist, ref_dist);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
