// game.h
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Структура, описывающая характеристики врага
typedef struct {
    char *name;
    int health;
    int attack;
    int defense;
    int money;
    int effect_chance;  // Шанс на эффект (например, заморозка, огонь и т.д.)
} Enemy;

// Перечисление для типов локаций
typedef enum {
    ICE,    // Ледяная локация
    FIRE,   // Огненная локация
    DESERT  // Пустынная локация
} LocationType;

// Структура, описывающая игрока
typedef struct {
    char *name;
    int health;
    int money;
    int attack;
    int defense;
    LocationType current_location;  // Текущая локация игрока
} Player;

// Функции для игры
void start_game(Player *player);
void move_to_next_floor(Player *player);
void battle(Player *player, Enemy *enemy);
void unlock_command(Player *player, const char *command);
void show_player_status(const Player *player);

#endif // GAME_H
