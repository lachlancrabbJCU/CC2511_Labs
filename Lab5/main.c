/**************************************************************
 * main.c
 * rev 1.0 23-Aug-2023 Lachl
 * Lab5
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include "hardware/pwm.h"
#include <stdio.h>

#define CONTENT_OF(addr) (*(volatile uint32_t *)addr)
#define PAD_CONTROL_BASE 0x4001c000
#define REG_PAD_CONTROL_GPIO15 (PAD_CONTROL_BASE + 0x40)
#define CTRL_PDE 0x4
#define CTRL_PUE 0x8

#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

#define STEP 25
#define MAX_LEVEL 250
#define MIN_LEVEL 0
#define INPUT_PIN 15

int main(void)
{
  stdio_init_all();

  gpio_init(INPUT_PIN);
  gpio_init(BLUE_LED);
  gpio_set_dir(INPUT_PIN, GPIO_IN);
  gpio_set_dir(BLUE_LED, GPIO_OUT);

  CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PUE | CTRL_PDE);

  gpio_set_function(RED_LED, GPIO_FUNC_PWM);
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM);
  //gpio_set_function(BLUE_LED, GPIO_FUNC_PWM);

  uint slice_num_RED = pwm_gpio_to_slice_num(RED_LED);
  uint slice_num_GREEN = pwm_gpio_to_slice_num(GREEN_LED);
  //uint slice_num_BLUE = pwm_gpio_to_slice_num(BLUE_LED);

  int red_level = 0;
  int green_level = 0;
  //int blue_level = 0;

  pwm_set_enabled(slice_num_RED, true);
  pwm_set_enabled(slice_num_GREEN, true);
  //pwm_set_enabled(slice_num_BLUE, true);

  pwm_set_gpio_level(RED_LED, red_level);
  pwm_set_gpio_level(GREEN_LED, green_level);
 // pwm_set_gpio_level(BLUE_LED, blue_level);

  int ch;

  while (true)
  {
    int ch = getchar_timeout_us(0);
    if (ch != PICO_ERROR_TIMEOUT)
    {
      switch (ch)
      {
      case 'u':
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) |= CTRL_PUE;
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~CTRL_PDE;
        printf("Pin %d to Pullup\r\n", INPUT_PIN);
        break;
      case 'd':
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~CTRL_PUE;
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) |= CTRL_PDE;
        printf("Pin %d to Pulldown\r\n", INPUT_PIN);
        break;
      case 'o':
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~CTRL_PUE;
        CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~CTRL_PDE;
        printf("Pin %d to Pull None\r\n", INPUT_PIN);
        break;
      case 'r':
        red_level = (red_level + STEP <= MAX_LEVEL) ? red_level + STEP : MAX_LEVEL;
        printf("RED increase %d\r\n", red_level);
        break;
      case 'R':
        red_level = (red_level - STEP >= MIN_LEVEL) ? red_level - STEP : MIN_LEVEL;
        printf("RED decrease %d\r\n", red_level);
        break;
      case 'g':
        green_level = (green_level + STEP <= MAX_LEVEL) ? green_level + STEP : MAX_LEVEL;
        printf("green increase %d\r\n", green_level);
        break;
      case 'G':
        green_level = (green_level - STEP >= MIN_LEVEL) ? green_level - STEP : MIN_LEVEL;
        printf("green decrease %d\r\n", green_level);

        break;
      // case 'b':
      //   blue_level = (blue_level + STEP <= MAX_LEVEL) ? blue_level + STEP : MAX_LEVEL;
      //   printf("blue increase\r\n");
      //   printf("%d ", blue_level);
      //   break;
      // case 'B':
      //   blue_level = (blue_level - STEP >= MIN_LEVEL) ? blue_level - STEP : MIN_LEVEL;
      //   printf("blue decrease\r\n");
      //   printf("%d ", blue_level);
      //   break;
      default:
          CONTENT_OF(REG_PAD_CONTROL_GPIO15) &= ~(CTRL_PUE | CTRL_PDE);
        break;
      }

      pwm_set_gpio_level(RED_LED, red_level * red_level);
      pwm_set_gpio_level(GREEN_LED, green_level * green_level);
     // pwm_set_gpio_level(BLUE_LED, blue_level * blue_level);
      gpio_put(BLUE_LED, gpio_get(INPUT_PIN));
    }
  }
}