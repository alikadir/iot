## PlatformIO 
https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start

## Steps of soldering on Li-on 
https://www.youtube.com/watch?v=dFoV_nLu7AA

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

## CH341A SPI/I2C EEPROM USB Programmer Application
https://github.com/alikadir/IMSProg

# CH341A Programmer (UART, SPI, I2C) Schematic
![CH341A](https://raw.githubusercontent.com/alikadir/iot/refs/heads/main/images/CH341A.png)

# Serial Communication Protocols 
![uart-i2c-spi](https://raw.githubusercontent.com/alikadir/iot/refs/heads/main/images/i2c-spi-uart.gif)

#### Raspberry Pi Nodejs GPIO
using npm library ```npm install onoff``` and read the issue https://github.com/fivdi/onoff/issues/197#issuecomment-1948884220

#### GPIO management with WiringPi on RaspberryPi and OrangePi
https://github.com/WiringPi/WiringPi
```
$ gpio readall

+-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |   IN | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |   IN | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | IN   | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | OUT  | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 1 | OUT  | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | OUT  | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
                                                                                                                                       
 +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 |      |     |     3.3V |        |   |  1 || 2  |   |        | 5V       |     |      |
 |  229 |   0 |    SDA.3 |    OFF | 0 |  3 || 4  |   |        | 5V       |     |      |
 |  228 |   1 |    SCL.3 |    OFF | 0 |  5 || 6  |   |        | GND      |     |      |
 |   73 |   2 |      PC9 |   ALT6 | 1 |  7 || 8  | 0 | OFF    | TXD.5    | 3   | 226  |
 |      |     |      GND |        |   |  9 || 10 | 0 | OFF    | RXD.5    | 4   | 227  |
 |   70 |   5 |      PC6 |   ALT5 | 0 | 11 || 12 | 0 | OFF    | PC11     | 6   | 75   |
 |   69 |   7 |      PC5 |   ALT5 | 0 | 13 || 14 |   |        | GND      |     |      |
 |   72 |   8 |      PC8 |    OFF | 0 | 15 || 16 | 0 | OFF    | PC15     | 9   | 79   |
 |      |     |     3.3V |        |   | 17 || 18 | 0 | OFF    | PC14     | 10  | 78   |
 |  231 |  11 |   MOSI.1 |    OFF | 0 | 19 || 20 |   |        | GND      |     |      |
 |  232 |  12 |   MISO.1 |    OFF | 0 | 21 || 22 | 1 | OUT    | PC7      | 13  | 71   |
 |  230 |  14 |   SCLK.1 |    OFF | 0 | 23 || 24 | 0 | OUT    | CE.1     | 15  | 233  |
 |      |     |      GND |        |   | 25 || 26 | 0 | OUT    | PC10     | 16  | 74   |
 |   65 |  17 |      PC1 |    OFF | 0 | 27 || 28 | 0 | ALT2   | PWM3     | 21  | 224  |
 |  272 |  18 |     PI16 |   ALT2 | 0 | 29 || 30 | 0 | ALT2   | PWM4     | 22  | 225  |
 |  262 |  19 |      PI6 |    OFF | 0 | 31 || 32 |   |        |          |     |      |
 |  234 |  20 |     PH10 |    OFF | 0 | 33 || 34 |   |        |          |     |      |
 +------+-----+----------+--------+---+----++----+---+--------+----------+-----+------+
 | GPIO | wPi |   Name   |  Mode  | V | Physical | V |  Mode  | Name     | wPi | GPIO |
 +------+-----+----------+--------+---+   H616   +---+--------+----------+-----+------+

```



#### Manual GPIO management with sh with sysfs on only RaspberryPi
Linux provides a **sysfs** interface for controlling GPIO pins via the /sys/class/gpio directory. 
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

