/*!
 * \file Power.cpp
 * \brief Power API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 07 janvier 2016
 *
 * Power management library.
 *
 * RUN mode (default) @ 48MHz => 12 mA
 * STOP mode => 3.7 uA
 * STANDY mode => 1.1 uA
 *
 */

#include "Power.h"

Power :: Power(void)
{
	// Enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}

void Power :: stop(void)
{
	// Stop mode
	PWR->CR &= ~(PWR_CR_PDDS | PWR_CR_LPSDSR);
	
	// Power down deepsleep: stop mode
	
	// Regulator: low power
	PWR->CR |= PWR_CR_LPSDSR;
	
	// System control register (sleepdeep)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	// Wait For Interrupt
	__WFI();
	
	// Clear sleepdeep bit
	SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk); 
	
	// Init clocks after Stop mode exit
	SystemInit();
}

void Power :: standby(PinName pin)
{
	// Enable WKUP pin
	switch(pin)
	{
		case PA_0: PWR->CSR |= PWR_CSR_EWUP1; break;
		case PC_13: PWR->CSR |= PWR_CSR_EWUP2; break;
		default: break;
	}
	
	// Clear wakeup flag
	PWR->CR |= PWR_CR_CWUF;
	
	// Standby mode
	PWR->CR |= PWR_CR_PDDS;
	
	// System control register (sleepdeep)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	// Wait For Interrupt
	__WFI();
}
