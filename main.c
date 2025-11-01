#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mpi.h"

#define DATA_SIZE 100
#define NUM_PROCESSES 5
#define ROOT_PROCESS 0

void print_vector(int* vector, int size, const char* mask, ...) {
    va_list args;
    va_start(args, mask);
    vprintf(mask, args);
    va_end(args);
    printf(": [");
    for (int i = 0; i < size; i++) {
        printf("%d", vector[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argc, char* argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != NUM_PROCESSES) {
        if (rank == ROOT_PROCESS) {
            fprintf(stderr, "Error: This program must be run with exactly %d processes.\n", NUM_PROCESSES);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int* global_data = NULL;
    int* gathered_data = NULL;
    const int chunk_size = DATA_SIZE / NUM_PROCESSES;
    int* local_data = (int*)malloc(chunk_size * sizeof(int));

    if (local_data == NULL) {
        fprintf(stderr, "[Process %d] Failed to allocate memory for local_data.\n", rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    if (rank == ROOT_PROCESS) {
        global_data = (int*)malloc(DATA_SIZE * sizeof(int));
        gathered_data = (int*)malloc(DATA_SIZE * sizeof(int));

        if (global_data == NULL || gathered_data == NULL) {
            fprintf(stderr, "[Process %d] Failed to allocate memory for global data.\n", ROOT_PROCESS);
            free(global_data);
            free(gathered_data);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        for (int i = 0; i < DATA_SIZE; i++) {
            global_data[i] = i + 1;
        }

        print_vector(global_data, DATA_SIZE, "[Process %d] Original Vector", ROOT_PROCESS);
    }

    MPI_Scatter(global_data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);
    print_vector(local_data, chunk_size, "[Process %d] Received Chunk", rank);

    for (int i = 0; i < chunk_size; i++) {
        local_data[i] = local_data[i] * local_data[i];
    }

    MPI_Gather(local_data, chunk_size, MPI_INT, gathered_data, chunk_size, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    if (rank == ROOT_PROCESS) {
        print_vector(gathered_data, DATA_SIZE, "[Process %d] Transformed Vector", ROOT_PROCESS);
        free(global_data);
        free(gathered_data);
    }

    free(local_data);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
