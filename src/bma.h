#pragma once

#ifdef  ARDUINO
#include <Arduino.h>
#else
#include <stdlib.h>
#endif

#include "bma423.h"

enum {
    DIRECTION_TOP_EDGE        = 0,
    DIRECTION_BOTTOM_EDGE     = 1,
    DIRECTION_LEFT_EDGE       = 2,
    DIRECTION_RIGHT_EDGE      = 3,
    DIRECTION_DISP_UP         = 4,
    DIRECTION_DISP_DOWN       = 5
} ;

typedef struct bma4_accel Accel;
typedef struct bma4_accel_config Acfg;

class BMA423
{

public:
    BMA423();
    ~BMA423();

    bool begin(bma4_com_fptr_t readCallBlack, bma4_com_fptr_t writeCallBlack, bma4_delay_fptr_t delayCallBlack,
               uint8_t address = BMA4_I2C_ADDR_PRIMARY);

    void softReset();
    void shutDown();
    void wakeUp();
    bool selfTest();

    uint8_t getDirection();

    bool setAccelConfig(Acfg &cfg);
    bool getAccelConfig(Acfg &cfg);
    bool getAccel(Accel &acc);
    bool getAccelEnable();
    bool disableAccel();
    bool enableAccel(bool en = true);

    bool setINTPinConfig(struct bma4_int_pin_config config, uint8_t pinMap);
    bool getINT();
    uint8_t getIRQMASK();
    bool disableIRQ(uint16_t int_map = BMA423_STEP_CNTR_INT);
    bool enableIRQ(uint16_t int_map = BMA423_STEP_CNTR_INT);
    bool isStepCounter();
    bool isDoubleClick();
    bool isTilt();
    bool isActivity();
    bool isAnyNoMotion();

    bool resetStepCounter();
    uint32_t getCounter();

    float readTemperature();
    float readTemperatureF();

    uint16_t getErrorCode();
    uint16_t getStatus();
    uint32_t getSensorTime();


    const char *getActivity();
    bool setRemapAxes(struct bma423_axes_remap *remap_data);

    bool enableFeature(uint8_t feature, uint8_t enable );
    bool enableStepCountInterrupt(bool en = true);
    bool enableTiltInterrupt(bool en = true);
    bool enableWakeupInterrupt(bool en = true);
    bool enableAnyNoMotionInterrupt(bool en = true);
    bool enableActivityInterrupt(bool en = true);

private:
    bma4_com_fptr_t __readRegisterFptr;
    bma4_com_fptr_t __writeRegisterFptr;
    bma4_delay_fptr_t __delayCallBlackFptr;

    uint8_t __address;
    uint16_t __IRQ_MASK;
    bool __init;
    struct bma4_dev __devFptr;
};
