#ifndef WATCHY_DOS_H
#define WATCHY_DOS_H

#include <Watchy.h>
#include "Px437_IBM_BIOS5pt7b.h"

class WatchyDOS : public WatchyExpanded{
    using WatchyExpanded::WatchyExpanded;
    public:
        void drawWatchFace();
};

#endif
