# IoT Smart Light
The aim of the project was to design remotely controlled device which emits RGB light. The device is connected to Azure cloud and can be controlled via mobile app from anywhere in the world. RGB LED strip allows to emits light in 16.8 million colours. Thanks to light detection module brightness of the light can be set automatically.

## General information
* RGB LED Device
* Connection with cloud
* Control via mobile app
* Automatic brightness adjustment

## Technologies used
* C++
* Microsoft Azure
* IoT Hub
* Autodesk EAGLE
* ESP-32

## Schematic
![circuit](/Documentation/Circuit_diagram.png)
![board](/Documentation/Board_diagram.png)

## Components description
* ESP-32 - microcontroller
* X1(JP1) - Power module with DC 2.1/5.5mm socket
* LED_STRIP - LED RGB WS2812 5050
* R1 - 10k resistor
* PH1 - GL5539 50k-100k photoresistor

## Breadboard version
![breadboard_1](/Photos/DSC_0131.JPG)
![breadboard_2](/Photos/DSC_0134.JPG)

## PCB version
![pcb_1](/Photos/DSC_0150.JPG)
![pcb_2](/Photos/DSC_0152.JPG)
![pcb_3](/Photos/DSC_0154.JPG)

## Instruction
1. Create your device using `Circuit_diagram.pdf`.
2. Create account on [Microsoft Azure](https://azure.microsoft.com/en-gb/).
3. Create [IoT Hub](https://docs.microsoft.com/en-gb/azure/iot-hub/iot-hub-create-through-portal).
4. In IoT Hub add new device via `Devices > Add Device`.
5. Clone repository.
6. Open `AzIoTConfig.h` file.
* Enter Wi-Fi credentials, network name `IOT_CONFIG_WIFI_SSID` and password `IOT_CONFIG_WIFI_PASSWORD`.
* In IoT Hub open `Overview` to show hostname.
* Enter cloud hostname `IOT_CONFIG_IOTHUB_FQDN`.
* In IoT Hub open `Devices > Add Device > Your Device Name` to show cloud device credentials.
* Enter cloud device credentials, Device ID `IOT_CONFIG_DEVICE_ID` and Primary Key `IOT_CONFIG_DEVICE_KEY`.
7. Build and upload programme to ESP-32.
8. Use Smart Ligth app to controll your device.
