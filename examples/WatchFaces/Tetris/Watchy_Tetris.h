#ifndef WATCHY_TETRIS_H
#define WATCHY_TETRIS_H

#include <Watchy.h>
#include "tetris.h"

class WatchyTetris : public WatchyExpanded{
    using WatchyExpanded::WatchyExpanded;
    public:
        WatchyTetris();
        void drawWatchFace();
};

#endif
