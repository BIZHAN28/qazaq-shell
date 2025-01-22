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
#define MAX_HEALTH 100

// ANSI escape codes для цветного текста
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

typedef struct {
    char *name;
    int health;
    int damage;
    int coin_reward;
    int effect_chance;
} Monster;

// Глобальные переменные для игры
int player_health = MAX_HEALTH;
int player_coins = 0;
int game_running = 0;

// Функции игры
void print_welcome_message() {
    printf(GREEN "Добро пожаловать в подземелье!\n");
    printf("В вашем распоряжении три этажа: Ледяной, Огненный и Пустынный.\n");
    printf("Бой с монстрами принесет вам монеты, которые вы можете использовать для разблокировки команд!\n\n");
}

void print_game_status() {
    printf("\n" CYAN "Здоровье игрока: %d, Монеты: %d\n" RESET, player_health, player_coins);
}

void game_over() {
    printf(RED "\nВы погибли! Игра закончена.\n" RESET);
    game_running = 0;
}

void fight_monster(Monster monster) {
    printf(MAGENTA "Вы встретили монстра: %s! Бой начинается!\n" RESET, monster.name);
    printf("Здоровье монстра: %d, Урон: %d, Награда за убийство: %d монет.\n", monster.health, monster.damage, monster.coin_reward);

    // Сражение
    while (monster.health > 0 && player_health > 0) {
        printf("\nВаш ход: (1) Атака (2) Пропустить ход\n");
        char choice;
        scanf(" %c", &choice);

        if (choice == '1') {
            printf("Вы атаковали монстра!\n");
            monster.health -= rand() % 20 + 10;  // Урон от игрока
        }

        if (monster.health <= 0) {
            printf(GREEN "Монстр повержен! Вы получили %d монет.\n" RESET, monster.coin_reward);
            player_coins += monster.coin_reward;
            break;
        }

        // Ответный ход монстра
        if (rand() % 100 < monster.effect_chance) {
            if (strcmp(monster.name, "Ледяной Миньон") == 0) {
                printf(RED "Монстр замораживает вас! Вы пропустили ход.\n" RESET);
            } else if (strcmp(monster.name, "Огненный Элементаль") == 0) {
                printf(RED "Вы горите! Получите урон от огня.\n" RESET);
                player_health -= rand() % 10 + 5;
            } else if (strcmp(monster.name, "Пустынный Монстр") == 0) {
                printf(RED "Монстр убегает от вас!\n" RESET);
                break;
            }
        } else {
            printf("Монстр промахнулся!\n");
        }

        if (player_health <= 0) {
            game_over();
            break;
        }

        print_game_status();
    }
}

void start_game() {
    game_running = 1;
    srand(time(NULL));  // Инициализация генератора случайных чисел

    print_welcome_message();

    // Игровой цикл
    while (game_running) {
        printf("Вы находитесь в подземелье. Выберите этаж:\n");
        printf("1) Ледяной Этаж\n");
        printf("2) Огненный Этаж\n");
        printf("3) Пустынный Этаж\n");
        printf("4) Выход из игры\n");

        int choice;
        scanf("%d", &choice);

        if (choice == 4) {
            break;
        }

        Monster monster;
        switch (choice) {
            case 1:  // Ледяной этаж
                monster.name = "Ледяной Миньон";
                monster.health = rand() % 30 + 10;
                monster.damage = rand() % 10 + 5;
                monster.coin_reward = rand() % 10 + 5;
                monster.effect_chance = 20;  // 20% шанс заморозить
                break;
            case 2:  // Огненный этаж
                monster.name = "Огненный Элементаль";
                monster.health = rand() % 30 + 15;
                monster.damage = rand() % 15 + 5;
                monster.coin_reward = rand() % 15 + 10;
                monster.effect_chance = 30;  // 30% шанс горения
                break;
            case 3:  // Пустынный этаж
                monster.name = "Пустынный Монстр";
                monster.health = rand() % 40 + 20;
                monster.damage = rand() % 20 + 5;
                monster.coin_reward = rand() % 20 + 15;
                monster.effect_chance = 10;  // 10% шанс на убегание
                break;
            default:
                printf(RED "Неверный выбор!\n" RESET);
                continue;
        }

        fight_monster(monster);
    }
}

void execute_command(char *command) {
    char *args[MAX_ARGS];
    int arg_count = 0;

    // Разделение строки на аргументы
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;
    if (arg_count == 0) {
        return;
    }

    // Вызов игры
    if (strcmp(args[0], "start_game") == 0) {
        if (!game_running) {
            start_game();
        } else {
            printf(RED "Игра уже идет! Завершите текущую игру перед началом новой.\n" RESET);
        }
        return;
    }

    // Если команда не является игрой, выполняем обычную команду
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Ошибка выполнения команды");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("Ошибка создания процесса");
    }
}

int main() {
    char input[MAX_INPUT];
    FILE *file = fopen("logo.txt", "r");
    char line[256];

    // Вывод логотипа
    while (fgets(line, sizeof(line), file)) {
        printf(YELLOW "%s" RESET, line);
    }
    fclose(file);

    // Основной цикл командной оболочки
    while (1) {
        printf(CYAN "kz_shell> " RESET);
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        // Удаляем символ новой строки
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0) {
            break;
        }

        execute_command(input);
    }
    return 0;
}
