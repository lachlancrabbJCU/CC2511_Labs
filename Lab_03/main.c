/**************************************************************
 * main.c
 * rev 1.0 09-Aug-2023 Lachlan
 * Lab3
 * ***********************************************************/

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13
#define PULSE_MILLISECONDS 300

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
  // TODO - Initialise components and variables
  // stdio_init_all();
  // stdio_usb_init();

  // gpio_init(RED_LED);
  // gpio_init(GREEN_LED);
  // gpio_init(BLUE_LED);
  // gpio_set_dir(RED_LED, GPIO_OUT);
  // gpio_set_dir(GREEN_LED, GPIO_OUT);
  // gpio_set_dir(BLUE_LED, GPIO_OUT);
  // while (true)
  // {
  //   // TODO - Repeated code here

  //   int ch = getchar_timeout_us(0);
  //   switch (ch)
  //   {
  //   case 'r':
  //     gpio_put(RED_LED, !gpio_get(RED_LED));
  //     printf("RED TOGGLED\r\n");
  //     break;
  //   case 'g':
  //     gpio_put(GREEN_LED, !gpio_get(GREEN_LED));
  //     printf("GREEN TOGGLED\r\n");
  //     break;
  //   case 'b':
  //     gpio_put(BLUE_LED, !gpio_get(BLUE_LED));
  //     printf("BLUE TOGGLED\r\n");
  //     break;
  //   case 'w':
  //     gpio_put(RED_LED, 1);
  //     gpio_put(GREEN_LED, 1);
  //     gpio_put(BLUE_LED, 1);
  //     break;
  //   case 'o':
  //     gpio_put(RED_LED, 0);
  //     gpio_put(GREEN_LED, 0);
  //     gpio_put(BLUE_LED, 0);
  //     break;
  //   case 'a':
  //     for (int i = 0; i < 5; i++)
  //     {
  //       gpio_put(RED_LED, 1);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(GREEN_LED, 1);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(RED_LED, 0);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(BLUE_LED, 1);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(GREEN_LED, 0);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(RED_LED, 1);
  //       sleep_ms(PULSE_MILLISECONDS);
  //       gpio_put(BLUE_LED, 0);
  //     }
  //     break;
  //   default:
  //     break;
  //   }
  //   sleep_ms(20); // Wait 20ms
  //}
}
