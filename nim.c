#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>

typedef struct
{
    char name[7];
    int color;
} Player;

int RandomPlayer(int stick_number)
{
    if (stick_number < 3)
    {
        return rand() % stick_number + 1;
    }
    return rand() % 3 + 1;
}

int IAPlayer(int stick_number)
{
    if (stick_number % 4 == 0)
    {
        return 1;
    }
    return stick_number % 4;
}

void draw(int stick_number, int removed, Player player)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    system("clear");

    int XPos = sz.ws_col / 2 - 2 - strlen(player.name) / 2;
    printf("\033[1;%dH\33[%dmJoueur %s\33[0m", XPos, player.color, player.name);

    for (size_t i = 0; i < 2; i++)
    {
        int XPos = sz.ws_col / 2 - (stick_number + removed) * 3 / 2 + 1;
        int YPos = sz.ws_row / 2 + i;
        printf("\033[%d;%dH", YPos, XPos);
        for (size_t i = 0; i < stick_number; i++)
        {
            printf(" | ");
        }
        printf("\33[90m");
        for (size_t i = 0; i < removed; i++)
        {
            printf(" | ");
        }
        printf("\33[0m\n");
    }
}

void end(Player player)
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);

    system("clear");

    int XPos = sz.ws_col / 2 - 9 - strlen(player.name) / 2;
    int YPos = sz.ws_row / 2 + 1;
    int LastYPos = sz.ws_row - 2;
    printf("\033[%d;%dHLe \33[%dmjoueur %s\33[0m a gagné !\033[%d;1H", YPos, XPos, player.color, player.name, LastYPos);
}

int main()
{
    srand(time(NULL));

    int stick_number = 20;

    Player iaPlayer = {.color = 91, .name = "IA"};
    Player randomPlayer = {.color = 94, .name = "Random"};

    char firstPlayer[7];
    while (strcmp(firstPlayer, "IA") && strcmp(firstPlayer, "Random"))
    {
        printf("Choisir le premier joueur (IA, Random): ");
        scanf("%6s", firstPlayer);
    }

    // Hide cursor
    printf("\e[?25l");

    Player *actualPlayer = strcmp(firstPlayer, "IA") ? &iaPlayer : &randomPlayer;

    while (stick_number > 0)
    {
        int removed;
        if (strcmp(actualPlayer->name, "IA"))
        {
            actualPlayer = &iaPlayer;
            removed = IAPlayer(stick_number);
        }
        else
        {
            actualPlayer = &randomPlayer;
            removed = RandomPlayer(stick_number);
        }

        stick_number -= removed;

        draw(stick_number, removed, *actualPlayer);
        sleep(1);
    }

    end(*actualPlayer);

    // Show cursor
    printf("\e[?25h");

    return EXIT_SUCCESS;
}