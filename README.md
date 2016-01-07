# PPMtoUART
- Simple converter PPM signal to UART byte flow

###Format
- $RC[uint8_t num][uint16_t val][uint8_t crc]
- $RC - header of 3 bytes
- num - number of rc channel
- val - in microseconds
- crc - xor of all previous bytes

###Workflow
- Pakage of data sends aftet each pulse of ppm signal
- Trigger to sync is MAX_PULSE macro in main.cpp
