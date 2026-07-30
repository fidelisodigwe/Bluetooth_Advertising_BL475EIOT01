#include "mbed.h"

#include "ble/BLE.h"
#include "ble_process.h"
#include "stm32l475e_iot01.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_tsensor.h"
#include <cstdint>
#include <events/mbed_events.h>

class Lab3Server : ble :: GattServer::EventHandler{
public:
   Lab3Server(){}
   ~Lab3Server(){}

    void start(BLE &ble, events::EventQueue &event_queue){
      const UUID uuid = GattService::UUID_ENVIRONMENTAL_SERVICE;
      GattCharacteristic *charTable[] = {};
      GattService sensorService(uuid, charTable,sizeof(charTable) / sizeof(charTable[0]));
      ble.gattServer().addService(sensorService);
      ble.gattServer().setEventHandler(this);
      printf("Service started.\n");

    }

};

class Lab3ServerProcess : public BLEProcess {
public:
  Lab3ServerProcess(events::EventQueue &event_queue, BLE &ble_interface)
   : BLEProcess(event_queue, ble_interface) {}
   const char *get_device_name() override {
       static const char name[] = "fidel's bluetooth";
       return name;
   }

};
static EventQueue event_queue(10 * EVENTS_EVENT_SIZE);

int main(){
  BLE &ble = BLE:: Instance();
  printf("Lab 3 - BLE");

  Lab3ServerProcess ble_process(event_queue, ble);
  Lab3Server server;
  ble_process.on_init(callback(&server, &Lab3Server::start));
  ble_process.start();
  return 0;

}