/** 
 * CC2511 Lab 10
 */

/* Assembler directives */

.syntax unified /* Use modern syntax                                                    */
.text           /* The "text" section of the executable is where code is placed.        */
.thumb_func     /* Specifies that main is a thumb mode function and will be offset by 1 */ 
.global main    /* Declare "main" as a global symbol (callable from other files).       */
.align 4        /* Make sure the starting address is a multiple of 4.                   */

/* Definitions */
/* These are equivalent to C macros. However, note that they use a comma. */

.EQU WAIT_LOOPS_ON,           10000000
.EQU WAIT_LOOPS_OFF,          10000000

/* TODO - Add definitions of register addresses used in the program */  
.EQU GPIO_PAD, 0x4001c000
.EQU REG_PAD_GPIO25_OFFSET, 0x68
.EQU REG_PAD25, 0x4001c068

.EQU GPIO_CONTROL_BASE, 0x40014000
.EQU PAD_CONTROL_GPIO25, 0x400140cc

.EQU SIO_BASE,          0xd0000000
.EQU REG_GPIO_OE,       0xd0000020
.EQU REG_GPIO_OUT_SET,  0xd0000014
.EQU REG_GPIO_OUT_CLR,  0xd0000018
.EQU REG_GPIO_OE_SET,   0xd0000024

.EQU GPIO_FUNCT_RESET, 0x1F

.EQU LED_PIN, 0x02000000
.EQU WAIT_TIME, 0x7A1200


/* Main function */

main:
  push {lr}

  /* Enabling Pad */
  ldr r0, =REG_PAD25
  ldr r1, [r0]
  movs r2, #0x40
  ands r1, r1, r2
  str r1, [r0] /* r1 is 0x40 */

  /* SET GPIO function */
  ldr r0, =PAD_CONTROL_GPIO25
  ldr r1, [r0]
  ldr r2, =GPIO_FUNCT_RESET
  orrs r1, r1, r2
  movs r3, #0x1a
  eors r1, r3, r1
  str r1, [r0]

  /* set GPIO direction */
  ldr r2, =LED_PIN
  ldr r0, =REG_GPIO_OE_SET
  ldr r1, [r0]
  orrs r1, r1, r2
  str r1, [r0]

movs r3, #01
    blinkloop:
        /* turn on led */
        ldr r2, =LED_PIN 
        ldr r0, =REG_GPIO_OUT_SET
        ldr r1, [r0]
        orrs r1, r1, r2
        str r1, [r0]

        ldr r0, =WAIT_TIME
        waitloop:
            subs r0, #1   /* r0 -= 1 */
            cmp r0, #0   /* is r0 equal to zero? */
            bne waitloop     /* branch if not equal */

        /* turn off led */
        ldr r2, =LED_PIN 
        ldr r0, =REG_GPIO_OUT_CLR
        ldr r1, [r0]
        eors r1, r1, r2
        str r1, [r0]

        ldr r0, =WAIT_TIME
        waitloop2:
            subs r0, #1   /* r0 -= 1 */
            cmp r0, #0   /* is r0 equal to zero? */
            bne waitloop2     /* branch if not equal */

        cmp r3, #0   /* is r0 equal to zero? */
        bne blinkloop     /* branch if not equal */  

	pop {pc}  /* Return from this function, by popping the return address into the program counter (pc) */
