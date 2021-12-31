#ifndef WATCHY_POKEMON_H
#define WATCHY_POKEMON_H

#include <Watchy.h>
#include "pokemon.h"

class WatchyPokemon : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif