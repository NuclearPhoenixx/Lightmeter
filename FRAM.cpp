/*
 * Original Author: KTOWN (Adafruit Industries), licensed under BSD - see header file!
 * Changes involved to make it work best with my lightmeter.
*/
#include <stdlib.h>
#include <math.h>

#include "FRAM.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

FRAM_SPI::FRAM_SPI(int8_t cs)
{
  _cs = cs;
  _framInitialised = false;
}

////////////////////////////////////////////////////////////////////////////////
// Initializes SPI and configures the chipdoing anything else)

boolean FRAM_SPI::begin()
{
  return begin(_cs, 2);
}

boolean FRAM_SPI::begin(uint8_t nAddressSizeBytes)
{
  return begin(_cs, nAddressSizeBytes);
}

boolean FRAM_SPI::begin(int8_t cs, uint8_t nAddressSizeBytes)
{
  _cs = cs;
  setAddressSize(nAddressSizeBytes);

  /* Configure SPI */
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  SPI.begin();
  SPI.setClockDivider (SPI_CLOCK_DIV2); // 8 MHz
  SPI.setDataMode(SPI_MODE0);

  /* Make sure we're actually connected */
  uint8_t manufID;
  uint16_t prodID;
  getDeviceID(&manufID, &prodID);

  if (manufID != 0x04 && manufID != 0x7f)
  {
    //Serial.print("Unexpected Manufacturer ID: 0x"); Serial.println(manufID, HEX);
    return false;
  }
  if (prodID != 0x0302 && prodID != 0x7f7f)
  {
    //Serial.print("Unexpected Product ID: 0x"); Serial.println(prodID, HEX);
    return false;
  }

  /* Everything seems to be properly initialised and connected */
  _framInitialised = true;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Enables or disables writing to the SPI flash

void FRAM_SPI::writeEnable (bool enable)
{
  digitalWrite(_cs, LOW);
  if (enable)
  {
    SPItransfer(OPCODE_WREN);
  }
  else
  {
    SPItransfer(OPCODE_WRDI);
  }
  digitalWrite(_cs, HIGH);
}

/**************************************************************************/
/*!
    @brief  Writes a byte at the specific FRAM address

    @params[in] addr
                The 32-bit address to write to in FRAM memory
    @params[in] i2cAddr
                The 8-bit value to write at framAddr
*/
/**************************************************************************/
void FRAM_SPI::write8 (uint32_t addr, uint8_t value)
{
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRITE);
  writeAddress(addr);
  SPItransfer(value);
  /* CS on the rising edge commits the WRITE */
  digitalWrite(_cs, HIGH);
}

/**************************************************************************/
/*!
    @brief  Writes count bytes starting at the specific FRAM address

    @params[in] addr
                The 32-bit address to write to in FRAM memory
    @params[in] values
                The pointer to an array of 8-bit values to write starting at addr
    @params[in] count
                The number of bytes to write
*/
/**************************************************************************/
void FRAM_SPI::write (uint32_t addr, const uint8_t *values, size_t count)
{
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRITE);
  writeAddress(addr);
  for (size_t i = 0; i < count; i++)
  {
    SPItransfer(values[i]);
  }
  /* CS on the rising edge commits the WRITE */
  digitalWrite(_cs, HIGH);
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value from the specified FRAM address

    @params[in] addr
                The 32-bit address to read from in FRAM memory

    @returns    The 8-bit value retrieved at framAddr
*/
/**************************************************************************/
uint8_t FRAM_SPI::read8 (uint32_t addr)
{
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_READ);
  writeAddress(addr);
  uint8_t x = SPItransfer(0);
  digitalWrite(_cs, HIGH);
  return x;
}

/**************************************************************************/
/*!
    @brief  Read count bytes starting at the specific FRAM address

    @params[in] addr
                The 32-bit address to write to in FRAM memory
    @params[out] values
                The pointer to an array of 8-bit values to read starting at addr
    @params[in] count
                The number of bytes to read
*/
/**************************************************************************/
void FRAM_SPI::read (uint32_t addr, uint8_t *values, size_t count)
{
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_READ);
  writeAddress(addr);
  for (size_t i = 0; i < count; i++)
  {
    uint8_t x = SPItransfer(0);
    values[i] = x;
  }
  digitalWrite(_cs, HIGH);
}

/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID from the IC

    @params[out]  manufacturerID
                  The 8-bit manufacturer ID (Fujitsu = 0x04)
    @params[out]  productID
                  The memory density (bytes 15..8) and proprietary
                  Product ID fields (bytes 7..0). Should be 0x0302 for
                  the MB85RS64VPNF-G-JNERE1.
*/
/**************************************************************************/
void FRAM_SPI::getDeviceID(uint8_t *manufacturerID, uint16_t *productID)
{
  uint8_t a[4] = { 0, 0, 0, 0 };
  //uint8_t results;

  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_RDID);
  a[0] = SPItransfer(0);
  a[1] = SPItransfer(0);
  a[2] = SPItransfer(0);
  a[3] = SPItransfer(0);
  digitalWrite(_cs, HIGH);

  /* Shift values to separate manuf and prod IDs */
  /* See p.10 of http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RS64V-DS501-00015-4v0-E.pdf */
  *manufacturerID = (a[0]);
  *productID = (a[2] << 8) + a[3];
}

////////////////////////////////////////////////////////////////////////////////
// Reads the status register

uint8_t FRAM_SPI::getStatusRegister()
{
  uint8_t reg = 0;
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_RDSR);
  reg = SPItransfer(0);
  digitalWrite(_cs, HIGH);
  return reg;
}

////////////////////////////////////////////////////////////////////////////////
// Sets the status register

void FRAM_SPI::setStatusRegister(uint8_t value)
{
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRSR);
  SPItransfer(value);
  digitalWrite(_cs, HIGH);
}

void FRAM_SPI::setAddressSize(uint8_t nAddressSize)
{
  _nAddressSizeBytes = nAddressSize;
}

uint8_t FRAM_SPI::SPItransfer(uint8_t x)
{
  return SPI.transfer(x);
}

void FRAM_SPI::writeAddress(uint32_t addr)
{
  if (_nAddressSizeBytes>3)
    SPItransfer((uint8_t)(addr >> 24));
  if (_nAddressSizeBytes>2)
    SPItransfer((uint8_t)(addr >> 16));
  SPItransfer((uint8_t)(addr >> 8));
  SPItransfer((uint8_t)(addr & 0xFF));
}

