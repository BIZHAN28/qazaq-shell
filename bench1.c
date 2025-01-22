#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define FILE_PATH "test_file.bin"
#define BLOCK_SIZE 1024

void random_write(int iterations) {
    int fd = open(FILE_PATH, O_CREAT | O_WRONLY | O_SYNC, 0644);
    if (fd < 0) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        buffer[i] = rand() % 256;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < iterations; i++) {
        lseek(fd, (rand() % iterations) * BLOCK_SIZE, SEEK_SET);
        if (write(fd, buffer, BLOCK_SIZE) != BLOCK_SIZE) {
            perror("Ошибка записи");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 +
                          (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("Время выполнения: %.2f мс\n", elapsed_time);

    close(fd);
    remove(FILE_PATH);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество итераций>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int iterations = atoi(argv[1]);
    random_write(iterations);
    return EXIT_SUCCESS;
}


