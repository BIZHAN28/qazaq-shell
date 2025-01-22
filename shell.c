#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

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
        return; 
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс || бала процесі
        if (strcmp(args[0], "alga") == 0) {
          char musicCommand[256];
          char imgCommand[256];
          snprintf(musicCommand, sizeof(musicCommand), "vlc --intf dummy --play-and-exit %s &", "./music/masterpiece.ogg");
          snprintf(imgCommand, sizeof(imgCommand), "eog %s", "./img/flag.png");
          system(musicCommand);
          system(imgCommand);
        }
        execvp(args[0], args);
        perror("Пәрменді орындау қатесі");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс || Ата-ана процесі
        int status;
        waitpid(pid, &status, 0);

        gettimeofday(&end, NULL);
        double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 +
                              (end.tv_usec - start.tv_usec) / 1000.0;
        printf("Жұмыс уақыты: %.2f мс\n", elapsed_time);
    } else {
        perror("Процесті құру қатесі");
    }
}

int main() {
    char input[MAX_INPUT];
    FILE *file = fopen("logo.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
      printf("%s", line);
    }
    fclose(file);
  
    while (1) {
        printf("kz_shell> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }
 
        // Удаляем символ новой строки || Жаңа жол таңбасын жойыңыз
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute_command(input);
    }

    return 0;
}
