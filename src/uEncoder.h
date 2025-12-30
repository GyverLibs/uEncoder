#pragma once
#include <GyverIO.h>

#include "uEncoderVirt.h"

class uEncoder : public uEncoderVirt {
   public:
    // указать пины и их режим работы
    uEncoder(uint8_t p0, uint8_t p1, uint8_t mode = INPUT) {
        _p0 = p0;
        _p1 = p1;
        gio::init(_p0, mode);
        gio::init(_p1, mode);
        initEnc(gio::read(_p0), gio::read(_p1));
    }

    // опросить энкодер
    bool tick(bool pressed = false) {
        if (_isr) _isr = false;
        else _state = _st(pollRaw(gio::read(_p0), gio::read(_p1), pressed));
        return _state != _st(State::Idle);
    }

    // опросить энкодер в прерывании
    void tickISR(bool pressed = false) {
        State state = pollRaw(gio::read(_p0), gio::read(_p1), pressed);
        if (state != State::Idle) {
            _state = _st(state);
            _isr = true;
        }
    }

   private:
    using uEncoderVirt::poll;
    using uEncoderVirt::pollRaw;

    uint8_t _p0, _p1;
};