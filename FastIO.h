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
    FastInOut(PinName pin) : _pin(pin){
        INIT_PIN(_pin);
    }
    
    ~FastInOut() {
        DESTROY_PIN;
    }

    void write(int value) {
        if ( value )
            WRITE_PIN_SET(_pin);
        else
            WRITE_PIN_CLR(_pin);
    }
    int read() {
        return READ_PIN(_pin);
    }

    void mode(PinMode pull) {
        SET_MODE(_pin, pull);
    }

    void output() {
        SET_DIR_OUTPUT(_pin);
    }

    void input() {
        SET_DIR_INPUT(_pin);
    }

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
    FastOut(PinName pin, int initial = 0) : FastInOut(pin) {
        this->write(initial);
        SET_DIR_OUTPUT(pin);
    }

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
    FastIn(PinName pin, PinMode pinmode = PullDefault) : FastInOut(pin) {
        SET_MODE(pin, pinmode);
        SET_DIR_INPUT(pin);
    }

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
     * FastIn<LED1> led1;
     * @endcode
     *
     * @param pin pin the FastIn object should be used for
     * @param pinmode (optional) initial mode of the pin after construction: default is PullDefault
     */
    FastBusIn(PinName p0, PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName p7, PinName p8, PinName p9, PinName p10, PinName p11, PinName p12, PinName p13, PinName p14, PinName p15)
    {
        PinName pins[16] = {p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15};

        // No lock needed in the constructor
        _nc_mask = 0;
        for (int i = 0; i < 16; i++) {
            _pin[i] = (pins[i] != NC) ? new FastIn(pins[i]) : 0;
            if (pins[i] != NC) {
                _nc_mask |= (1 << i);
            }
        }
    }

    FastBusIn(PinName pins[16])
    {
        // No lock needed in the constructor
        _nc_mask = 0;
        for (int i = 0; i < 16; i++) {
            _pin[i] = (pins[i] != NC) ? new FastIn(pins[i]) : 0;
            if (pins[i] != NC) {
                _nc_mask |= (1 << i);
            }
        }
    }

    ~FastBusIn()
    {
        // No lock needed in the destructor
        for (int i = 0; i < 16; i++) {
            if (_pin[i] != 0) {
                delete _pin[i];
            }
        }
    }

    int read()
    {
        int v = 0;
        for (int i = 0; i < 16; i++) {
            if (_pin[i] != 0) {
                v |= _pin[i]->read() << i;
            }
        }
        return v;
    }

    void mode(PinMode pull)
    {
        for (int i = 0; i < 16; i++) {
            if (_pin[i] != 0) {
                _pin[i]->mode(pull);
            }
        }
    }

    private:
    FastIn *_pin[16];
    int _nc_mask;

};

#endif