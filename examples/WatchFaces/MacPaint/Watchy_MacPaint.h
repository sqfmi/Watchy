#ifndef WATCHY_MACPAINT_H
#define WATCHY_MACPAINT_H

#include <Watchy.h>
#include "macpaint.h"

class WatchyMacPaint : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
};

#endif