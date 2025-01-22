#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char name[50];
    int health;
    int damage;
    int coins;
} Player;

void init_player(Player *player) {
    strcpy(player->name, "Игрок");
    player->health = 100;
    player->damage = 10;
    player->coins = 0;
}
