#include "DGUS_Async.h"

#ifndef FRAME_TIMEOUT
#define FRAME_TIMEOUT 20
#endif

DGUS_Async::DGUS_Async(Stream& serial) : _serial(&serial)
{
}

#if DGUS_HAS_SOFTSERIAL
DGUS_Async::DGUS_Async(uint8_t rx, uint8_t tx)
{
  _softSerial = new SoftwareSerial(rx, tx);
  _serial = _softSerial;
}
#endif

void DGUS_Async::begin(long baud)
{
#if DGUS_HAS_SOFTSERIAL
  if (_softSerial) {
    _softSerial->begin(baud);
  }
#else
  (void)baud;
#endif
}

void DGUS_Async::update()
{
  process();

  if (available()) {
    if (_eventCallback) {
      _eventCallback(_address, _value);
    }

    if (_stableCallback) {
      _pendingAddr = _address;
      _pendingValue = _value;
      _lastEventTime = millis();
      _hasPending = true;
    }
  }

  if (_stableCallback && _hasPending && (millis() - _lastEventTime >= _stableDelay)) {
    _stableCallback(_pendingAddr, _pendingValue);
    _hasPending = false;
  }

  updateAnimations();
}

void DGUS_Async::onEvent(DGUS_EventCallback cb)
{
  _eventCallback = cb;
}

void DGUS_Async::onStableEvent(DGUS_EventCallback cb, uint16_t delayMs)
{
  _stableCallback = cb;
  _stableDelay = delayMs;
}

void DGUS_Async::attachAnimation(DGUS_Animation* anim)
{
  if (animationCount < MAX_ANIMATIONS) {
    animations[animationCount++] = anim;
  }
}

void DGUS_Async::updateAnimations()
{
  for (uint8_t i = 0; i < animationCount; i++) {
    if (animations[i]) {
      animations[i]->update();
    }
  }
}

bool DGUS_Async::enqueue(uint8_t* data, uint8_t len)
{
  uint8_t next = (uint8_t)((qHead + 1) % DGUS_QUEUE_SIZE);
  if (next == qTail) {
    return false;
  }

  memcpy(queue[qHead].data, data, len);
  queue[qHead].len = len;
  qHead = next;
  return true;
}

void DGUS_Async::process()
{
  if (qTail == qHead) {
    return;
  }

  DGUSFrame& f = queue[qTail];
  _serial->write(f.data, f.len);
  qTail = (uint8_t)((qTail + 1) % DGUS_QUEUE_SIZE);
}

bool DGUS_Async::available()
{
  while (_serial->available()) {
    uint8_t b = (uint8_t)_serial->read();
    lastByteTime = millis();

    if (index == 0) {
      if (b != 0x5A) continue;
    } else if (index == 1) {
      if (b != 0xA5) {
        index = 0;
        continue;
      }
    } else if (index == 2) {
      expectedLen = (uint8_t)(b + 3);
      if (expectedLen > BUFFER_SIZE) {
        index = 0;
        expectedLen = 0;
        continue;
      }
    }

    if (index < BUFFER_SIZE) {
      buffer[index++] = b;
    } else {
      index = 0;
      expectedLen = 0;
      continue;
    }

    if (expectedLen && index >= expectedLen) {
      bool ok = (buffer[0] == 0x5A && buffer[1] == 0xA5 && buffer[3] == 0x83 && expectedLen >= 9);
      if (ok) {
        _address = (uint16_t)((buffer[4] << 8) | buffer[5]);
        _value   = (uint16_t)((buffer[7] << 8) | buffer[8]);
      }
      index = 0;
      expectedLen = 0;
      if (ok) return true;
    }
  }

  if (index && (millis() - lastByteTime > FRAME_TIMEOUT)) {
    index = 0;
    expectedLen = 0;
  }
  return false;
}

uint16_t DGUS_Async::getAddress()
{
  return _address;
}

uint16_t DGUS_Async::getValue()
{
  return _value;
}

