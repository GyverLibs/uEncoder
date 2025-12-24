[![latest](https://img.shields.io/github/v/release/GyverLibs/uEncoder.svg?color=brightgreen)](https://github.com/GyverLibs/uEncoder/releases/latest/download/uEncoder.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/uEncoder.svg)](https://registry.platformio.org/libraries/gyverlibs/uEncoder)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/uEncoder?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# uEncoder
Очередной класс энкодера и энкодера с кнопкой (на базе [uButton](https://github.com/GyverLibs/uButton)) для Arduino.

По API это на 99% аналог [EncButton](https://github.com/GyverLibs/EncButton), но чуточку легче, в 1.5 раза быстрее опрашивается и написан в более читаемом конечно-автоматном стиле, а не на флагах.

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Зависимости
- [uButton](https://github.com/GyverLibs/uButton)
- [GyverIO](https://github.com/GyverLibs/GyverIO)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### Дефайны настроек
Объявлять до подключения библиотеки

```cpp
#define UE_FAST_TIME 32  // время между тиками для быстрого поворота
```

### Классы
#### uEncoderVirt
Класс виртуального энкодера без кнопки.

```cpp
// =============== НАСТРОЙКИ ===============

// типы энкодеров
enum class Type {
    Step4Low,   // активный низкий сигнал (подтяжка к VCC). Полный период (4 фазы) за щелчок
    Step4High,  // активный высокий сигнал (подтяжка к GND). Полный период (4 фазы) за щелчок
    Step2,      // половина периода (2 фазы) за щелчок
    Step1,      // четверть периода (1 фаза) за щелчок
};

// установить тип энкодера (Type::Step4Low, Type::Step4High, Type::Step2, Type::Step1)
void setEncType(Type type);

// инвертировать направление энкодера
void setEncReverse(bool rev);

// инициализация энкодера
void initEnc(bool e0, bool e1);

// =============== СОСТОЯНИЕ ===============

enum class State {
    Idle,           // простаивает
    Left,           // поворот влево
    LeftFast,       // быстрый поворот влево
    LeftHold,       // нажатый поворот влево
    LeftHoldFast,   // быстрый нажатый поворот влево
    Right,          // поворот вправо
    RightFast,      // быстрый поворот вправо
    RightHold,      // нажатый поворот вправо
    RightHoldFast,  // быстрый нажатый поворот вправо
};
State getState();   // получить текущее состояние
void reset();       // сбросить состояние (принудительно закончить обработку)

// =============== СОБЫТИЯ ===============

// был поворот [событие]
uint8_t turn();

// направление энкодера (1 или -1) [состояние]
int8_t dir();

// направление энкодера (0 или 1) [состояние]
bool dirBool();

// нажатый поворот энкодера [событие]
bool turnH();

// быстрый поворот энкодера [состояние]
bool fast();

// поворот направо [событие]
bool right();

// поворот налево [событие]
bool left();

// нажатый поворот направо [событие]
bool rightH();

// нажатый поворот налево [событие]
bool leftH();

// нажата кнопка энкодера [состояние]
bool encHolding();

// =============== ОПРОС ===============

// опросить энкодер
bool tick(bool e0, bool e1, bool pressed = false);

// опросить энкодер без смены состояния
State poll(bool e0, bool e1, bool pressed = false);
```

#### uEncoder
Класс `uEncoder` наследует `uEncoderVirt`, отправляя данные с него в `poll` внутри `tick`.

```cpp
// указать пины и их режим работы
uEncoder(uint8_t p0, uint8_t p1, uint8_t mode = INPUT);

// опросить энкодер
bool tick(bool pressed = false);

// опросить энкодер в прерывании
void tickISR(bool pressed = false);
```

#### uEncButton
Класс `uEncButton` наследует `uEncoder` и `uButton`, опрашивает их. Доступны все события кнопки. При повороте энкодера при нажатой кнопке сбрасываются все события кнопки, кроме отпускания.

```cpp
uEncButton(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP);

// опросить. Вернёт true при событии
bool tick();

// опросить энкодер в прерывании
void tickISR();
```

## Примеры
```cpp

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
```

<a id="versions"></a>

## Версии
- v1.0

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **uEncoder** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/uEncoder/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код