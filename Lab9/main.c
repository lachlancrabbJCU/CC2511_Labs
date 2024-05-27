/**************************************************************
 * main.c
 * rev 1.0 03-Oct-2023 Lachl
 * Lab9
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/adc.h"

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

#define PULSE_1 60
#define PULSE_0 40
#define PULSE_IDLE 50
#define PULSE_WIDTH_ERROR 2


//#define ADC_MAX (1 << 12)
#define LDR 26
// #define CONVERSION_FACTOR_VOLTAGE 3.3f/(1<<12)
// #define CONVERSION_FACTOR_PWM  255.0f/(ADC_MAX)

uint16_t ADC;
uint16_t prev_ADC = 0;
bool rising_edge_detected = false;
absolute_time_t rising_edge_time;
uint32_t pulse_width_ms = 0;

int white_on(void)
{
  gpio_put(RED_LED, 1);
  gpio_put(GREEN_LED, 1);
  gpio_put(BLUE_LED, 1);
}

int white_off(void)
{
  gpio_put(RED_LED, 0);
  gpio_put(GREEN_LED, 0);
  gpio_put(BLUE_LED, 0);
}

int receive(void)
{
  ADC = adc_read();

  // Detect rising and falling edges
  int16_t diff = ADC - prev_ADC;
  if (diff > 100 && !rising_edge_detected)
  {
    // Rising edge detected
    rising_edge_time = get_absolute_time();
    rising_edge_detected = true;
  }

  else if (diff < -100 && rising_edge_detected)
  {
    // Falling edge detected
    absolute_time_t falling_edge_time = get_absolute_time();
    pulse_width_ms = absolute_time_diff_us(rising_edge_time, falling_edge_time) / 1000;

    // check if 0 or 1 is being recieved 
    if (pulse_width_ms > (PULSE_1 - PULSE_WIDTH_ERROR) && pulse_width_ms < (PULSE_1 + PULSE_WIDTH_ERROR))
    {
      printf("1");
    }
    else if (pulse_width_ms > (PULSE_0 - PULSE_WIDTH_ERROR) && pulse_width_ms < (PULSE_0 + PULSE_WIDTH_ERROR))
    {
      printf("0");
    }
    rising_edge_detected = false;
  }
  // Store the current ADC reading as the previous reading for the next iteration
  prev_ADC = ADC;
  sleep_ms(1);
}

int transmit(void)
{
  if(uart_is_readable(uart0)){
  int ch = getchar();
  if(ch == '0'){
    white_on();
    sleep_ms(PULSE_0);
    white_off();
    sleep_ms(PULSE_0);
  }

  else if(ch == '1'){
    white_on();
    sleep_ms(PULSE_1);
    white_off();
    sleep_ms(PULSE_1);
  }
  }
  else {
    white_on();
    sleep_ms(PULSE_IDLE);
    white_off();
    sleep_ms(PULSE_IDLE);
  }
}

int main(void)
{
  // TODO - Initialise components and variables
  // initiate blink
  gpio_init(RED_LED);
  gpio_init(GREEN_LED);
  gpio_init(BLUE_LED);

  gpio_set_dir(RED_LED, GPIO_OUT);
  gpio_set_dir(GREEN_LED, GPIO_OUT);
  gpio_set_dir(BLUE_LED, GPIO_OUT);

  // initiate ADC
  stdio_init_all();
  adc_init();
  adc_gpio_init(LDR);
  adc_select_input(0);

  // adc set up
  adc_init();
  adc_gpio_init(LDR);
  adc_select_input(0);

  while (true)
  {
    //transmit();
    receive();
  }
}
