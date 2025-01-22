// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

// Структура игрока
typedef struct {
    char *name;
    int health;
    int money;
    int attack;
    int defense;
    LocationType current_location;  // Текущая локация игрока
} Player;

// Функции для работы с игроком
void init_player(Player *player, const char *name);
void show_player_status(const Player *player);
void player_attack(Player *player, Enemy *enemy);
void take_damage(Player *player, int damage);
void earn_money(Player *player, int amount);

#endif // PLAYER_H
