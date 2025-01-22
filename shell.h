// shell.h
#ifndef SHELL_H
#define SHELL_H

#include "game.h"

// Функции для работы с командной оболочкой
void execute_command(char *command, Player *player);
void start_game_command(Player *player);
void unlock_command_shell(Player *player, const char *command);
void print_logo();

#endif // SHELL_H
