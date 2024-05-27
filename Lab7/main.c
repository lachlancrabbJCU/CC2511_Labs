/**************************************************************
 * main.c
 * rev 1.0 11-Sep-2023 Lachl
 * Lab7
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "terminal.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

// PWM LED Control
#define RED_LED 11
#define GREEN_LED 12
#define BLUE_LED 13

#define DESC_ALIGN 35
#define OPT_ALIGN 15

volatile bool input_ready = false;

static int chars_rxed = 0;

volatile char buff[100];
volatile unsigned int buff_index = 0;
// int key_press;
uint8_t red_level = 0;
uint8_t green_level = 0;
uint8_t blue_level = 0;

uint8_t brightness;

int count = 0;

void display_header()
{
  term_cls(); // CLEAR TERMINAL
  term_move_to(1, 1);
  term_set_color(clrWhite, clrBlack); // SET FOReGROUND RED, BACKGROUND BLUE
  printf("|-----------------------| LED CONTROLLER UI |-----------------------|\r\n");
  term_move_to(OPT_ALIGN, 2);
  printf("> red n");
  term_move_to(OPT_ALIGN, 3);
  printf("> green n");
  term_move_to(OPT_ALIGN, 4);
  printf("> blue n");
  term_move_to(OPT_ALIGN, 5);
  printf("> off");
  term_move_to(DESC_ALIGN, 2);
  printf("Set the red PWM ratio to n");
  term_move_to(DESC_ALIGN, 3);
  printf("Set the green PWM ratio to n");
  term_move_to(DESC_ALIGN, 4);
  printf("Set the blue PWM ratio to n");
  term_move_to(DESC_ALIGN, 5);
  printf("Set all PWM ratios to Zero");
  term_move_to(1, 6);
  printf("|-------------------------------------------------------------------|\r\n");
  printf("|-------------------------------------------------------------------|\r\n");
  printf("|-------------------------------------------------------------------|\r\n");
  term_move_to(1, 10);
  printf("> ");
}

void on_uart_rx()
{
  // count++;
  while (uart_is_readable(UART_ID))
  { 
    uint8_t key_press = uart_getc(UART_ID);
    uart_putc(UART_ID, key_press);

    buff[buff_index] = key_press;
    buff_index++;

    if (key_press != '\r')
    {
      uart_putc(UART_ID, '\n');
      key_press = '\n';
      buff[buff_index] = '\0';
      buff_index = 0;
    }
    else if (sscanf((char *)buff, "red %hhu", &brightness) == 1)
    {
      red_level = brightness;
    }

    else if (sscanf((char *)buff, "green %hhu", &brightness) == 1)
    {
      green_level = brightness;
    }

    else if (sscanf((char *)buff, "blue %hhu", &brightness) == 1)
    {
      blue_level = brightness;
    }

    else if (strcmp((char *)buff, "off\r") == 0)
    {
      red_level = 0;
      green_level = 0;
      blue_level = 0;
    }

    // buff = 0;
    pwm_set_gpio_level(RED_LED, red_level * red_level);
    pwm_set_gpio_level(GREEN_LED, green_level * green_level);
    pwm_set_gpio_level(BLUE_LED, blue_level * blue_level);
    term_move_to(1, 6);
    display_bar(red_level, clrRed);
    display_bar(green_level, clrGreen);
    display_bar(blue_level, clrBlue);
    term_move_to(1, 11);
    term_erase_line();
    term_move_to(3, 10);
    term_erase_line();
    input_ready = true;

    // pwm_set_gpio_level(RED_LED, red_level * red_level);
    // pwm_set_gpio_level(GREEN_LED, green_level * green_level);
    // pwm_set_gpio_level(BLUE_LED, blue_level *blue_level);
  }
}

int main(void)
{
  // TODO - Initialise components and variables
  stdio_init_all();

  uart_init(UART_ID, 2400);

  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  int key_press;

  int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);

  uart_set_hw_flow(UART_ID, false, false);

  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

  uart_set_fifo_enabled(UART_ID, false);

  int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

  irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
  irq_set_enabled(UART_IRQ, true);

  uart_set_irq_enables(UART_ID, true, false);

  display_header();

  gpio_set_function(RED_LED, GPIO_FUNC_PWM);
  gpio_set_function(GREEN_LED, GPIO_FUNC_PWM);
  gpio_set_function(BLUE_LED, GPIO_FUNC_PWM);

  uint slice_num_RED = pwm_gpio_to_slice_num(RED_LED);
  uint slice_num_GREEN = pwm_gpio_to_slice_num(GREEN_LED);
  uint slice_num_BLUE = pwm_gpio_to_slice_num(BLUE_LED);

  pwm_set_enabled(slice_num_RED, true);
  pwm_set_enabled(slice_num_GREEN, true);
  pwm_set_enabled(slice_num_BLUE, true);

  pwm_set_gpio_level(RED_LED, red_level);
  pwm_set_gpio_level(GREEN_LED, green_level);
  pwm_set_gpio_level(BLUE_LED, blue_level);

  while (true)
  {
    while (input_ready)
    {
      __asm("wfi");
    }
    input_ready = false;
  }
}
