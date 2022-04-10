#ifndef WATCHY_POKEMON_H
#define WATCHY_POKEMON_H

#include <Watchy.h>
#include "pokemon.h"

class WatchyPokemon : public WatchyExpanded{
    using WatchyExpanded::WatchyExpanded;
    public:
        void drawWatchFace();
};

#endif
