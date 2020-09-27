# Lightmeter 

![GitHub issues](https://img.shields.io/github/issues/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub pull requests](https://img.shields.io/github/issues-pr/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub](https://img.shields.io/github/license/phoenix1747/lightmeter.svg?style=for-the-badge)

Fully autonomous instrument for measuring and logging the levels of artificial night sky brightening.

This instrument is based on a microcontroller, a light sensor and the data logging capabilities, all compatible with the Arduino IDE so that it is easy to update, improve & maintain. As long as there is power and space on the uSD Card the board automatically measures the current light levels in lux and saves it together with a timestamp and current temperature from an ultra-precise RTC to the uSD Card. All the data is CSV formatted and saved to ensure a long runtime. No other requirements.

This project was inspired by the Kuffner-Sternwarte Lightmeter and aims to be an easier to use all-in-one instrument. More info about the [idea](https://github.com/Phoenix1747/Lightmeter#idea) is below.

More resources regarding the original Kuffner-Sternwarte Lightmeter can be found here:
- [Monitoring Night-Sky Brightness with a Lightmeter Network, G. Wuchterl, talk at 2009 European Symposium for the protection of the night skies](http://hms.sternhell.at/lightwiki/images/f/fe/Lightmeter_Network_Armagh.pdf)
- [Lightmeter performance and calibraton in the Atacama](http://hms.sternhell.at/lightwiki/images/7/77/Mueller_lightmeter_site-testing2010.pdf)

## Idea

The idea for this project came from my friends at the [Kuffner Sternwarte](http://kuffner-sternwarte.at), their well-known [Lightmeter](http://hms.sternhell.at/lightwiki) and their campaign against light pollution and the ongoing disappearence of our beautiful nightsky. As just mentioned they developed their own little light sensor board, initially for the International Year of Astronomy 2009, and have since then made extensive measurements of the light levels in Europe (and Japan) with the support of universities and other observatories.

However, I as normal user and developer have found some things that I think are improvable. For example, you have to have a logging device constantly plugged into the lightmeter with the correct drivers and software on top of the much bigger power requirement. So I came up with my own solution - this project. My goal is to provide an all-in-one solution with the sensor, a microcontroller, RTC and uSD card instead of only the instrument itself.

If you're more interested about this please head to the [comparison](https://github.com/Phoenix1747/Lightmeter#comparison) section where I compare it to the Kuffner-Sternwarte Lightmeter as good as I can.

## Hardware features

ATMEGA328P MCU:
* Clocked @ 8Mhz
* 32KB flash memory
* 2KB SRAM
* Specifically low power

DS3231SN RTC:
* Leap-Year compensation up to the year 2100
* Integrated temperature-compensated crystal oscillator (TCXO) and crystal
* Accuracy ±3.5ppm from -40°C to +85 °C
* Years of precise timekeeping

TSL2591 Ambient Light Sensor:
* Lux Range: 188 μLux - 88,000 Lux on the fly; software scaled up to 110,000 Lux
* 600M:1 Dynamic Range
* Separate measurements of infrared, full-spectrum or human-visible light possible

## Hardware prototype

You can find the full schematic right [here](schematic_lightmeter.pdf). You only have to work on the pcb layout for yourself and then you're ready to go. The software is designed and tested to work with this hardware prototype.

Important: Input voltage with the LDO regulator is as defined in the datasheet 2.3V to 6.0V. Output is fixed at 3.3V.

## Flashing the software

If you're using the hardware prototype and the Arduino IDE then you can flash the firmware code over the ICSP interface with a programmer like [USBasp](https://www.fischl.de/usbasp/) which works great for me.

To flash it from inside the IDE, you need to set the board config to "`Arduino Pro or Pro Mini`" and set the processor to "`ATmega328P (3.3V, 8 MHz)`" then go to "Sketch > Upload Using Programmer" and you should be good to go. Just check for the right orientation of the ICSP header.

Important: Don't forget to use 3.3V with your ICSP programmer as mentioned above at "hardware prototype".

## Software features

* Arbitrary time interval for measurements
* Can be used with any FAT32 formatted uSD card
* Custom file name for the data log files
* Custom maximum file size. If the size limit is reached, the system will continue to log to another file appending a number to the end of the new file name, e.g. "LOG1.TXT". This is used to not overflow the FAT32 file limit and to not crash any software because of too large files
* CSV formatted data (see [data structure](https://github.com/Phoenix1747/Lightmeter#Data-and-settings))
* LED status feedback

## Data and settings

If you open the Arduino project you will see a section of the code is settings, these should pretty much all be self-explanatory:

```cpp
/* == SETTINGS == */
const String FILE_NAME = "DATA"; //Filename for the data file; 8 chars or less (FAT32 limit).
const String FILE_EXTENSION = "CSV"; //File extension for the data file; 3 chars or less (FAT32 limit).
const uint32_t MAX_FILESIZE = 4000000000; //Max filesize in bytes; defaults 4GB (FAT32 size limit).
const uint32_t M_INTERVAL = 5000; //Time between measurements [ms].
const bool MEASURE_TEMP = true; //Bool to log additional temperature info.
/* == == */
```

The measured data will be saved into a csv file named according `FILE_NAME`. If the file exceeds the `MAX_FILESIZE` then it will append a number to the end of the (file)name and continue recording. Eventually the software will create another new file and increment the number and so on and on.

The formatted csv string looks like that:

```
timestamp,luxvalue
```

If you decide to (optionally) record the temperature as well you will get this:

```
timestamp,luxvalue,temperature
```

## Comparison

I try to compare it as objectively as possible, but please bear in mind that this is never fully possible. If I forgot something important, please let me know and write an issue or pull request!

Also note that this is an all-in-one instrument prototype and not a sensor on it's own. This means that you're bound to all of the hardware and it's limitations and I also don't have a nice weather-proof case. Furthermore, the board (i.e. the software) needs to be calibrated for the much needed exact measurements. This looks to be harder than expected and needs to be done empirically.

I'm basing this comparison on the specs that I found in the official [data sheet](http://hms.sternhell.at/lightwiki/images/3/30/Lightmeter_specifications.pdf) and a [document](http://hms.sternhell.at/lightwiki/images/a/a7/Mueller_Low_Cost_Luxmeter.pdf) published by Dr. Günther Wuchter and André Müller. However, these documents look outdated and there are already like three newer generations of the lightmeter, on which I didn't find any technical information. If you found something, please let me know!

Please note that fields with a `-` do not apply to the prototype yet and will be updated in the future.

| Stat | Kuffner Lightmeter | Phoenix's Lightmeter |
| --- | --- | --- |
| Lux Range | 500 μLux - 200,000 Lux | 188 μLux - 110,000 Lux (probably not as accurate) |
| Resolution | 50 - 100 μLux | 1000 μLux ([very likely wrong](https://github.com/adafruit/Adafruit_TSL2591_Library/issues/22)) |
| Measurement Rate | 1/sec - 1/hour | 1/sec - 1/49 days |
| Length x Width x Height (mm) | 92 x 92 x 15 | no case |
| Weight | 370g | no case |
| Input Voltage Range | 5V | 3.3V - 6V |
| Max Power Consumption | 10mA + logging hardware @5V | 4.5mA active + μSD active; 0.22mA standby + μSD standby @5V |
| Requirements | Logging hardware, e.g. Raspberry Pi, and drivers | μSD Card, CR1220 Battery |
| Data Destination | Logging hardware | μSD Card |
| Data Rates (@1 measurement/sec) | 3 MB/day | 2.1 MB/day |
| Price | ~100 EUR (?) | ~50 EUR |
