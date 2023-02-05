#ifndef WATCHY_MARIO_H
#define WATCHY_MARIO_H

#include <Watchy.h>
#include "mario.h"

class WatchyMario: public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif