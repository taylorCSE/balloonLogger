# Header

0-1    SYNC  EEEE
2-3    ID    0000 - FFFF
4      CMD   1A,2A

## GPS Packet (CMD = 1A)

5      Status
6-7    Altitude
8-9    Rate
10-45  LAT,LAT REF,LON,LON REF,SPD,HDG
46-47  CRC16

## Data Packet (CMD = 2A)

5-9    Digital Payload
10-45  Analog Payload 1 - 18
8-9    Rate
10-45  LAT,LAT REF,LON,LON REF,SPD,HDG
46-47  CRC16
