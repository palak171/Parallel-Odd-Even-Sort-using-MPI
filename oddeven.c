#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10  // Array size

void odd_even_sort(int *arr, int n, int rank, int size) {
    for (int phase = 0; phase < n; phase++) {
        int partner = (phase % 2 == 0) ? rank ^ 1 : rank ^ 0;
        if (partner >= 0 && partner < size) {
            int recv;
            MPI_Sendrecv(&arr[rank], 1, MPI_INT, partner, 0, &recv, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if ((rank < partner && arr[rank] > recv) || (rank > partner && arr[rank] < recv)) {
                arr[rank] = recv;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size, arr[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank);
    arr[rank] = rand() % 100;
    odd_even_sort(arr, N, rank, size);

    MPI_Finalize();
    return 0;
}
