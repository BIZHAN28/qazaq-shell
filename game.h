// game.h
#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "enemy.h"

// Перечисление для типов локаций
typedef enum {
    ICE,    // Ледяная локация
    FIRE,   // Огненная локация
    DESERT  // Пустынная локация
} LocationType;

// Функции для игры
void start_game(Player *player);
void move_to_next_floor(Player *player);
void battle(Player *player, Enemy *enemy);
void unlock_command(Player *player, const char *command);
void show_player_status(const Player *player);

#endif // GAME_H
