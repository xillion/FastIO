#ifndef __FAST_IO_H
#define __FAST_IO_H

#include "FastIO_LPC1768.h"
#include "FastIO_LPC11UXX.h"
#include "FastIO_LPC11U6X.h"
#include "FastIO_LPC81X.h"
#include "FastIO_KLXX.h"
#include "FastIO_K20D50M_KPSDK.h"
#include "FastIO_STM32F4.h"
#include "FastIO_STM32L073XX.h"
#include "FastIO_STM32L1.h"
#include "FastIO_STM32L4.h"
#include "FastIO_NUCLEO_F030.h"
#include "FastIO_LPC11XX.h"
#include "FastIO_EFM32.h"
#include "FastIO_LPC43XX.h"
#include "FastIO_NRF51822.h"

#ifndef INIT_PIN
#warning Target is not supported by FastIO
#warning Reverting to regular DigitalInOut
#include "FastIO_Unsupported.h"
#endif

#include "mbed.h"

/**
 * Faster alternative compared to regular DigitalInOut
 *
 * Except the constructor it is compatible with regular DigitalInOut.
 * Code is based on Igor Skochinsky's code (http://mbed.org/users/igorsk/code/FastIO/)
 */
class FastInOut
{
public:
    /**
     * Construct new FastInOut object
     *
     * @code
     * FastInOut<LED1> led1;
     * @endcode
     *
     * No initialization is done regarding input/output mode,
     * FastIn/FastOut can be used if that is required
     *
     * @param pin pin the FastOut object should be used for
     */
    FastInOut(PinName pin);
    
    ~FastInOut();

    void write(int value);
    int read();

    void mode(PinMode pull);

    void output();

    void input();

    FastInOut& operator= (int value) {
        write(value);
        return *this;
    };
    FastInOut& operator= (FastInOut& rhs) {
        write(rhs.read());
        return *this;
    };
    operator int() {
        return read();
    };
    
    protected:
    fastio_vars container;
    PinName _pin;
};

/**
 * Faster alternative compared to regular DigitalOut
 *
 * Except the constructor it is compatible with regular DigitalOut. Aditionally all
 * functions from DigitalInOut are also available (only initialization is different)
 * Code is based on Igor Skochinsky's code (http://mbed.org/users/igorsk/code/FastIO/)
 */
class FastOut : public FastInOut
{
public:
    /**
     * Construct new FastOut object
     *
     * @code
     * FastOut<LED1> led1;
     * @endcode
     *
     * @param pin pin the FastOut object should be used for
     * @param initial (optional) initial state of the pin after construction: default is 0 (low)
     */
    FastOut(PinName pin, int initial = 0);

    FastOut& operator= (int value) {
        this->write(value);
        return *this;
    };
    FastOut& operator= (FastOut& rhs) {
        this->write(rhs.read());
        return *this;
    };
    operator int() {
        return this->read();
    };
};

/**
 * Faster alternative compared to regular DigitalIn
 *
 * Except the constructor it is compatible with regular DigitalIn. Aditionally all
 * functions from DigitalInOut are also available (only initialization is different)
 * Code is based on Igor Skochinsky's code (http://mbed.org/users/igorsk/code/FastIO/)
 */
class FastIn : public FastInOut
{
public:
    /**
     * Construct new FastIn object
     *
     * @code
     * FastIn<LED1> led1;
     * @endcode
     *
     * @param pin pin the FastIn object should be used for
     * @param pinmode (optional) initial mode of the pin after construction: default is PullDefault
     */
    FastIn(PinName pin, PinMode pinmode = PullDefault);

    FastIn& operator= (int value) {
        this->write(value);
        return *this;
    };
    FastIn& operator= (FastIn& rhs) {
        this->write(rhs.read());
        return *this; 
    };
    operator int() {
        return this->read();
    };
};

/**
 * Faster alternative compared to regular BusIn
 *
 * Except the constructor it is compatible with regular DigitalIn. Aditionally all
 * functions from DigitalInOut are also available (only initialization is different)
 * Code is based on Igor Skochinsky's code (http://mbed.org/users/igorsk/code/FastIO/)
 */
class FastBusIn
{
public:
    /**
     * Construct new FastBusIn object
     *
     * @code
     * FastBusIn bus(PC_1, PC_3, PA_1);
     * @endcode
     *
     * @param pin pin the FastIn object should be used for
     * @param pinmode (optional) initial mode of the pin after construction: default is PullDefault
     */
    FastBusIn(PinName p0, PinName p1 = NC, PinName p2 = NC, PinName p3 = NC,
          PinName p4 = NC, PinName p5 = NC, PinName p6 = NC, PinName p7 = NC,
          PinName p8 = NC, PinName p9 = NC, PinName p10 = NC, PinName p11 = NC,
          PinName p12 = NC, PinName p13 = NC, PinName p14 = NC, PinName p15 = NC);

    FastBusIn(PinName pins[16]);

    ~FastBusIn();

    int read();

    void mode(PinMode pull);

    private:
    FastIn *_pin[16];
    int _nc_mask;

};

#endif