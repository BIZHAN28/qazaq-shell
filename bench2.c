#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000

void sort_array(int iterations) {
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }

    struct timespec start, end;

    for (int i = 0; i < iterations; i++) {
        // Заполнение массива случайными числами
        for (int j = 0; j < ARRAY_SIZE; j++) {
            array[j] = rand();
        }

        clock_gettime(CLOCK_MONOTONIC, &start);

        // Сортировка массива
        for (int j = 0; j < ARRAY_SIZE - 1; j++) {
            for (int k = 0; k < ARRAY_SIZE - j - 1; k++) {
                if (array[k] > array[k + 1]) {
                    int temp = array[k];
                    array[k] = array[k + 1];
                    array[k + 1] = temp;
                }
            }
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 +
                              (end.tv_nsec - start.tv_nsec) / 1e6;
        printf("Итерация %d: %.2f мс\n", i + 1, elapsed_time);
    }

    free(array);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество итераций>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int iterations = atoi(argv[1]);
    sort_array(iterations);
    return EXIT_SUCCESS;
}



