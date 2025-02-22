## PlatformIO 
https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start

## Steps of soldering on Li-on 
https://www.youtube.com/watch?v=dFoV_nLu7AA

## Raspberry Pi Nodejs GPIO
using npm library ```npm install onoff``` and read the issue https://github.com/fivdi/onoff/issues/197#issuecomment-1948884220

#### Manual GPIO management with sh
```
$ cat /sys/kernel/debug/gpio


gpiochip0: GPIOs 512-569, parent: platform/fe200000.gpio, pinctrl-bcm2711:
 gpio-512 (ID_SDA              )
 gpio-513 (ID_SCL              )
 gpio-514 (GPIO2               )
 gpio-515 (GPIO3               )
 gpio-516 (GPIO4               )
 gpio-517 (GPIO5               )
 gpio-518 (GPIO6               )
 gpio-519 (GPIO7               )
 gpio-520 (GPIO8               )
 gpio-521 (GPIO9               )
 gpio-522 (GPIO10              )
 gpio-523 (GPIO11              )
 gpio-524 (GPIO12              )
 gpio-525 (GPIO13              )
 gpio-526 (GPIO14              )
 gpio-527 (GPIO15              )
 gpio-528 (GPIO16              )
 gpio-529 (GPIO17              |sysfs               ) out hi 
 gpio-530 (GPIO18              |sysfs               ) out lo 
 gpio-531 (GPIO19              )
 gpio-532 (GPIO20              )
 gpio-533 (GPIO21              )
 gpio-534 (GPIO22              |sysfs               ) out lo 
 gpio-535 (GPIO23              )
 gpio-536 (GPIO24              )
 gpio-537 (GPIO25              )
...
```
512 GPIO System = 512 + 18gpioPin = GPIO-530

```
$ cd /sys/class/gpio
$ ls -la


--w--w----  1 root gpio 4096 Feb 22 23:14 export
lrwxrwxrwx  1 root root    0 Feb 22 23:14 gpio529 -> ../../devices/platform/soc/fe200000.gpio/gpiochip0/gpio/gpio529
lrwxrwxrwx  1 root root    0 Feb 22 21:47 gpio534 -> ../../devices/platform/soc/fe200000.gpio/gpiochip0/gpio/gpio534
lrwxrwxrwx  1 root gpio    0 Feb 21 02:17 gpiochip512 -> ../../devices/platform/soc/fe200000.gpio/gpio/gpiochip512
lrwxrwxrwx  1 root gpio    0 Feb 21 02:17 gpiochip570 -> ../../devices/platform/soc/soc:firmware/soc:firmware:gpio/gpio/gpiochip570
--w--w----  1 root gpio 4096 Feb 22 23:14 unexport
```

Pin marked using and then read value or write value from pin
```
$ echo 530 > /sys/class/gpio/export
```
Unusing, remove link
```
$ echo 530 > /sys/class/gpio/unexport
```

```
$ cd /sys/class/gpio/gpio530
$ ls -la


total 0
drwxrwxr-x 3 root gpio    0 Feb 23 01:56 .
drwxr-xr-x 6 root root    0 Feb 22 21:41 ..
-rw-rw-r-- 1 root gpio 4096 Feb 23 01:56 active_low
lrwxrwxrwx 1 root gpio    0 Feb 23 01:56 device -> ../../../gpiochip0
-rw-rw-r-- 1 root gpio 4096 Feb 23 01:56 direction
-rw-rw-r-- 1 root gpio 4096 Feb 23 01:56 edge
drwxrwxr-x 2 root gpio    0 Feb 23 01:56 power
lrwxrwxrwx 1 root gpio    0 Feb 23 01:56 subsystem -> ../../../../../../../class/gpio
-rw-rw-r-- 1 root gpio 4096 Feb 23 01:56 uevent
-rw-rw-r-- 1 root gpio 4096 Feb 23 01:57 value
```

Write and Read value
```
$ echo 1 > /sys/class/gpio/gpio529/value 
$ echo 0 > /sys/class/gpio/gpio529/value
$ cat /sys/class/gpio/gpio529/value
0
```

Set direction in or out
```
echo "out" > /sys/class/gpio/gpio529/direction 
```

Also we can manage gpio pins with ```pinctrl``` command


## QR Code generator to connect esp's wifi
https://www.qr-code-generator.com/solutions/wifi-qr-code/

## LoRa EByte RF Settings App for Mac
https://github.com/mosvov/ebyte-modules-setting/releases/tag/v1.0.1

## LoRa EByte RF Settings App For Windows (original)
https://www.ebyte.com/pdf-down/3341.html

## CP2102 Driver Windows
https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads
Extract zip file > Right Click to "silabser.inf" Install

## SerialPort Terminal (CoolTerm) for MacOs 
http://freeware.the-meiers.org

# Serial Communication Protocols 
![uart-i2c-spi](https://raw.githubusercontent.com/alikadir/iot/refs/heads/main/images/i2c-spi-uart.gif)
