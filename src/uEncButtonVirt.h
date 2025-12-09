#pragma once
#include "uButtonVirt.h"
#include "uEncoderVirt.h"

class uEncButtonVirt : public uButtonVirt, public uEncoderVirt {
   public:
    // опросить. Вернёт true при событии
    bool poll(bool e0, bool e1, bool btn) {
        if (uEncoderVirt::poll(e0, e1, pressing())) {
            pressing() ? skipToRelease() : skipToTimeout();
            return true;
        }
        return uButtonVirt::poll(btn);
    }

   private:
    using uButtonVirt::poll;
    using uButtonVirt::pollRaw;
    using uEncoderVirt::poll;
    using uEncoderVirt::pollRaw;
};