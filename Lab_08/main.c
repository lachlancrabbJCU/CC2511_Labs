/**************************************************************
 * main.c
 * rev 1.0 13-Sep-2023 Lachl
 * Lab8
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define ADC_MAX 1100
#define ADC_MIN 18

#define LDR 26
#define CONVERSION_FACTOR_VOLTAGE 3.3f/(1<<12)
#define CONVERSION_FACTOR_PWM  255.0f/(ADC_MAX)

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13
#define MAX_LEVEL 255

int main(void) {
  // TODO - Initialise components and variables
  uint16_t ADC;
  stdio_init_all();

  //adc set up
  adc_init();
  adc_gpio_init(LDR);
  adc_select_input(0);

  //initialise LED
  gpio_init(BLUE_LED);
  gpio_init(GREEN_LED);
  gpio_init(RED_LED);
  gpio_set_dir(RED_LED, GPIO_OUT);
  gpio_set_dir(GREEN_LED, GPIO_OUT);
  gpio_set_dir(BLUE_LED, GPIO_OUT);

  gpio_set_function(RED_LED, GPIO_FUNC_PWM);
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM);
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM);

  uint slice_num_RED = pwm_gpio_to_slice_num(RED_LED);
  uint slice_num_GREEN = pwm_gpio_to_slice_num(GREEN_LED);
  uint slice_num_BLUE = pwm_gpio_to_slice_num(BLUE_LED);

  int red_level = 0;
  int green_level = 0;
  int blue_level = 0;

  pwm_set_enabled(slice_num_RED, true);
  pwm_set_enabled(slice_num_GREEN, true);
  pwm_set_enabled(slice_num_BLUE, true);


  //init pwm

  while (true) {
    // TODO - Repeated code here
    ADC = adc_read(); //int -> 0 - 4000 approx 1000
    //float voltage = ADC*(CONVERSION_FACTOR_VOLTAGE);

    uint16_t adjusted_adc = ADC - ADC_MIN;
    float red_level = (CONVERSION_FACTOR_PWM)*adjusted_adc;
    //float red_level = ((CONVERSION_FACTOR_PWM)*adjusted_adc <= MAX_LEVEL) ? (CONVERSION_FACTOR_PWM)*adjusted_adc : MAX_LEVEL;
    printf("ADC level: %hi, ", ADC);
    printf("ADJUSTED ADC level: %hi, ", adjusted_adc);
    //printf("ADC Voltage: %f\r\n", voltage);
    printf("%f",CONVERSION_FACTOR_PWM);
    printf("ADC PWM: %f\r\n", red_level);

    pwm_set_gpio_level(RED_LED, (red_level) * (red_level));
    sleep_us(4);
   
    //set pwm to use led with ldr lvl
    // reset bounds for adc for realistic min to max

  }
}
