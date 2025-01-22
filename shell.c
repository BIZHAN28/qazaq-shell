#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include "game.h"

// Максимальная длина ввода
#define MAX_INPUT 1024
#define MAX_ARGS 64

// Структура для хранения информации о разблокированных командах
typedef struct {
    char *command;
    int unlocked;  // 0 - заблокирована, 1 - разблокирована
} Command;

// Массив команд, которые нужно разблокировать
Command commands[] = {
    {"ls", 0},
    {"pwd", 0},
    {"echo", 0},
    {"cat", 0},
    {NULL, 0}  // Конец списка команд
};

// Функция для выполнения команды
void execute_command(char *command) {
    // Поиск в списке команд
    for (int i = 0; commands[i].command != NULL; i++) {
        if (strcmp(command, commands[i].command) == 0) {
            if (commands[i].unlocked == 0) {
                printf("Команда %s заблокирована! Запустите игру для разблокировки.\n", command);
                start_game();  // Запуск игры для разблокировки
                commands[i].unlocked = 1;  // Разблокировать команду
                printf("Команда %s теперь разблокирована!\n", command);
                return;
            }
            break;
        }
    }

    // Разделяем команду на аргументы
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (arg_count == 0) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс
        execvp(args[0], args);
        perror("Ошибка при выполнении команды");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("Ошибка при создании процесса");
    }
}

int main() {
    char input[MAX_INPUT];
    FILE *file = fopen("logo.txt", "r");
    char line[256];

    // Вывод логотипа
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);

    while (1) {
        printf("shell> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        // Удаление символа новой строки
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute_command(input);  // Обработка команды
    }

    return 0;
}
