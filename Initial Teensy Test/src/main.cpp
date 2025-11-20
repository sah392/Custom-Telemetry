#include <Arduino.h>
#include <XBee.h>
#include <vector>
#include <IntervalTimer.h>

// Timing stuff
IntervalTimer txTimer;
volatile bool send = false;
unsigned long period_us = 100000; // 100 ms for 10 Hz

// XBee object
XBee xbee = XBee();


// Address information, specific to XBee module
uint32_t addrMSB = 0x0013A200;
uint32_t addrLSB = 0x424D7364;
XBeeAddress64 addr64 = XBeeAddress64(addrMSB, addrLSB);

// Test packet we will sending
std::vector<uint8_t> packet;

// Every time timer goes off, send packet
void txISR() {
  send = true;
}

// Assemble the test packet
void buildPacket() {

  packet.clear(); 

  for (int i = 0; i < 100; i++) {
  packet.push_back(i);
  }

}

// Send the test packet
void sendPacket() {

  // Create transmit request frame object
  ZBTxRequest txRequest(addr64, packet.data(), packet.size());

  xbee.send(txRequest); // Send the frame

}

void setup() {

  Serial1.begin(230400);
  xbee.setSerial(Serial1); // Tell library which serial port to use
  txTimer.begin(txISR, period_us); // Timing

}

void loop() {

  if (send) {
      send = false;
      buildPacket();
      sendPacket();
  }

}