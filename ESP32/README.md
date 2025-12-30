- Arduino IDE > Settings > Additional boards manager URLs > ```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```
- Select board: ```NodeMCU-32S``` 
- Library Manager > ```HomeSpan```
- (millisDelay) MenuBar > Sketch > Include Library > Add .ZIP Library > https://github.com/ansonhe97/millisDelay/archive/refs/heads/master.zip
- HomeSpan setup wifi as manuel on Serial Monitor or ```homeSpan.setWifiCredentials("AKB_2.4GHz","****");```
- HomeSpan factory reset for not discovery accessory at Apple Home, type ```F``` in Serial Monitor in Arduino IDE
- CH340 Driver [install to MacOS via homebrew](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver?tab=readme-ov-file#installation-with-homebrew-cask) and then select port ```/dev/cu.wchusbserial120 Serial Port (USB)``` on Arduino IDE

```
*** HomeSpan Commands ***

  s - print connection status
  i - print summary information about the HAP Database
  d - print the full HAP Accessory Attributes Database in JSON format
  m - print free heap memory

  W - configure WiFi Credentials and restart
  X - delete WiFi Credentials and restart
  S <code> - change the HomeKit Pairing Setup Code to <code>
  Q <id> - change the HomeKit Setup ID for QR Codes to <id>
  O - change the OTA password
  A - start the HomeSpan Setup Access Point

  V - delete value settings for all saved Characteristics
  U - unpair device by deleting all Controller data
  H - delete HomeKit Device ID as well as all Controller data and restart

  P - output Pairing Data that can be saved offline to clone a new device
  C - clone Pairing Data previously saved offline from another device

  R - restart device
  F - factory reset and restart
  E - erase ALL stored data and restart

  L <level> - change the Log Level setting to <level>

  ? - print this list of commands

*** End Commands ***
```

![pinout](https://raw.githubusercontent.com/alikadir/iot/refs/heads/main/ESP32/pinout-esp32-c3.jpeg)
