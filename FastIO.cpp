#include "FastIO.h"


FastInOut::FastInOut(PinName pin) : _pin(pin){
    INIT_PIN(_pin);
};

FastInOut::~FastInOut() {
    DESTROY_PIN;
};

void FastInOut::write(int value) {
    if ( value )
        WRITE_PIN_SET(_pin);
    else
        WRITE_PIN_CLR(_pin);
};
int FastInOut::read() {
    return READ_PIN(_pin);
};

void FastInOut::mode(PinMode pull) {
    SET_MODE(_pin, pull);
};

void FastInOut::output() {
    SET_DIR_OUTPUT(_pin);
};

void FastInOut::input() {
    SET_DIR_INPUT(_pin);
};



FastOut::FastOut(PinName pin, int initial) : FastInOut(pin) {
    this->write(initial);
    SET_DIR_OUTPUT(pin);
};


FastIn::FastIn(PinName pin, PinMode pinmode) : FastInOut(pin) {
    SET_MODE(pin, pinmode);
    SET_DIR_INPUT(pin);
};



FastBusIn::FastBusIn(PinName p0, PinName p1, PinName p2, PinName p3, PinName p4, PinName p5, PinName p6, PinName p7, PinName p8, PinName p9, PinName p10, PinName p11, PinName p12, PinName p13, PinName p14, PinName p15)
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
};

FastBusIn::FastBusIn(PinName pins[16])
{
    _nc_mask = 0;
    for (int i = 0; i < 16; i++) {
        _pin[i] = (pins[i] != NC) ? new FastIn(pins[i]) : 0;
        if (pins[i] != NC) {
            _nc_mask |= (1 << i);
        }
    }
};

FastBusIn::~FastBusIn()
{
    for (int i = 0; i < 16; i++) {
        if (_pin[i] != 0) {
            delete _pin[i];
        }
    }
};

int FastBusIn::read()
{
    int v = 0;
    for (int i = 0; i < 16; i++) {
        if (_pin[i] != 0) {
            v |= _pin[i]->read() << i;
        }
    }
    return v;
};

void FastBusIn::mode(PinMode pull)
{
    for (int i = 0; i < 16; i++) {
        if (_pin[i] != 0) {
            _pin[i]->mode(pull);
        }
    }
};