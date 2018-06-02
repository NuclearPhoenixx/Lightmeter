# Lightmeter

Fully autonomous instrument for measuring and logging the levels of artificial night sky brightening.

This instrument is based on a micro-controller, a light sensor and the data logging capabilities, all compatible with the Arduino IDE so that it is easy to flash, improve & maintain. As long as there is power and space on the SD Card the board autonomously measures the current light levels in lux and saves them together with the time from an ultra-precise RTC to a file on the SD Card in easy-to-work-with JSON format. No other requirements.

This project aims to be a more straightforward and easier to use, though obviously less professional, instrument than the Kuffner-Sternwarte Lightmeter, see below at [idea](https://github.com/Phoenix1747/Lightmeter#idea).

## Idea

The idea for this project came from my friends at the [Kuffner Sternwarte](http://kuffner-sternwarte.at), their well-known [Lightmeter](http://hms.sternhell.at/lightwiki) and their campaign against light pollution and the ongoing disappearence of our beautiful nightsky. As just mentioned they developed their own little light sensor board, initially for the International Year of Astronomy 2009, and have since then made extensive measurements of the light levels in Europe with the support of universities and other observatories.

However, I as a normal user and also as developer have found some things that I don't really like about it. For example, you have to have a  logging device constantly plugged into the lightmeter and you need to have the drivers and software for it. So I came up with my own solution - this project. If you're more interested about this please head to the [comparison](https://github.com/Phoenix1747/Lightmeter#comparison) section where I compare it to the Kuffner-Sternwarte Lightmeter.

## Parts list

This list refers to the current prototype:

* [Adafruit Feather 32u4 Adalogger](https://learn.adafruit.com/adafruit-feather-32u4-adalogger/overview)
* [DS3231 Precision RTC from Adafruit](https://learn.adafruit.com/adafruit-ds3231-precision-rtc-breakout/overview)
* [TSL2591 High Dynamic Range Digital Light Sensor from Adafruit](https://learn.adafruit.com/adafruit-tsl2591)

## Hardware Features

TSL2591 light sensor:
* Lux Range: 188 uLux - 88,000 Lux on the fly
* 600M:1 Dynamic Range
* Separate measurements of infrared, full-spectrum or human-visible light

DS3231 RTC:
* Leap-Year compensation up to 2100
* Integrated temperature-compensated crystal oscillator (TCXO) and crystal
* Accuracy ±3.5ppm from -40°C to +85 °C
* Years of precise timekeeping

Adafruit Feather:
* Tiny (51mm x 23mm x 8mm) and light 5.1g
* ATmega32u4 @ 8MHz, 32K flash & 2K RAM 
* Embedded uSD Card slot

## Software Features

* Arbitrary time interval for measurements
* FAT32 filesystem for uSD Card
* Custom file name for the data log file(s)
* Custom max file size. If the size limit is reached, the system will continue to log to another file appending a number to the new file name, e.g. "LOG1.TXT". This is used to not overflow the FAT32 file limit and to not crash any software because of too large files
* LED feedback about the status (e.g. errors)

## Comparison

Welcome to everyone's favorite section: the comparison!

I try to compare it as objectively as possible, but please bear in mind that this is never fully possible. If I forgot something important, please let me know and write an issue or pull request!

I'm basing this comparison on the specs that I found on the official [data sheet](http://hms.sternhell.at/lightwiki/images/3/30/Lightmeter_specifications.pdf) and a [document](http://hms.sternhell.at/lightwiki/images/a/a7/Mueller_Low_Cost_Luxmeter.pdf) published by Dr. Günther Wuchter and André Müller. However, these documents are rather old and there already are like 3 newer generations of the lightmeter, on which I didn't find any technical information. If you found something, please let me know!

Please note that fields with a `-` do not apply to the prototype yet and will be updated in the future.

| Stat | Kuffner Lightmeter | Phoenix's Lightmeter |
| --- | --- | --- |
| Lux Range | 500 uLux - 200,000 Lux | 188 uLux - 120,000 Lux |
| Resolution | 50 - 100 uLux | 1000 uLux ([likely wrong](https://github.com/adafruit/Adafruit_TSL2591_Library/issues/22)) |
| Measurement Rate | 1/sec - 1/hour | 1/sec - 1/month |
| Length x Width x Height (mm) | 92 x 92 x 15 | - |
| Weight | 370g | - |
| Power Supply | 5V | 5V / 3.3V |
| Max Power Consumption | 10mA + logging hardware | 30mA active; 0.9mA standby |
| Requirements | Logging hardware, e.g. PC/Raspberry Pi and drivers | uSD Card, CR1220 Battery |
| Data Destination | Logging hardware | uSD Card |
| Data Rates (1 measurement/sec) | 3 MB/day | 3.3 MB/day |
| Price | ~100 EUR | ~50 EUR |
