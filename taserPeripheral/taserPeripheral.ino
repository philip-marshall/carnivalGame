// Code by Philip Marshall - 8/23/2021
// BLE peripheral for carnival project. Connects to central device (raspberry pi) and advertises a relay characteristic that can be read and written.
// Once it is written with a bytearray of 1, it will activate a relay connected to the taser
// Code adapted from Protostax Arduino Nano BLE RGB LED Service found at: https://github.com/protostax/ProtoStax_Arduino_Nano_BLE_RGBLED_Service/blob/main/rgbBLE_Central.py
#include <ArduinoBLE.h>t

 #define RELAY A0  
 #define RHIGH 1
 #define RLOW 255

// While debugging (DEBUG = true), require serial monitor to be up before starting
 #define DEBUG false

BLEService nanoService("13012F00-F8C3-4F4A-A8F4-15CD926DA146"); // BLE Service

// BLE Characteristics - relay characteristic that is readable and writeable by the Central device (rasp pi)
BLEByteCharacteristic relayCharacteristic("13012F01-F8C3-4F4A-A8F4-15CD926DA146", BLEWrite | BLERead);

void setup() {
    Serial.begin(9600);
    
    if (DEBUG) {
      while(!Serial);
    }
    
    pinMode(RELAY, OUTPUT);

    digitalWrite(RELAY, RLOW); // start relay in off position

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (1);
    }

    // set advertised local name and service UUID:
    BLE.setLocalName("Arduino");
    BLE.setAdvertisedService(nanoService);

    // add the characteristic to the service
    nanoService.addCharacteristic(relayCharacteristic);

    // add service
    BLE.addService(nanoService);

    // set the initial value for the characeristic:
    relayCharacteristic.writeValue(0);

    // start advertising
    BLE.advertise();
    delay(100);
    Serial.println("Relay Peripheral stated.");
}

void loop() {
    // listen for BLE centrals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected()) {
            // if the remote device wrote to the characteristic,
            if (relayCharacteristic.written()) {
                Serial.println("written");
                if (relayCharacteristic.value()) {
                    Serial.println("RELAY on");
                    analogWrite(RELAY, RHIGH);
                    delay(1000);
                    analogWrite(RELAY, RLOW);
                }
            }
        }

        // when the central disconnects, print it out:
        Serial.print(F("Disconnected from central: "));
        Serial.println(central.address());
    }
}
