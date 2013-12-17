# Overview

48 Byte packet

    0-1    SYNC (EEEE)
    2-4    Header
    5-45   Payload
    46-47  CRC-16

## Header

    2-3    ID    0000 - FFFF
    4      CMD   1A,2A

## GPS Payload (CMD = 1A)

    5      Status
    6-7    Altitude
    8-9    Rate
    10-45  LAT,LAT REF,LON,LON REF,SPD,HDG (26 bytes)
    46-47  CRC16

## Data Payload (CMD = 2A)

    5      Digital Payload
    6-7    Altitude
    8-9    Rate
    10-45  Analog Payload 1 - 18