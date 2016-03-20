#ifndef _ACROBOT_CONFIG_H
#define _ACROBOT_CONFIG_H

#include "Utils.h"
#include <EEPROM.h>

#define CONFIG_NAME_LEN 7

namespace ACRobot {

template<class Data>
class Config: public PollingInterface
{
  public:
    typedef uint8_t name_array[CONFIG_NAME_LEN];
  private:
    struct Buffer {
      name_array name;
      uint8_t version;
      Data data;
    } buffer;
  public:
    Config(const char *name, uint8_t version, const Data &default_config);

    bool poll();
    bool poll(const Data &settings) {
      (*this)(settings);
      return poll();
    }

    const Data &operator() () const {
      return buffer.data;
    }
    void operator() (const Data &data) {
      _updated = true;
      buffer.data = data;
    }

    const name_array& name() const {
      return buffer.name;
    }
    const uint8_t& version() const {
      return buffer.version;
    }

  private:

    uint8_t _updated;
};

template<class Data>
Config<Data>::Config(const char *name, uint8_t version, const Data &default_config):
  _updated(false)
{
  bool already_exists = true;
  uint8_t name_len = strlen(name);
  for (uint8_t i = 0; i < CONFIG_NAME_LEN; i++)
  {
    byte sym = 0;
    if (i < name_len)
      sym = name[i];
    buffer.name[i] = sym;
    if (EEPROM.read(i) != sym)
      already_exists = false;
  }
  buffer.version = version;
  if (EEPROM.read(CONFIG_NAME_LEN) != version)
    already_exists = false;
  if (!already_exists) {
    buffer.data = default_config;
    uint8_t *pbuffer = (uint8_t *) &buffer;
    for (uint8_t i = 0; i < sizeof(buffer); i++)
      EEPROM.write(i, pbuffer[i]);
  } else {
    uint8_t *pbuffer = (uint8_t *) &buffer;
    for (uint8_t i = CONFIG_NAME_LEN + 1; i < sizeof(buffer); i++)
      pbuffer[i] = EEPROM.read(i);
  }
}

template<class Data>
bool Config<Data>::poll()
{
  if(_updated) {
    uint8_t *pbuffer = (uint8_t *) &buffer;
    for (uint8_t i = CONFIG_NAME_LEN + 1; i < sizeof(buffer); i++)
      EEPROM.write(i, pbuffer[i]);
    _updated = false;
    return true;
  }
  return false;
}

} // ACRobot namespace

#endif

