#include <Arduino.h>

// настройки таймаутов, мс
// #define UE_FAST_TIME 32  // время между тиками для быстрого поворота

#include <uEncButton.h>

uEncButton eb(2, 3, 4);

// режим опроса энка в прерывании
// void isr() {
//     eb.tickISR();
// }

void setup() {
    Serial.begin(115200);

    // режим опроса энка в прерывании
    // attachInterrupt(0, isr, CHANGE);
    // attachInterrupt(1, isr, CHANGE);
}

void loop() {
    eb.tick();

    // обработка поворота общая
    if (eb.turn()) {
        Serial.print("turn: dir ");
        Serial.print(eb.dir());
        Serial.print(", fast ");
        Serial.print(eb.fast());
        Serial.print(", hold ");
        Serial.print(eb.pressing());
        Serial.print(", clicks ");
        Serial.print(eb.getClicks());
        Serial.println();
    }

    // обработка поворота раздельная
    if (eb.left()) Serial.println("left");
    if (eb.right()) Serial.println("right");
    if (eb.leftH()) Serial.println("leftH");
    if (eb.rightH()) Serial.println("rightH");

    // кнопка
    if (eb.press()) Serial.println("press");
    if (eb.click()) Serial.println("click");
    if (eb.release()) Serial.println("release");

    // общий таймаут
    if (eb.timeout(1000)) Serial.println("timeout");
}