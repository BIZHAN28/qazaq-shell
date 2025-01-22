#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "player.h"
#include "enemy.h"
#include "battle.h"

void start_game() {
    Player player;
    init_player(&player);

    int current_level = 1;
    while (player.health > 0) {
        printf("Вы на уровне %d\n", current_level);
        play_level(&player, current_level);
        current_level++;
        if (current_level > 3) {
            printf("Поздравляю, вы прошли все уровни!\n");
            break;
        }
    }

    if (player.health <= 0) {
        printf("Вы погибли. Игра окончена.\n");
    }
}

void play_level(Player *player, int level) {
    Enemy *enemy = spawn_enemy(level);
    printf("Вы встретили врага: %s\n", enemy->name);
    battle(player, enemy);
    free(enemy);
}
