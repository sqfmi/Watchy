#include "bma.h"

#define DEBUGPORT Serial
#ifdef DEBUGPORT
#define DEBUG(...)      DEBUGPORT.printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

BMA423::BMA423()
{
    __readRegisterFptr = nullptr;
    __writeRegisterFptr = nullptr;
    __delayCallBlackFptr = nullptr;
    __init = false;
}

BMA423::~BMA423()
{

}

bool BMA423::begin(bma4_com_fptr_t readCallBlack,
                   bma4_com_fptr_t writeCallBlack,
                   bma4_delay_fptr_t delayCallBlack,
                   uint8_t address)
{

    if (__init ||
            readCallBlack == nullptr ||
            writeCallBlack == nullptr ||
            delayCallBlack == nullptr) {
        return true;
    }

    __readRegisterFptr = readCallBlack;
    __writeRegisterFptr = writeCallBlack;
    __delayCallBlackFptr = delayCallBlack;

    __devFptr.dev_addr        = address;
    __devFptr.interface       = BMA4_I2C_INTERFACE;
    __devFptr.bus_read        = readCallBlack;
    __devFptr.bus_write       = writeCallBlack;
    __devFptr.delay           = delayCallBlack;
    __devFptr.read_write_len  = 8;
    __devFptr.resolution      = 12;
    __devFptr.feature_len     = BMA423_FEATURE_SIZE;

    softReset();

    __delayCallBlackFptr(20);

    if (bma423_init(&__devFptr) != BMA4_OK) {
        DEBUG("BMA423 FAIL\n");
        return false;
    }

    if (bma423_write_config_file(&__devFptr) != BMA4_OK) {
        DEBUG("BMA423 Write Config FAIL\n");
        return false;
    }

    __init = true;

    struct bma4_int_pin_config config ;
    config.edge_ctrl = BMA4_LEVEL_TRIGGER;
    config.lvl = BMA4_ACTIVE_HIGH;
    config.od = BMA4_PUSH_PULL;
    config.output_en = BMA4_OUTPUT_ENABLE;
    config.input_en = BMA4_INPUT_DISABLE;


    if (bma4_set_int_pin_config(&config, BMA4_INTR1_MAP, &__devFptr) != BMA4_OK) {
        DEBUG("BMA423 SET INT FAIL\n");
        return false;
    }
    return true;
}

void BMA423::softReset()
{
    uint8_t reg = BMA4_RESET_ADDR;
    __writeRegisterFptr(BMA4_I2C_ADDR_PRIMARY, BMA4_RESET_SET_MASK, &reg, 1);
}

void BMA423::shutDown()
{
    bma4_set_advance_power_save(BMA4_DISABLE,  &__devFptr);
}

void BMA423::wakeUp()
{
    bma4_set_advance_power_save(BMA4_ENABLE, &__devFptr);
}

uint16_t BMA423::getErrorCode()
{
    struct bma4_err_reg err;
    uint16_t rslt = bma4_get_error_status(&err, &__devFptr);
    return rslt;
}

uint16_t BMA423::getStatus()
{
    uint8_t status;
    bma4_get_status(&status, &__devFptr);
    return status;
}

uint32_t BMA423::getSensorTime()
{
    uint32_t ms;
    bma4_get_sensor_time(&ms, &__devFptr);
    return ms;
}

bool BMA423::selfTest()
{
    return (BMA4_OK == bma4_selftest_config(BMA4_ACCEL_SELFTEST_ENABLE_MSK, &__devFptr));
}

uint8_t BMA423::getDirection()
{
    Accel acc;
    if (bma4_read_accel_xyz(&acc, &__devFptr) != BMA4_OK) {
        return 0;
    }
    uint16_t absX = abs(acc.x);
    uint16_t absY = abs(acc.y);
    uint16_t absZ = abs(acc.z);

    if ((absZ > absX) && (absZ > absY)) {
        if (acc.z > 0) {
            return  DIRECTION_DISP_DOWN;
        } else {
            return DIRECTION_DISP_UP;
        }
    } else if ((absY > absX) && (absY > absZ)) {
        if (acc.y > 0) {
            return DIRECTION_RIGHT_EDGE;
        } else {
            return  DIRECTION_LEFT_EDGE;
        }
    } else {
        if (acc.x < 0) {
            return  DIRECTION_BOTTOM_EDGE;
        } else {
            return DIRECTION_TOP_EDGE;
        }
    }
}

float BMA423::readTemperature()
{
    int32_t data = 0;
    bma4_get_temperature(&data, BMA4_DEG, &__devFptr);
    float res = (float)data / (float)BMA4_SCALE_TEMP;
    /* 0x80 - temp read from the register and 23 is the ambient temp added.
     * If the temp read from register is 0x80, it means no valid
     * information is available */
    if (((data - 23) / BMA4_SCALE_TEMP) == 0x80) {
        res = 0;
    }
    return res;
}


