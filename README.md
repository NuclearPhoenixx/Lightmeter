# Lightmeter 

![GitHub issues](https://img.shields.io/github/issues/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub pull requests](https://img.shields.io/github/issues-pr/phoenix1747/lightmeter.svg?style=for-the-badge) ![GitHub](https://img.shields.io/github/license/phoenix1747/lightmeter.svg?style=for-the-badge)

Fully autonomous instrument for measuring and logging the levels of artificial night sky brightening.

This instrument is based on a microcontroller, a light sensor and the data logging capabilities, all compatible with the Arduino IDE so that it is easy to update, improve & maintain. As long as there is power and space on the uSD Card the board automatically measures the current light levels in lux and saves it together with a timestamp from an ultra-precise RTC to the uSD Card. All the data is JSON formatted and saved with as little whitespace as possible to ensure a long runtime. No other requirements.

This project was inspired by the Kuffner-Sternwarte Lightmeter and aims to be an easier to use all-in-one instrument. More info about the [idea](https://github.com/Phoenix1747/Lightmeter#idea) is below.

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

To do

## Software features

* Arbitrary time interval for measurements
* Can be used with any FAT32 formatted uSD card
* Custom file name for the data log files
* Custom maximum file size. If the size limit is reached, the system will continue to log to another file appending a number to the end of the new file name, e.g. "LOG1.TXT". This is used to not overflow the FAT32 file limit and to not crash any software because of too large files
* JSON formatted data (see [data structure](https://github.com/Phoenix1747/Lightmeter#Data-and-settings))
* LED status feedback

## Preparing the Lightmeter

To do: This will explain how to get started using the Arduino IDE and an ICSP programmer.

## Data and settings

To do: Explain how the JSON data is formatted and what settings are available.

## Comparison

I try to compare it as objectively as possible, but please bear in mind that this is never fully possible. If I forgot something important, please let me know and write an issue or pull request!

Also note that this is an all-in-one instrument prototype and not a sensor on it's own. This means that you're bound to all of the hardware and it's limitations and I also don't have a nice weather-proof case. Furthermore, the board (i.e. the software) needs to be calibrated for the much needed exact measurements. This looks to be harder than expected and needs to be done empirically.

I'm basing this comparison on the specs that I found in the official [data sheet](http://hms.sternhell.at/lightwiki/images/3/30/Lightmeter_specifications.pdf) and a [document](http://hms.sternhell.at/lightwiki/images/a/a7/Mueller_Low_Cost_Luxmeter.pdf) published by Dr. Günther Wuchter and André Müller. However, these documents look outdated and there are already like three newer generations of the lightmeter, on which I didn't find any technical information. If you found something, please let me know!

Please note that fields with a `-` do not apply to the prototype yet and will be updated in the future.

| Stat | Kuffner Lightmeter | Phoenix's Lightmeter |
| --- | --- | --- |
| Lux Range | 500 μLux - 200,000 Lux | 188 μLux - 110,000 Lux |
| Resolution | 50 - 100 μLux | 1000 μLux ([likely wrong](https://github.com/adafruit/Adafruit_TSL2591_Library/issues/22)) |
| Measurement Rate | 1/sec - 1/hour | 1/sec - 1/month |
| Length x Width x Height (mm) | 92 x 92 x 15 | - |
| Weight | 370g | - |
| Power Input | 5V | 3.3V - 6V |
| Max Power Consumption | 10mA + logging hardware | 5mA active + μSD active; 0.23mA standby + μSD standby |
| Requirements | Logging hardware, e.g. Raspberry Pi, and drivers | μSD Card, CR1220 Battery |
| Data Destination | Logging hardware | μSD Card |
| Data Rates (1 measurement/sec) | 3 MB/day | 3.3 MB/day |
| Price | ~100 EUR (?) | ~50 EUR |
