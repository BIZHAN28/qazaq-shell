#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

// Функция для обработки кастомных команд || Кастомды пәрмендерді өңдеу функциясы
int handle_custom_command(char *command, char *args[]) {
    if (strcmp(args[0], "alga") == 0) {
        char musicCommand[256];
        char imgCommand[256];
        snprintf(musicCommand, sizeof(musicCommand), "vlc --intf dummy --play-and-exit %s &", "./music/masterpiece.ogg");
        snprintf(imgCommand, sizeof(imgCommand), "eog %s", "./img/flag.png");
        system(musicCommand);
        system(imgCommand);
        return 1; // Команда обработана || Пәрмен орындалды
    }

    if (strcmp(args[0], "help") == 0) {
        printf("Доступные команды:\n");
        printf("  alga   - Воспроизводит музыку и показывает флаг. || Музыка ойнап, жалау көрсетеді.\n");
        printf("  help   - Показывает это сообщение. || Бұл хабарламаны көрсетеді.\n");
        printf("  exit   - Выход из оболочки. || Шеллден шығу.\n");
        return 1; // Команда обработана || Пәрмен орындалды
    }

    // Добавьте другие кастомные команды здесь || Мұнда басқа кастомды пәрмендерді қосыңыз
    return 0; // Команда не обработана || Пәрмен орындалмады
}

void execute_command(char *command) {
    char *args[MAX_ARGS];
    int arg_count = 0;

    // Разделение строки на аргументы || Жолды аргументтерге бөлу
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (arg_count == 0) {
        return; // Пустая строка || Бос жол
    }

    // Обработка кастомных команд || Кастомды пәрмендерді өңдеу
    if (handle_custom_command(command, args)) {
        return; // Если кастомная команда была обработана, выходим || Егер кастомды пәрмен орындалса, шығамыз
    }

    // Измерение времени выполнения || Орындалу уақытын өлшеу
    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс || Бала процесс
        execvp(args[0], args);
        perror("Ошибка выполнения команды || Пәрменді орындау қатесі");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс || Ата-ана процесс
        int status;
        waitpid(pid, &status, 0);

        gettimeofday(&end, NULL);
        double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 +
                              (end.tv_usec - start.tv_usec) / 1000.0;
        printf("Время выполнения: %.2f мс || Орындалу уақыты: %.2f мс\n", elapsed_time);
    } else {
        perror("Ошибка создания процесса || Процесті құру қатесі");
    }
}

int main() {
    char input[MAX_INPUT];

    // Печать ASCII-логотипа из файла || Файлдан ASCII логотипін басып шығару
    FILE *file = fopen("logo.txt", "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        perror("Не удалось открыть logo.txt || logo.txt файлын ашу мүмкін болмады");
    }

    while (1) {
        printf("kz_shell> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        // Удаляем символ новой строки || Жаңа жол таңбасын жою
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute_command(input);
    }

    return 0;
}
