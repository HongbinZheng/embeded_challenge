#include "aht10.h"

// Initialization of AHT10
void aht10_init(I2C & i2c)
{
    thread_sleep_for(40);   // wait at least 40ms after powering up the AHT10
    char init_arr[3] = {0xe1, 0x08, 0x00};  // init command 0xe1 and code 0x08, 0x00
    int ack = i2c.write(aht10_addr, init_arr, 3);
    if(ack != 1)
    {
        cout<<"The AHT10 is not correctly connected."<<endl;
    }
}

void aht10_soft_reset(I2C & i2c)
{
    char soft_reset[1] = {reset_code};   // soft reset code 0xba
    i2c.write(reset_addr, soft_reset, 1);
    thread_sleep_for(20);   // soft reset takes no more than 20ms to complete
}

int aht10_read_data(I2C & i2c, double & humidity, double & temperature)
{
    // Start measurement
    char start_measurement[3] = {0xac, 0x33, 0x00}; // measure command 0xac and code 0x33, 0x00
    i2c.write(aht10_addr, start_measurement, 3);
    thread_sleep_for(80);   // wait at least 80ms after sending the command 0xac

    //--------------------------------------------------------------------------------------------------
    // Read data
    char data_buffer[6] = {};
    i2c.read(aht10_addr, data_buffer, 6);
    // if bit 3 of the first byte(status byte) == 0, the device is not calibrated, and needs to initlize 
    // return false value 1, which forces the outer while loop to execute again
    if ((data_buffer[0] & 0x08) == 0)
    {
        aht10_init(i2c);
        return 1;
    }
    // if bit 7 of the first byte(status byte) == 1, the device is busy, and requires waiting
    // return false value 1, which forces the outer while loop to execute again
    if ((data_buffer[0] & 0x80) == 1)
    {
        return 1;
    }
    // now that bit 7 == 0 and bit 3 == 1, the data is ready to be read
    int raw_humi = (data_buffer[1] << 12) | (data_buffer[2] << 4) | (data_buffer[3] >> 4);  // refer to datasheet page 8
    int raw_temp = ((data_buffer[3] & 0x0f) << 16) | (data_buffer[4] << 8) | (data_buffer[5]);  // refer to datsheet page 8
    humidity = 100.0 * (double)raw_humi / pow(2.0,20.0); // calculated in percentage
    temperature = 200.0 * (double)raw_temp / pow(2.0,20.0) - 50.0; // calculated in Celsius 
    return 0;
}