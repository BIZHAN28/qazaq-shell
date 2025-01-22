#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_COMMANDS 10

typedef struct {
    char name[64];
    int cost;       // Цена команды || Пәрменнің бағасы
    int unlocked;   // Флаг разблокировки || Ашылған күйдің жалауы
} Command;

Command available_commands[MAX_COMMANDS];
int command_count = 0;
int player_coins = 0; // Монеты игрока || Ойыншының тиындары

// Инициализация доступных команд || Қолжетімді пәрмендерді инициализациялау
void initialize_commands() {
    strcpy(available_commands[0].name, "ls");
    available_commands[0].cost = 10;
    available_commands[0].unlocked = 0;

    strcpy(available_commands[1].name, "cat");
    available_commands[1].cost = 15;
    available_commands[1].unlocked = 0;

    strcpy(available_commands[2].name, "echo");
    available_commands[2].cost = 5;
    available_commands[2].unlocked = 0;
    
    strcpy(available_commands[3].name, "alga");
    available_commands[3].cost = 500;
    available_commands[3].unlocked = 0;

    strcpy(available_commands[4].name, "goyda");
    available_commands[4].cost = 500;
    available_commands[4].unlocked = 1;
    command_count = 5; // Количество команд || Пәрмендер саны
}

// Проверка, разблокирована ли команда || Пәрменнің ашылғанын тексеру
int is_command_unlocked(char *command) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(available_commands[i].name, command) == 0) {
            return available_commands[i].unlocked;
        }
    }
    return 0;
}

// Покупка команды || Пәрменді сатып алу
void buy_command(char *command) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(available_commands[i].name, command) == 0) {
            if (available_commands[i].unlocked) {
                printf("Команда %s уже разблокирована! || %s пәрмені қазірдің өзінде ашылған!\n", command, command);
                return;
            }
            if (player_coins >= available_commands[i].cost) {
                player_coins -= available_commands[i].cost;
                available_commands[i].unlocked = 1;
                printf("Команда %s разблокирована! || %s пәрмені ашылды!\n", command, command);
            } else {
                printf("Недостаточно монет для разблокировки команды %s! || %s пәрменін ашуға тиын жеткіліксіз!\n", command, command);
            }
            return;
        }
    }
    printf("Команда %s не существует! || %s пәрмені жоқ!\n", command, command);
}

// Простая текстовая игра || Қарапайым мәтіндік ойын
void dungeon_crawler_game() {
    printf("Добро пожаловать в подземелье! || Зынданға қош келдіңіз!\n");
    printf("Вы встретили монстра! || Сіз құбыжықты кездестірдіңіз!\n");

    int monster_health = 20 + rand() % 10; // Здоровье монстра || Құбыжықтың денсаулығы
    int player_health = 30;
    int damage, monster_damage;

    while (monster_health > 0 && player_health > 0) {
        printf("Ваше здоровье: %d || Сіздің денсаулығыңыз: %d\n", player_health, player_health);
        printf("Здоровье монстра: %d || Құбыжықтың денсаулығы: %d\n", monster_health, monster_health);

        printf("1. Атаковать || Шабуыл жасау\n");
        printf("2. Убежать || Қашу\n");
        printf("Выберите действие: || Әрекетті таңдаңыз: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            damage = 5 + rand() % 10;
            monster_damage = 3 + rand() % 5;

            monster_health -= damage;
            player_health -= monster_damage;

            printf("Вы нанесли %d урона монстру! || Сіз құбыжыққа %d зиян келтірдіңіз!\n", damage, damage);
            printf("Монстр атаковал вас и нанёс %d урона! || Құбыжық сізге шабуыл жасап, %d зиян келтірді!\n", monster_damage, monster_damage);
        } else if (choice == 2) {
            printf("Вы сбежали из боя! || Сіз ұрыс алаңынан қашып кеттіңіз!\n");
            return;
        } else {
            printf("Неверный выбор! || Қате таңдау!\n");
        }
    }

    if (player_health > 0) {
        printf("Вы победили монстра! || Сіз құбыжықты жеңдіңіз!\n");
        int coins_reward = 10 + rand() % 10;
        player_coins += coins_reward;
        printf("Вы получили %d монет! || Сіз %d тиын алдыңыз!\n", coins_reward, coins_reward);
    } else {
        printf("Вы проиграли! || Сіз жеңілдіңіз!\n");
    }
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

    // Обработка специальных команд || Арнайы пәрмендерді өңдеу
    if (strcmp(args[0], "game") == 0) {
        dungeon_crawler_game();
        return;
    } else if (strcmp(args[0], "buy") == 0 && arg_count > 1) {
        buy_command(args[1]);
        return;
    }

    // Проверка, разблокирована ли команда || Пәрменнің ашылғанын тексеру
    if (!is_command_unlocked(args[0])) {
        printf("Команда %s заблокирована! Заработайте монеты в игре, чтобы её разблокировать. || %s пәрмені бұғатталған! Оны ашу үшін ойында тиын жинаңыз.\n", args[0], args[0]);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс || Бала процесс
	if (strcmp(args[0], "alga") == 0) {
          char musicCommand[256];
          char imgCommand[256];
          snprintf(musicCommand, sizeof(musicCommand), "vlc --intf dummy --play-and-exit %s &", "./music/masterpiece.ogg");
          snprintf(imgCommand, sizeof(imgCommand), "eog %s", "./img/flag.png");
          system(musicCommand);
          system(imgCommand);
        } else if (strcmp(args[0], "goyda") == 0) {
          char videoCommand[256];
          snprintf(videoCommand, sizeof(videoCommand), "vlc --play-and-exit %s &", "./video/goyda.mp4");
          system(videoCommand);
        }
        execvp(args[0], args);
        perror("Ошибка выполнения команды || Пәрменді орындау қатесі");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс || Ата-ана процесс
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("Ошибка создания процесса || Процесті құру қатесі");
    }
}

int main() {
    srand(time(NULL));
    initialize_commands();

    char input[MAX_INPUT];
    printf("Добро пожаловать в kz_shell! || kz_shell-ге қош келдіңіз!\n");
    printf("Все команды заблокированы. Играйте в игру 'game', чтобы разблокировать их! || Барлық пәрмендер бұғатталған. Оларды ашу үшін 'game' ойынын ойнаңыз!\n");

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
