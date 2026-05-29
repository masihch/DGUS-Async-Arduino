#ifndef DGUS_ASYNC_H
#define DGUS_ASYNC_H

#include <Arduino.h>

#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_STM32) || defined(ESP8266)
  #include <SoftwareSerial.h>
  #define DGUS_HAS_SOFTSERIAL 1
#else
  #define DGUS_HAS_SOFTSERIAL 0
#endif

class DGUS_Animation;

typedef void (*DGUS_EventCallback)(uint16_t addr, uint16_t value);

#ifndef DGUS_QUEUE_SIZE
#define DGUS_QUEUE_SIZE 20
#endif

class DGUS_Async
{
  public:
    DGUS_Async(Stream& serial);

#if DGUS_HAS_SOFTSERIAL
    DGUS_Async(uint8_t rx, uint8_t tx);
#endif

    void begin(long baud = 115200);
    void update();

    void onEvent(DGUS_EventCallback cb);
    void onStableEvent(DGUS_EventCallback cb, uint16_t delayMs = 150);

    bool available();
    uint16_t getAddress();
    uint16_t getValue();

    void write(uint16_t address, uint16_t value);
    void beginMulti(uint16_t address);
    void add(uint16_t value);
    void add(uint8_t high, uint8_t low);
    void sendMulti();

    void attachAnimation(DGUS_Animation *anim);
    void read(uint16_t address);
    void setPage(uint16_t page);
    void getPage();
    void setBrightness(uint8_t brightness);
    void getBrightness();

  private:
    Stream* _serial;

#if DGUS_HAS_SOFTSERIAL
    SoftwareSerial* _softSerial = nullptr;
#endif

    static const uint8_t BUFFER_SIZE = 32;
    uint8_t buffer[BUFFER_SIZE];
    uint8_t index = 0;
    uint8_t expectedLen = 0;
    unsigned long lastByteTime = 0;

    uint16_t _address = 0;
    uint16_t _value = 0;

    void process();

    DGUS_EventCallback _eventCallback = nullptr;

    DGUS_EventCallback _stableCallback = nullptr;
    uint16_t _stableDelay = 150;
    bool _hasPending = false;
    uint16_t _pendingAddr = 0;
    uint16_t _pendingValue = 0;
    unsigned long _lastEventTime = 0;

    struct DGUSFrame
    {
      uint8_t data[20];
      uint8_t len;
    };

    DGUSFrame queue[DGUS_QUEUE_SIZE];

    uint8_t qHead = 0;
    uint8_t qTail = 0;

    bool enqueue(uint8_t *data, uint8_t len);

    static const uint8_t MULTI_BUFFER = 32;
    uint16_t multiData[MULTI_BUFFER];
    uint8_t multiCount = 0;
    uint16_t multiAddress = 0;

    static const uint8_t MAX_ANIMATIONS = 10;
    DGUS_Animation* animations[MAX_ANIMATIONS];
    uint8_t animationCount = 0;

    void updateAnimations();
};

class DGUS_Animation
{
  public:
    DGUS_Animation(DGUS_Async *dgus,
                   uint16_t vpAddr,
                   uint16_t startFrame,
                   uint16_t endFrame,
                   unsigned long frameInterval);

    void start(bool loop = false, int startFrame = -1);
    uint16_t stop();
    void hide();
    void update();

    void setSpeed(uint16_t interval);
    uint16_t getSpeed();

    inline bool isPlaying() const { return _playing; }
    inline uint16_t currentFrame() const { return _current; }

  private:
    DGUS_Async *_dgus;
    uint16_t _vpAddr;
    uint16_t _start;
    uint16_t _end;
    uint16_t _current;
    bool _loop;
    bool _playing;
    unsigned long _interval;
    unsigned long _lastFrameTime;
};

#endif
