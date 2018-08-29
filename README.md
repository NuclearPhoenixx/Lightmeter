# Lightmeter 

![GitHub issues](https://img.shields.io/github/issues/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub pull requests](https://img.shields.io/github/issues-pr/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub](https://img.shields.io/github/license/phoenix1747/lightmeter.svg?style=for-the-badge)

Fully autonomous instrument for measuring and logging the levels of artificial night sky brightening.

This instrument is based on a micro-controller, a light sensor and the data logging capabilities, all compatible with the Arduino IDE so that it is easy to update, improve & maintain. As long as there is power and space on the SD Card the board autonomously measures the current light levels in lux and saves them together with the time from an ultra-precise RTC to a file on the SD Card in easy-to-work-with JSON format (optional temperature logging available). No other requirements.

This project was inspired by the Kuffner-Sternwarte Lightmeter and aims to be a more straightforward and easier to use all-in-one instrument. More info about the [idea](https://github.com/Phoenix1747/Lightmeter#idea) is below.

## Idea

The idea for this project came from my friends at the [Kuffner Sternwarte](http://kuffner-sternwarte.at), their well-known [Lightmeter](http://hms.sternhell.at/lightwiki) and their campaign against light pollution and the ongoing disappearence of our beautiful nightsky. As just mentioned they developed their own little light sensor board, initially for the International Year of Astronomy 2009, and have since then made extensive measurements of the light levels in Europe with the support of universities and other observatories.

However, I as a normal user and also as developer have found some things that I don't really like about it. For example, you have to have a logging device constantly plugged into the lightmeter and you need to have the drivers and software for it. So I came up with my own solution - this project. My goal is to provide an all-in-one solution with the sensor, a microcontroller, RTC and SD card instead of only the instrument itself.

If you're more interested about this please head to the [comparison](https://github.com/Phoenix1747/Lightmeter#comparison) section where I compare it to the Kuffner-Sternwarte Lightmeter (if that's even possible).

## Parts list

This list refers to the current prototype; only most important parts listed:

* ATMEGA328P @ 8Mhz, 32KB flash & 2KB SRAM
* Maxim Integrated DS3231SN#T&R RTC
* [TSL2591 High Dynamic Range Digital Light Sensor (from Adafruit for prototyping)](https://learn.adafruit.com/adafruit-tsl2591)

## Hardware Features

ATMEGA328P Microcontroller:
* Clocked @ 8Mhz
* 32KB flash memory
* 2KB SRAM
* Specifically Low Power

DS3231SN RTC:
* Leap-Year compensation up to 2100
* Integrated temperature-compensated crystal oscillator (TCXO) and crystal
* Temperature sensor output (± 3°C, 0.25°C resolution)
* Accuracy ±3.5ppm from -40°C to +85 °C
* Years of precise timekeeping

TSL2591 Ambient Light Sensor:
* Lux Range: 188 μLux - 88,000 Lux on the fly; software scaled up to 120,000 Lux
* 600M:1 Dynamic Range
* Separate measurements of infrared, full-spectrum or human-visible light

## Hardware Prototype

To Do: Waiting for the new version of the protoype.

## Software Features

* Arbitrary time interval for measurements
* FAT32 filesystem for any uSD Card
* Custom file name for the data log file(s)
* Custom max file size. If the size limit is reached, the system will continue to log to another file appending a number to the new file name, e.g. "LOG1.TXT". This is used to not overflow the FAT32 file limit and to not crash any software because of too large files
* JSON formatted data (see [data structure](https://github.com/Phoenix1747/Lightmeter#data-structure))
* LED feedback about the status (e.g. errors)
* Automatic retries, gain/timing scaling and error correction

## Preparing The Lightmeter

To Do: This will explain how to flash the software using the Arduino IDE and an ICSP programmer such as the USBasp. In addition, this will show you how to prepare your uSD card.

## Data Structure

To Do: This will show you how the JSON data is formatted and saved onto the uSD card so that you know how to evaluate the data.

## Settings Config

To Do: This will be the documentation about all the possible config options.

## Comparison

Welcome to everyone's favorite section: the comparison!

I try to compare it as objectively as possible, but please bear in mind that this is never fully possible. If I forgot something important, please let me know and write an issue or pull request!

Also note that this is an all-in-one instrument prototype and not a sensor on it's own. This means that the board is obviously more fault-prone and I also don't have a nice weather-proof case. Furthermore, the board (i.e. the software) needs to be calibrated for exact measurements. This might be harder than expected and needs to be done empiricly.

I'm basing this comparison on the specs that I found on the official [data sheet](http://hms.sternhell.at/lightwiki/images/3/30/Lightmeter_specifications.pdf) and a [document](http://hms.sternhell.at/lightwiki/images/a/a7/Mueller_Low_Cost_Luxmeter.pdf) published by Dr. Günther Wuchter and André Müller. However, these documents are rather old and there already are like 3 newer generations of the lightmeter, on which I didn't find any technical information. If you found something, please let me know!

Please note that fields with a `-` do not apply to the prototype yet and will be updated in the future.

| Stat | Kuffner Lightmeter | Phoenix's Lightmeter |
| --- | --- | --- |
| Lux Range | 500 μLux - 200,000 Lux | 188 μLux - 120,000 Lux |
| Resolution | 50 - 100 μLux | 1000 μLux ([likely wrong](https://github.com/adafruit/Adafruit_TSL2591_Library/issues/22)) |
| Measurement Rate | 1/sec - 1/hour | 1/sec - 1/month |
| Length x Width x Height (mm) | 92 x 92 x 15 | - |
| Weight | 370g | - |
| Power Supply | 5V | 3.5V - 6V |
| Max Power Consumption | 10mA + logging hardware | 5mA active + μSD active; 0.23mA standby + μSD standby |
| Requirements | Logging hardware, e.g. PC/Raspberry Pi and drivers | μSD Card, CR1220 Battery |
| Data Destination | Logging hardware | μSD Card |
| Data Rates (1 measurement/sec) | 3 MB/day | 3.3 MB/day |
| Price | ~100 EUR (?) | ~50 EUR |
