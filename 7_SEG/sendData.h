#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <HTTPClient.h>
#include "parameters.h"

#define ENDPOINT_API "watchy-data"
#define ENDPOINT_STEPS "steps"

class SendData {
    public:
        static void pushAPIData(String json);
};

#endif