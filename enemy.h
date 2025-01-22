// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

// Структура врага
typedef struct {
    char *name;
    int health;
    int attack;
    int defense;
    int money;
    int effect_chance;  // Шанс на эффект (например, замораживание или огонь)
} Enemy;

// Функции для работы с врагами
void spawn_enemy(Enemy *enemy, LocationType location);
void enemy_attack(Enemy *enemy, Player *player);
void enemy_take_damage(Enemy *enemy, int damage);
bool is_enemy_dead(Enemy *enemy);

#endif // ENEMY_H
