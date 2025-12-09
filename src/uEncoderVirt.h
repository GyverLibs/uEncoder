#pragma once
#include <Arduino.h>

#ifndef UE_FAST_TIME
#define UE_FAST_TIME 32  // время между тиками для быстрого поворота
#endif

#define UE_MS_SHIFT 4

#define UE_TURN_SH 0
#define UE_DIR_SH 1
#define UE_HOLD_SH 2
#define UE_FAST_SH 3

#define UE_TURN (1 << UE_TURN_SH)
#define UE_DIR (1 << UE_DIR_SH)
#define UE_HOLD (1 << UE_HOLD_SH)
#define UE_FAST (1 << UE_FAST_SH)

class uEncoderVirt {
   public:
    enum class Type : uint8_t {
        Step4Low,   // активный низкий сигнал (подтяжка к VCC). Полный период (4 фазы) за щелчок
        Step4High,  // активный высокий сигнал (подтяжка к GND). Полный период (4 фазы) за щелчок
        Step2,      // половина периода (2 фазы) за щелчок
        Step1,      // четверть периода (1 фаза) за щелчок
    };

    enum class State : uint8_t {
        Idle = 0,                               // простаивает
        Left = UE_TURN,                         // поворот влево
        LeftFast = Left | UE_FAST,              // быстрый поворот влево
        LeftHold = Left | UE_HOLD,              // нажатый поворот влево
        LeftHoldFast = LeftFast | LeftHold,     // быстрый нажатый поворот влево
        Right = UE_TURN | UE_DIR,               // поворот вправо
        RightFast = Right | UE_FAST,            // быстрый поворот вправо
        RightHold = Right | UE_HOLD,            // нажатый поворот вправо
        RightHoldFast = RightFast | RightHold,  // быстрый нажатый поворот вправо
    };

    uEncoderVirt() : _state(State::Idle), _type(Type::Step4Low) {}

    // установить тип энкодера (Type::Step4Low, Type::Step4High, Type::Step2, Type::Step1)
    void setEncType(Type type) {
        _type = type;
    }

    // инвертировать направление энкодера
    void setEncReverse(bool rev) {
        _rev = rev;
    }

    // инициализация энкодера
    void initEnc(bool e0, bool e1) {
        _e0 = e0, _e1 = e1;
    }

    // сбросить состояние (принудительно закончить обработку)
    void reset() {
        _state = State::Idle;
    }

    // ====================== GET ======================
    // получить текущее состояние
    State getState() {
        return _state;
    }

    // был поворот [событие]
    uint8_t turn() {
        // return uint8_t(_state);
        switch (_state) {
            case State::Idle: return false;
            default: return true;
        }
    }

    // направление энкодера (1 или -1) [состояние]
    int8_t dir() {
        return dirBool() ? 1 : -1;
    }

    // направление энкодера (0 или 1) [состояние]
    bool dirBool() {
        // return uint8_t(_state) & UE_DIR;
        switch (_state) {
            case State::Right:
            case State::RightFast:
            case State::RightHold:
            case State::RightHoldFast:
                return 1;
            default: return 0;
        }
    }

    // нажатый поворот энкодера [событие]
    bool turnH() {
        // return _eq(UE_TURN | UE_HOLD);
        switch (_state) {
            case State::RightHold:
            case State::RightHoldFast:
            case State::LeftHold:
            case State::LeftHoldFast:
                return true;
            default: return false;
        }
    }

    // быстрый поворот энкодера [состояние]
    bool fast() {
        // return uint8_t(_state) & UE_FAST;
        switch (_state) {
            case State::RightFast:
            case State::RightHoldFast:
            case State::LeftFast:
            case State::LeftHoldFast:
                return true;
            default: return false;
        }
    }

    // поворот направо [событие]
    bool right() {
        // return _eq(UE_TURN | UE_DIR | UE_HOLD, UE_TURN | UE_DIR);
        switch (_state) {
            case State::Right:
            case State::RightFast:
                return true;
            default: return false;
        }
    }

    // поворот налево [событие]
    bool left() {
        // return _eq(UE_TURN | UE_DIR | UE_HOLD, UE_TURN);
        switch (_state) {
            case State::Left:
            case State::LeftFast:
                return true;
            default: return false;
        }
    }

    // нажатый поворот направо [событие]
    bool rightH() {
        // return _eq(UE_TURN | UE_DIR | UE_HOLD, UE_TURN | UE_DIR | UE_HOLD);
        switch (_state) {
            case State::RightHold:
            case State::RightHoldFast:
                return true;
            default: return false;
        }
    }

    // нажатый поворот налево [событие]
    bool leftH() {
        // return _eq(UE_TURN | UE_DIR | UE_HOLD, UE_TURN | UE_HOLD);
        switch (_state) {
            case State::LeftHold:
            case State::LeftHoldFast:
                return true;
            default: return false;
        }
    }

    // нажата кнопка энкодера [состояние]
    bool encHolding() {
        // return uint8_t(_state) & UE_HOLD;
        switch (_state) {
            case State::RightHold:
            case State::RightHoldFast:
            case State::LeftHold:
            case State::LeftHoldFast:
                return true;
            default: return false;
        }
    }

    // опросить энкодер, вернёт true при повороте
    bool poll(bool e0, bool e1, bool pressed = false) {
        _state = pollRaw(e0, e1, pressed);
        return _state != State::Idle;
    }

    // опросить энкодер без смены состояния
    State pollRaw(bool e0, bool e1, bool pressed = false) {
        if (!(_e0 ^ _e1 ^ e0 ^ e1)) return State::Idle;

        (_e1 ^ e0) ? ++_pos : --_pos;
        _e0 = e0, _e1 = e1;
        if (!_pos) return State::Idle;

        switch (_type) {
            case Type::Step4Low:
                if (!(e0 & e1)) return State::Idle;  // skip 01, 10, 00
                break;
            case Type::Step4High:
                if (e0 | e1) return State::Idle;  // skip 01, 10, 11
                break;
            case Type::Step2:
                if (e0 ^ e1) return State::Idle;  // skip 10 01
                break;
            case Type::Step1:
                break;
        }

        uint8_t state = (1 << UE_TURN_SH) |
                        (((_pos < 0) ^ _rev) << UE_DIR_SH) |
                        (pressed << UE_HOLD_SH) |
                        ((uint8_t(uint8_t(millis() >> UE_MS_SHIFT) - _tmr) < (UE_FAST_TIME >> UE_MS_SHIFT)) << UE_FAST_SH);
        _pos = 0;
        _tmr = (millis() >> UE_MS_SHIFT);
        return State(state);
    }

   protected:
    State _state : 4;
    Type _type : 2;
    uint8_t _e0 : 1;
    uint8_t _e1 : 1;

    int8_t _pos : 4;
    int8_t _rev : 2;
    int8_t _isr : 2;

    uint8_t _tmr;

    // bool _eq(uint8_t mask, uint8_t val) {
    //     return (uint8_t(_state) & mask) == val;
    // }
    // bool _eq(uint8_t mask) {
    //     return _eq(mask, mask);
    // }
};