float BMA423::readTemperatureF()
{
    float temp = readTemperature();
    if (temp != 0) {
        temp = temp * 1.8 + 32.0;
    }
    return (temp);
}

bool BMA423::getAccel(Accel &acc)
{
    memset(&acc, 0, sizeof(acc));
    if (bma4_read_accel_xyz(&acc, &__devFptr) != BMA4_OK) {
        return false;
    }
    return true;
}

bool BMA423::getAccelEnable()
{
    uint8_t en;
    bma4_get_accel_enable(&en, &__devFptr);
    return (en & BMA4_ACCEL_ENABLE_POS) == BMA4_ACCEL_ENABLE_POS;
}

bool BMA423::disableAccel()
{
    return enableAccel(false);
}

bool BMA423::enableAccel(bool en)
{
    return (BMA4_OK == bma4_set_accel_enable(en ? BMA4_ENABLE : BMA4_DISABLE, &__devFptr));
}

bool BMA423::setAccelConfig(Acfg &cfg)
{
    return (BMA4_OK == bma4_set_accel_config(&cfg, &__devFptr));
}

bool BMA423::getAccelConfig(Acfg &cfg)
{
    return (BMA4_OK == bma4_get_accel_config(&cfg, &__devFptr));
}

bool BMA423::setRemapAxes(struct bma423_axes_remap *remap_data)
{
    return (BMA4_OK == bma423_set_remap_axes(remap_data, &__devFptr));
}

bool BMA423::resetStepCounter()
{
    return  BMA4_OK == bma423_reset_step_counter(&__devFptr) ;
}

uint32_t BMA423::getCounter()
{
    uint32_t stepCount;
    if (bma423_step_counter_output(&stepCount, &__devFptr) == BMA4_OK) {
        return stepCount;
    }
    return 0;
}

bool BMA423::setINTPinConfig(struct bma4_int_pin_config config, uint8_t pinMap)
{
    return BMA4_OK == bma4_set_int_pin_config(&config, pinMap, &__devFptr);
}

bool BMA423::getINT()
{
    return bma423_read_int_status(&__IRQ_MASK, &__devFptr) == BMA4_OK;
}

uint8_t BMA423::getIRQMASK()
{
    return __IRQ_MASK;
}

bool BMA423::disableIRQ(uint16_t int_map)
{
    return (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_DISABLE, &__devFptr));
}

bool BMA423::enableIRQ(uint16_t int_map)
{
    return (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_ENABLE, &__devFptr));
}

bool BMA423::enableFeature(uint8_t feature, uint8_t enable)
{
    if ((feature & BMA423_STEP_CNTR) == BMA423_STEP_CNTR) {
        bma423_step_detector_enable(enable ? BMA4_ENABLE : BMA4_DISABLE, &__devFptr);
    }
    return (BMA4_OK == bma423_feature_enable(feature, enable, &__devFptr));
}

bool BMA423::isStepCounter()
{
    return (bool)(BMA423_STEP_CNTR_INT & __IRQ_MASK);
}

bool BMA423::isDoubleClick()
{
    return (bool)(BMA423_WAKEUP_INT & __IRQ_MASK);
}

bool BMA423::isTilt()
{
    return (bool)(BMA423_TILT_INT & __IRQ_MASK);
}

bool BMA423::isActivity()
{
    return (bool)(BMA423_ACTIVITY_INT & __IRQ_MASK);
}

bool BMA423::isAnyNoMotion()
{
    return (bool)(BMA423_ANY_NO_MOTION_INT & __IRQ_MASK);
}

bool BMA423::enableStepCountInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP,  BMA423_STEP_CNTR_INT, en, &__devFptr));
}

bool BMA423::enableTiltInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_TILT_INT, en, &__devFptr));
}

bool BMA423::enableWakeupInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_WAKEUP_INT, en, &__devFptr));
}

bool BMA423::enableAnyNoMotionInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_ANY_NO_MOTION_INT, en, &__devFptr));
}

bool BMA423::enableActivityInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_ACTIVITY_INT, en, &__devFptr));
}

const char *BMA423::getActivity()
{
    uint8_t activity;
    bma423_activity_output(&activity, &__devFptr);
    if (activity & BMA423_USER_STATIONARY) {
        return "BMA423_USER_STATIONARY";
    } else if (activity & BMA423_USER_WALKING) {
        return "BMA423_USER_WALKING";
    } else if (activity & BMA423_USER_RUNNING) {
        return "BMA423_USER_RUNNING";
    } else if (activity & BMA423_STATE_INVALID) {
        return "BMA423_STATE_INVALID";
    }
    return "None";
}