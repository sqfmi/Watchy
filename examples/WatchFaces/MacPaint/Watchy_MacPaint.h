#ifndef WATCHY_MACPAINT_H
#define WATCHY_MACPAINT_H

#include <Watchy.h>
#include "macpaint.h"

class WatchyMacPaint : public WatchyExpanded{
    using WatchyExpanded::WatchyExpanded;
    public:
        void drawWatchFace();
};

#endif
