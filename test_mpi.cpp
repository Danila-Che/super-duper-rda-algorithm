#include <gtest/gtest.h>
#include <mpi.h>
#include "rd.h"

TEST(MPITestSuite, SummaryCheck) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank;
    int global_sum = 0;

    MPI_Allreduce(&local_value, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int expected_sum = size * (size - 1) / 2;

    EXPECT_EQ(global_sum, expected_sum);
}

TEST(MPITestSuite, status_synchronization) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> local_statuses(size, UNPROCESSED);
    std::vector<int> global_statuses(size, UNPROCESSED);

    local_statuses[rank] = PROCESSING;

    MPI_Allgather(&local_statuses[rank], 1, MPI_INT, global_statuses.data(), 1, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(global_statuses[i], PROCESSING);
    }

    local_statuses[rank] = DONE;

    MPI_Allgather(&local_statuses[rank], 1, MPI_INT, global_statuses.data(), 1, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < size; ++i) {
        EXPECT_EQ(global_statuses[i], DONE);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();

    MPI_Finalize();
    return result;
}
