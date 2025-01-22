#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_COMMANDS 10

typedef struct {
    char name[64];
    int health;
    int damage;
    int coins_reward;
    float effect_chance;
} Enemy;

typedef struct {
    char name[64];
    int cost;
    int unlocked;
} Command;

Command available_commands[MAX_COMMANDS];
int command_count = 0;
int player_health = 100;
int player_coins = 0;
int player_frozen = 0;    // Состояние "заморожен"
int player_burning = 0;   // Состояние "горит"

// Инициализация доступных команд
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

    command_count = 3;
}

// Генерация случайного врага
Enemy generate_enemy(char *location) {
    Enemy enemy;
    int type = rand() % 3; // 0: Миньон, 1: Элементаль, 2: Обычный монстр

    if (type == 0) {
        strcpy(enemy.name, "Миньон");
        enemy.health = 20 + rand() % 10;
        enemy.damage = 5 + rand() % 5;
        enemy.coins_reward = 5 + rand() % 5;
        enemy.effect_chance = strcmp(location, "Ледяная") == 0 ? 0.2 : strcmp(location, "Огненная") == 0 ? 0.1 : 0.05;
    } else if (type == 1) {
        strcpy(enemy.name, "Элементаль");
        enemy.health = 25 + rand() % 10;
        enemy.damage = 10 + rand() % 5;
        enemy.coins_reward = 10 + rand() % 5;
        enemy.effect_chance = strcmp(location, "Ледяная") == 0 ? 0.5 : strcmp(location, "Огненная") == 0 ? 0.3 : 0.1;
    } else {
        strcpy(enemy.name, "Обычный монстр");
        enemy.health = 30 + rand() % 10;
        enemy.damage = 7 + rand() % 7;
        enemy.coins_reward = 8 + rand() % 7;
        enemy.effect_chance = strcmp(location, "Ледяная") == 0 ? 0.35 : strcmp(location, "Огненная") == 0 ? 0.2 : 0.07;
    }

    return enemy;
}

// Покупка команды
void buy_command(char *command) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(available_commands[i].name, command) == 0) {
            if (available_commands[i].unlocked) {
                printf("Команда %s уже разблокирована!\n", command);
                return;
            }
            if (player_coins >= available_commands[i].cost) {
                player_coins -= available_commands[i].cost;
                available_commands[i].unlocked = 1;
                printf("Команда %s разблокирована!\n", command);
            } else {
                printf("Недостаточно монет для разблокировки команды %s!\n", command);
            }
            return;
        }
    }
    printf("Команда %s не существует!\n", command);
}

// Сражение с врагом
void fight_enemy(Enemy enemy, char *location) {
    printf("Вы встретили врага: %s (Здоровье: %d, Урон: %d)!\n", enemy.name, enemy.health, enemy.damage);

    while (enemy.health > 0 && player_health > 0) {
        if (player_frozen) {
            printf("Вы заморожены и пропускаете ход!\n");
            player_frozen = 0; // Заморозка снимается после одного хода
        } else {
            printf("Ваше здоровье: %d\n", player_health);
            printf("Здоровье врага: %d\n", enemy.health);
            printf("1. Атаковать\n");
            printf("2. Убежать\n");
            printf("Выберите действие: ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                int damage = 10 + rand() % 5;
                printf("Вы нанесли %d урона врагу!\n", damage);
                enemy.health -= damage;

                if (strcmp(location, "Огненная") == 0 && rand() % 100 < (int)(enemy.effect_chance * 100)) {
                    player_burning = 5 + rand() % 5; // Игрок "горит"
                    printf("Вы горите! В следующий ход получите %d урона.\n", player_burning);
                }
            } else if (choice == 2) {
                printf("Вы сбежали из боя!\n");
                return;
            } else {
                printf("Неверный выбор!\n");
            }
        }

        // Атака врага
        if (enemy.health > 0) {
            player_health -= enemy.damage;
            printf("Враг атаковал вас и нанёс %d урона!\n", enemy.damage);

            if (strcmp(location, "Ледяная") == 0 && rand() % 100 < (int)(enemy.effect_chance * 100)) {
                player_frozen = 1; // Игрок "заморожен"
                printf("Вы заморожены и пропустите следующий ход!\n");
            }
        }

        // Урон от горения
        if (player_burning > 0) {
            printf("Вы получили %d урона от горения!\n", player_burning);
            player_health -= player_burning;
            player_burning = 0; // Эффект горения снимается
        }
    }

    if (player_health > 0) {
        printf("Вы победили врага и получили %d монет!\n", enemy.coins_reward);
        player_coins += enemy.coins_reward;
    } else {
        printf("Вы погибли! Игра окончена.\n");
        exit(0);
    }
}

// Основная функция игры
void dungeon_crawler() {
    char *locations[] = {"Ледяная", "Огненная", "Пустынная"};
    while (player_health > 0) {
        printf("Выберите уровень подземелья:\n");
        printf("1. Ледяная\n2. Огненная\n3. Пустынная\n");
        printf("Ваш выбор: ");
        int choice;
        scanf("%d", &choice);

        if (choice < 1 || choice > 3) {
            printf("Неверный выбор!\n");
            continue;
        }

        Enemy enemy = generate_enemy(locations[choice - 1]);
        fight_enemy(enemy, locations[choice - 1]);
    }
}

int main() {
    srand(time(NULL));
    initialize_commands();

    printf("Добро пожаловать в текстовую игру Dungeon Crawler!\n");

    dungeon_crawler();
    return 0;
}
