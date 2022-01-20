#include "include/uart.h"
#include "include/spi.h"
#include "include/sbus.h"
#include <avr/io.h>
#include "include/debugLCD.h"
#include <avr/interrupt.h>

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {
  
  /* Pin setup */

  //SPI CS
  DDRB |= (1 << DDB2) | (1 << DDB3) |(1 << DDB4) |( 1 << DDB5);
  //PORTB |= (1 << PINB2); 

  /* Init */
  //UART_init();
  //SPI_init();
  SBUS_init();
  // Interrupt enable (used by SBUS)
  sei();
  
  WriteNum(100,0);


  while(1) {


    double wheel_diam_m  = 0.150;
    double wheel_dist_m = 0.600;
    double wheel_gear_ratio = 6;

    double linx = map(channels[2],272,1712,-0.5,0.5);
    double angz = map(channels[3],272,1712,-0.5,0.5);

    //Process drive from remote
    double vel_left_desired = (linx - angz * wheel_dist_m / 2.0) / (wheel_diam_m / 2);
    double vel_right_desired = (linx + angz * wheel_dist_m / 2.0) / (wheel_diam_m/ 2);

    double rad_per_s_left = vel_left_desired / (wheel_diam_m/ 2);
    double rad_per_s_right = vel_right_desired / (wheel_diam_m/ 2);

    double degrees_per_s_left = (rad_per_s_left * 180) / 3.14;
    double degrees_per_s_right = (rad_per_s_right * 180) / 3.14;

    int32_t left_speed = -(degrees_per_s_left * wheel_gear_ratio);
    int32_t right_speed = degrees_per_s_right * wheel_gear_ratio;

    WriteNum(left_speed/100,0);
  } 
  return 0;

}