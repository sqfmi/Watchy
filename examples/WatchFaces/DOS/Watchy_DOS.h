#ifndef WATCHY_DOS_H
#define WATCHY_DOS_H

#include <Watchy.h>
#include "Px437_IBM_BIOS5pt7b.h"

class WatchyDOS : public Watchy{
    public:
        WatchyDOS();
        void drawWatchFace();
};

#endif