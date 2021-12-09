#include "TelnetStream.h"

TelnetStreamClass::TelnetStreamClass(uint16_t port, Stream *defaultStream) :server(port) {
  this->defaultStream = defaultStream;
}

void TelnetStreamClass::begin() {
  server.begin();
  client = server.available();
}

void TelnetStreamClass::stop() {
  client.stop();
}

boolean TelnetStreamClass::disconnected() {
  if (!server)
    return true;

  if (!client) {
    client = server.available();
  }
  if (client) {
    if (client.connected())
      return false;
    client.stop();
    client = server.available();
  }
  return true;
}

int TelnetStreamClass::read() {
  if (disconnected())
    return defaultStream->read();
  return client.read();
}

int TelnetStreamClass::available() {
  if (disconnected())
    return defaultStream->available();
  return client.available();
}

int TelnetStreamClass::peek() {
  if (disconnected())
    return defaultStream->peek();
  return client.peek();
}

size_t TelnetStreamClass::write(uint8_t val) {
  if (disconnected())
    return defaultStream->write(val);
  return client.write(val);
}

void TelnetStreamClass::flush() {
  if (disconnected())
    return defaultStream->flush();
  client.flush();
}

