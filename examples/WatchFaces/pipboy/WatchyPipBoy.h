#ifndef WATCHY_PIP_BOY_H
#define WATCHY_PIP_BOY_H

#include <Watchy.h>
#include "monofonto28pt7b.h"
#include "monofonto10pt7b.h"
#include "monofonto8pt7b.h"
#include "monofonto6pt7b.h"
#include "img.h"
#include "icons.h"

class WatchyPipBoy : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
};

#endif
