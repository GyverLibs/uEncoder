#pragma once
#include "uButton.h"
#include "uEncoder.h"

class uEncButton : public uButton, public uEncoder {
   public:
    uEncButton(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP) : uButton(btn, modeBtn), uEncoder(encA, encB, modeEnc) {}

    // опросить. Вернёт true при событии
    bool tick() {
        if (uEncoder::tick(pressing())) {
            pressing() ? skipToRelease() : skipToTimeout();
            return true;
        }
        return uButton::tick();
    }

    // опросить энкодер в прерывании
    void tickISR() {
        uEncoder::tickISR(pressing());
    }

   private:
};