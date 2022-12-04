#ifndef AHT10_H
#define AHT10_H

#include <mbed.h>
#include <iostream>
#define aht10_addr 0x70 //7-bit address 0x38 OR 0 -> (0x38<<1) | 0 == 0x70
#define reset_addr 0x80
#define reset_code 0xba


int aht10_detection(I2C & i2c);
void aht10_init(I2C & i2c);
void aht10_soft_reset(I2C & i2c);
int aht10_read_data(I2C & i2c, double & humidity, double & temperature);
#endif /*AHT10.H*/