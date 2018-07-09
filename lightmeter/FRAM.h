/*
    This contains everything to work with the Fujitsu MB85RS64V. Thanks to Adafruit!
    
    I forked the original by Adafruit and am now making changes to this lib to make
    it work best with my lightmeter.

    =============================================
    
    Software License Agreement (BSD License)

    Copyright (c) 2013, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __FRAM_H_
#define __FRAM_H_

#include <Arduino.h>
#include <SPI.h>

typedef enum opcodes_e
{
  OPCODE_WREN   = 0b0110,     /* Write Enable Latch */
  OPCODE_WRDI   = 0b0100,     /* Reset Write Enable Latch */
  OPCODE_RDSR   = 0b0101,     /* Read Status Register */
  OPCODE_WRSR   = 0b0001,     /* Write Status Register */
  OPCODE_READ   = 0b0011,     /* Read Memory */
  OPCODE_WRITE  = 0b0010,     /* Write Memory */
  OPCODE_RDID   = 0b10011111  /* Read Device ID */
} opcodes_t;

class FRAM_SPI {
 public:
  FRAM_SPI(int8_t cs);

  boolean begin();
  boolean begin(int8_t cs, uint8_t nAddressSizeBytes);
  void writeEnable(bool enable);
  void write8(uint16_t addr, uint8_t value);
  void write(uint16_t addr, const uint8_t *values, size_t count);
  uint8_t read8(uint16_t addr);
  void read(uint16_t addr, uint8_t *values, size_t count);
  void getDeviceID(uint8_t *manufacturerID, uint16_t *productID);
  uint8_t getStatusRegister();
  void setStatusRegister(uint8_t value);
  void setAddressSize(uint8_t nAddressSize);
  void bufferTimestamp(uint16_t addr, uint32_t timestamp);
  void bufferLux(uint16_t addr, float lux);

 private:
  uint8_t SPItransfer(uint8_t x);
  void writeAddress(uint16_t addr);

  boolean _framInitialised;
  uint8_t _nAddressSizeBytes;
  int8_t _cs;
};

#endif