void DGUS_Async::write(uint16_t address, uint16_t value)
{
  uint8_t frame[8] = {
    0x5A, 0xA5, 0x05, 0x82,
    (uint8_t)(address >> 8),
    (uint8_t)address,
    (uint8_t)(value >> 8),
    (uint8_t)value
  };
  enqueue(frame, 8);
}

void DGUS_Async::read(uint16_t address)
{
  uint8_t frame[7] = {
    0x5A, 0xA5, 0x04, 0x83,
    (uint8_t)(address >> 8),
    (uint8_t)address,
    0x01
  };
  enqueue(frame, 7);
}

void DGUS_Async::beginMulti(uint16_t address)
{
  multiAddress = address;
  multiCount = 0;
}

void DGUS_Async::add(uint16_t value)
{
  if (multiCount < MULTI_BUFFER) {
    multiData[multiCount++] = value;
  }
}

void DGUS_Async::add(uint8_t high, uint8_t low)
{
  if (multiCount < MULTI_BUFFER) {
    multiData[multiCount++] = (uint16_t)(((uint16_t)high << 8) | low);
  }
}

void DGUS_Async::sendMulti()
{
  if (!multiCount) return;

  const uint8_t len = (uint8_t)(3 + (multiCount * 2));
  uint8_t frame[80];

  frame[0] = 0x5A;
  frame[1] = 0xA5;
  frame[2] = len;
  frame[3] = 0x82;
  frame[4] = (uint8_t)(multiAddress >> 8);
  frame[5] = (uint8_t)multiAddress;

  uint8_t idx = 6;
  for (uint8_t i = 0; i < multiCount; i++) {
    frame[idx++] = (uint8_t)(multiData[i] >> 8);
    frame[idx++] = (uint8_t)multiData[i];
  }

  enqueue(frame, (uint8_t)(len + 3));
  multiCount = 0;
}

void DGUS_Async::setPage(uint16_t page)
{
  uint8_t frame[10] = {
    0x5A, 0xA5, 0x07, 0x82,
    0x00, 0x84, 0x5A, 0x01,
    (uint8_t)(page >> 8),
    (uint8_t)(page & 0xFF)
  };
  enqueue(frame, 10);
}

void DGUS_Async::getPage()
{
  read(0x0014);
}

void DGUS_Async::setBrightness(uint8_t brightness)
{
  if (brightness > 100) brightness = 100;

  uint8_t frame[7] = {
    0x5A, 0xA5, 0x04, 0x82,
    0x00, 0x82, brightness
  };
  enqueue(frame, 7);
}

void DGUS_Async::getBrightness()
{
  read(0x0082);
}

// --- Animation Implementation ---

DGUS_Animation::DGUS_Animation(DGUS_Async* dgus, uint16_t vpAddr, uint16_t startFrame, uint16_t endFrame, unsigned long frameInterval)
{
  _dgus = dgus;
  _vpAddr = vpAddr;
  _start = startFrame;
  _end = endFrame;
  _interval = frameInterval;
  _loop = false;
  _playing = false;
  _current = _start;
  _lastFrameTime = 0;
}

void DGUS_Animation::start(bool loop, int startFrame)
{
  _loop = loop;
  _playing = true;
  _current = (startFrame >= (int)_start && startFrame <= (int)_end) ? (uint16_t)startFrame : _start;
  _lastFrameTime = millis();
  _dgus->write(_vpAddr, _current);
}

uint16_t DGUS_Animation::stop()
{
  _playing = false;
  return _current;
}

void DGUS_Animation::hide()
{
  _playing = false;
  _dgus->write(_vpAddr, 0);
}

void DGUS_Animation::update()
{
  if (!_playing) return;

  unsigned long now = millis();
  if (now - _lastFrameTime < _interval) return;

  _lastFrameTime = now;
  _current++;

  if (_current > _end) {
    if (_loop) {
      _current = _start;
    } else {
      _playing = false;
      return;
    }
  }

  _dgus->write(_vpAddr, _current);
}

void DGUS_Animation::setSpeed(uint16_t interval)
{
  _interval = interval;
}

uint16_t DGUS_Animation::getSpeed()
{
  return _interval;
}
