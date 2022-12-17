//Bingbing Chen, netid: bc3285
//Yufeng Duan, netid: yd2284
//Han Lin, netid: hl4827
//Hongbin Zheng, netid: hz3322
#include "aht10.h"
using namespace std;

bool decrease(double pervious, double current){
  if(current - pervious < 0)
    return true;
  return false;
}

int main()
{
  I2C i2c(PB_7, PB_6);  // Datasheet Page 69: PB6 I2C1_SCL, PB7 I2C1_SDA
  double humidity;
  double temperature;
  double pervious = 0.0;
  DigitalOut led(LED1);
  DigitalOut led2(LED2);
  int index = 0;
  while(1)
  { 
    while(aht10_read_data(i2c, humidity, temperature));
    cout<<"Humidity: "<<humidity<<"%    Temperature: "<<temperature<<"°C"<<endl;
    // if humidity is too low, maybe this person stops breathing,increase the index by one equals to + 0.5s
    // or the current humidity is lower than pervious one, it might be stop breathing, increase the index, 
    // if countinous decrease by 10s(20 datapoints), the person is stop breathing
    // else the current humidity greater than previous, the decrease might becuase of breath in and breath out, reset the index.
    if(humidity < 40 || humidity - pervious < 0){
      index++;
    }else{
      index=0;
    }
    if(index==19){
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

    pervious = humidity;
    thread_sleep_for(500);
  }
}