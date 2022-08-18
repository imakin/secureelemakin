#ifndef READSTRINGUNTIL_H
#define READSTRINGUNTIL_H

#include <Arduino.h>//uint8_t and Serial

/**
 * similar to Serial.readStringUntil, but use predefined output buffer and max length
 * to save SRAM. without the need of reinstatiating String when the SRAM already have the buffer to use
 * @param terminator(uint8_t): terminator charracter of read-string-until (typical value: '\n')
 * @param *output (uint8_t*): output buffer 
 * @param max_length (uint8_t): stop reading and consider 'until' is reached if total read already reached max_length (typical value: length of output buffer)
 * @param start_index (uint8_t): start index when writing to *output (typical value: 0)
 * @return (uint8_t) length/how many charracters it actually writes into output buffer
 */
uint8_t readStringUntil(uint8_t terminator, uint8_t *output, uint8_t max_length, uint8_t start_index);


#endif
