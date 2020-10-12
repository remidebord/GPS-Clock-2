#ifndef __COMMON_H
#define __COMMON_H

/* includes ---------------------------------------------------------------- */
#include "stm32f0xx.h"
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"

/* define ------------------------------------------------------------------ */

/* enum -------------------------------------------------------------------- */
typedef enum
{
	PA_0 = GPIOA_BASE,
	PA_1,
	PA_2,
	PA_3,
	PA_4,
	PA_5,
	PA_6,
	PA_7,
	PA_8,
	PA_9,
	PA_10,
	PA_11,
	PA_12,
	PA_13,
	PA_14,
	PA_15,

	PB_0 = GPIOB_BASE,
	PB_1,
	PB_2,
	PB_3,
	PB_4,
	PB_5,
	PB_6,
	PB_7,
	PB_8,
	PB_9,
	PB_10,
	PB_11,
	PB_12,
	PB_13,
	PB_14,
	PB_15,

	PC_0 = GPIOC_BASE,
	PC_1,
	PC_2,
	PC_3,
	PC_4,
	PC_5,
	PC_6,
	PC_7,
	PC_8,
	PC_9,
	PC_10,
	PC_11,
	PC_12,
	PC_13,
	PC_14,
	PC_15,

	PD_2 = (GPIOD_BASE + 0x02),

	PF_0 = GPIOF_BASE,
	PF_1,
	PF_4 = (GPIOF_BASE + 0x04),
	PF_5,
	PF_6,
	PF_7,

	// Not connected
	NC = 0xFFFFFFFF
} PinName;

typedef enum
{
	Pin_Input  = 0x00,
	Pin_Output = 0x01,
	Pin_AF     = 0x02,
	Pin_AN     = 0x03
} PinMode;

typedef enum
{
	Push_Pull  = 0x00,
	Open_Drain = 0x01
} PinType;

typedef enum
{
	Pull_None    = 0x00,
	Pull_Up      = 0x01,
	Pull_Down    = 0x02,
	Pull_Default = Pull_None
} PinPull;

typedef enum
{
	Speed_Low    = 0x00,
	Speed_Medium = 0x01,
	Speed_High   = 0x03
} PinSpeed;

typedef enum
{
	AF_0 = 0x00, // WKUP, EVENTOUT, TIM15, SPI1, TIM17, MCO, SWDAT, SWCLK, TIM14, BOOT, USART1, CEC, IR_OUT, SPI2
	AF_1 = 0x01, // USART2, CEC, TIM3, USART1, USART2, EVENTOUT, I2C1, I2C2, TIM15
	AF_2 = 0x02, // TIM2, TIM1, EVENTOUT, TIM16, TIM17
	AF_3 = 0x03, // TS, I2C1, TIM15, EVENTOUT
	AF_4 = 0x04, // TIM14
	AF_5 = 0x05, // TIM16, TIM17
	AF_6 = 0x06, // EVENTOUT
	AF_7 = 0x07  // COMP1 OUT, COMP2 OUT
} PinAF;

typedef enum
{
	None = 0x00,
	Even = 0x02,
	Odd  = 0x03
} SerialParity;

typedef enum
{
	Channel_1 = 0x01,
	Channel_2 = 0x02,
	Channel_3 = 0x03,
	Channel_4 = 0x04
} TimerChannel;

/* functions --------------------------------------------------------------- */
uint8_t bcd2bin(uint8_t bcd);
uint8_t bin2bcd(uint8_t bin);

#endif
