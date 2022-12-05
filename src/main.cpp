#include "aht10.h"
using namespace std;

int main()
{
  I2C i2c(PB_7, PB_6);  // Datasheet Page 69: PB6 I2C1_SCL, PB7 I2C1_SDA
  double humidity;
  double temperature;
  DigitalOut led(LED1);
  DigitalOut led2(LED2);
  while(1)
  { 
    while(aht10_read_data(i2c, humidity, temperature));
    cout<<"Humidity: "<<humidity<<"%    Temperature: "<<temperature<<"°C"<<endl;


    // if humidity is too low, maybe this person stops breathing
    if(humidity < 40)
    {
      double humidity_buffer[10];
      int len_hb = sizeof(humidity_buffer) / sizeof(humidity_buffer[0]);
      // start recording the next 10 datapoints(0.5s per datapoint, total period of 5s)
      int index = 0;
      while(index < len_hb){
        while(aht10_read_data(i2c, humidity, temperature));
        cout<<"Humidity: "<<humidity<<"%    Temperature: "<<temperature<<"°C"<<endl;
        // humidity drop might be causing by breath in 
        // or the person resume breathing
        // reset the index so it can restart the recording when next stop breathing happen 
        if(humidity > 40){
          index =0;
          continue;
        }
        humidity_buffer[index] = humidity;
        index++;
        thread_sleep_for(500);
      }
      
      // for(int i = 0; i < len_hb; i++)
      // {
      //   while(aht10_read_data(i2c, humidity, temperature));
      //   cout<<"Humidity: "<<humidity<<"%    Temperature: "<<temperature<<"°C"<<endl;
      //   humidity_buffer[i] = humidity;
      //   thread_sleep_for(500);
      // }

      // calculate the average humidity, if it's still low, this person stops breathing, need to send a warning
      double sum = 0;
      for(int i = 0; i < len_hb; i++)
      {
        sum += humidity_buffer[i];
      }
      double average = sum/10;
      if(average < 40)
      {
        while(1)
        {
          led = 1;
          led2 = 0;
          thread_sleep_for(300);
          led = 0;
          led2 = 1;
          thread_sleep_for(300);
        }
      }

    }
    thread_sleep_for(500);
  }
}