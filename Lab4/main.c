/**************************************************************
 * main.c
 * rev 1.0 16-Aug-2023 Lachl
 * Lab4
 * ***********************************************************/

#include <stdbool.h>

typedef unsigned long uint32_t;

#define LED_PIN (1 << 25)
#define RED_PIN (1 << 11)
#define GREEN_PIN (1 << 12)
#define BLUE_PIN (1 << 13)

#define GPIO_BASE 0x4001c000
#define REG_PAD_GPIO11 (GPIO_BASE + 0x30)
#define REG_PAD_GPIO12 (GPIO_BASE + 0x34)
#define REG_PAD_GPIO13 (GPIO_BASE + 0x38)
#define REG_PAD_GPIO25 (GPIO_BASE + 0x68)

#define GPIO_CONTROL_BASE 0x40014000
#define GPIO25_CONTROL (GPIO_CONTROL_BASE + 0x0cc)
#define GPIO11_CONTROL (GPIO_CONTROL_BASE + 0x05c)
#define GPIO12_CONTROL (GPIO_CONTROL_BASE + 0x064)
#define GPIO13_CONTROL (GPIO_CONTROL_BASE + 0x06c)

#define SIO_BASE 0xd0000000
#define REG_GPIO_OE (SIO_BASE + 0x020)
#define REG_GPIO_OUT_SET (SIO_BASE + 0x014)
#define REG_GPIO_OUT_CLR (SIO_BASE + 0x018)
#define REG_GPIO_OE_SET (SIO_BASE + 0x024)

#define CONTENT_OF(addr) (*(volatile uint32_t *)addr)

int main(void)
{
  // TODO - Initialise components and variables
  // Enable the pads
  CONTENT_OF(REG_PAD_GPIO25) = 0x0;                               // reset
  CONTENT_OF(REG_PAD_GPIO25) = CONTENT_OF(REG_PAD_GPIO25) | 0x40; // set bit 6 to 1
  // Select the GPIO function

  CONTENT_OF(GPIO25_CONTROL) = CONTENT_OF(GPIO25_CONTROL) | 0x1F; // reset GPIO Function

 
  CONTENT_OF(GPIO25_CONTROL) = 0x1A ^ CONTENT_OF(GPIO25_CONTROL); // Set function to f5
  // set the data direction
  CONTENT_OF(REG_GPIO_OE_SET) = 0x0;
  CONTENT_OF(REG_GPIO_OE_SET) = CONTENT_OF(REG_GPIO_OE_SET) | LED_PIN;

  while (true)
  {

    CONTENT_OF(REG_GPIO_OUT_SET) = CONTENT_OF(REG_GPIO_OUT_SET) | LED_PIN;
    for (uint32_t i = 0; i < 8000000; i++)
    {
      __asm volatile("nop");
    }
    CONTENT_OF(REG_GPIO_OUT_CLR) = CONTENT_OF(REG_GPIO_OUT_CLR) | LED_PIN;
    for (uint32_t i = 0; i < 8000000; i++)
    {
      __asm volatile("nop");
    }
  }
}
