#include <stdio.h>
#include <mpi.h>

void broadcast_and_send_recv();

int main(int argc, char* argv[]) {

    int my_rank, num_procs;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Init(&argc, &argv);

    broadcast_and_send_recv();

    MPI_Finalize();

    return 0;

}

void broadcast_and_send_recv() {
    int n;
    double result;

    int my_rank, num_procs;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Process 0 reads data for n
    if (my_rank == 0) {
        printf("Enter the number of elements (n): \n");
        scanf("%d", &n);
    }
    // buffer to be sent, size of buffer, type, root process, communicator
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Broadcasting to every process in MPI_COMM_WORLD

    // Doing some work
    result = 1.0 * my_rank * n;

    printf("I am process %i out of %i handling the %ith part of n=%i elements, result=%f\n", my_rank, num_procs, my_rank, n, result);

    if (my_rank != 0) {
        // Send results to process 0
        // buffer to be sent, size of buffer, type, destination, message tag, communicator
        MPI_Send(&result, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
    } else {
        int rank;
        printf("I'm process 0: My own result is %f\n", result);
        for (rank=1; rank < num_procs; rank++)
        {
            // Proc 0 receive results
            // Buffer to which recieved data goes, size of buffer, type, rank of process, tag, communicator, status object (ignored here)
            MPI_Recv(&result, 1, MPI_DOUBLE, rank, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("I'm process 0: recieved result of process %i is %f\n", rank, result);
        }
    }
}
