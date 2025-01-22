#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Enemy {
    char name[50];
    int health;
    int damage;
    int money;
    int effect_chance;  // Шанс на эффект (заморозка, горение, побег)
} Enemy;

Enemy* spawn_enemy(int level) {
    Enemy *enemy = malloc(sizeof(Enemy));

    // Простой выбор врага по уровню
    if (level == 1) {
        strcpy(enemy->name, "Миньон");
        enemy->health = 10;
        enemy->damage = 5;
        enemy->money = 1;
        enemy->effect_chance = 10;
    } else if (level == 2) {
        strcpy(enemy->name, "Элементаль");
        enemy->health = 20;
        enemy->damage = 8;
        enemy->money = 5;
        enemy->effect_chance = 20;
    } else {
        strcpy(enemy->name, "Обычный монстр");
        enemy->health = 30;
        enemy->damage = 10;
        enemy->money = 10;
        enemy->effect_chance = 30;
    }

    return enemy;
}
