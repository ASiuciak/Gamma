#include "player.h"

Player new_Player() {
    Player p;
    p.areas = 0;
    p.occupied = 0;
    p.adjacent_free = 0;
    p.golden = true;
    return p;
}

