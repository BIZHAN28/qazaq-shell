#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "enemy.h"

void battle(Player *player, Enemy *enemy) {
    printf("Бой с %s начинается!\n", enemy->name);
    while (player->health > 0 && enemy->health > 0) {
        // Игрок атакует
        int player_attack = rand() % player->damage + 1;
        enemy->health -= player_attack;
        printf("Вы нанесли %d урона %s. Осталось здоровья: %d\n", player_attack, enemy->name, enemy->health);

        if (enemy->health <= 0) {
            printf("Вы победили!\n");
            player->coins += enemy->money;
            printf("Вы получили %d монет.\n", enemy->money);
            break;
        }

        // Враг атакует
        int enemy_attack = rand() % enemy->damage + 1;
        player->health -= enemy_attack;
        printf("%s нанес вам %d урона. Осталось здоровья: %d\n", enemy->name, enemy_attack, player->health);

        // Применение эффекта (заморозка, горение, побег)
        apply_enemy_effect(player, enemy);
    }
}

void apply_enemy_effect(Player *player, Enemy *enemy) {
    int effect_chance = rand() % 100 + 1;
    if (effect_chance <= enemy->effect_chance) {
        // Применение эффекта
        int effect_type = rand() % 3;
        if (effect_type == 0) {
            printf("Враг заморозил вас! Пропускаете ход.\n");
            // Игрок пропускает ход
        } else if (effect_type == 1) {
            printf("Вы горите! Следующий ход будет болезненным.\n");
            // Игрок горит
        } else {
            printf("Враг пытается сбежать, но не получается!\n");
            // Враг пытается сбежать, но не может
        }
    }
}
