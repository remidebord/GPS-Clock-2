/*!
 * \file RTC.cpp
 * \brief RTC API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 09 janvier 2016
 *
 * RTC library.
 *
 */

#include "RTC.h"

extern "C"
{
	void (*alarmCallback)(void);
}

Clock :: Clock(void)
{
	// Enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	// Disable RTC domain write protection
	PWR->CR |= PWR_CR_DBP;
	
	// Enable LSI clock
	RCC->CSR |= RCC_CSR_LSION;
		
	// Wait LSI
	while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY);
	
	// RTC not configured ?
	if(RTC->BKP0R != RTC_BKP_VALUE)
	{	
		// RTC clock source: LSI
		RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;
			
		// Enable RTC clock
		RCC->BDCR |= RCC_BDCR_RTCEN;
			
		// Disable RTC registers write protection 
		RTC->WPR = 0xCA;
		RTC->WPR = 0x53;
		
		// Enter in initialisation mode
		RTC->ISR |= RTC_ISR_INIT;
		
		// Wait initialisation flag
		while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
		
		// RTC prescaler (LSI = 40kHz) => 40000 / (128 * 312) = 1 Hz
		RTC->PRER = ((RTC_PREDIV_A << 16) | RTC_PREDIV_S);
		
		// 24H format
		RTC->TR &= ~RTC_TR_PM;
		
		// Bypass shadow registers
		RTC->CR |= RTC_CR_BYPSHAD;
		
		// Exit initialisation mode
		RTC->ISR = 0;
		
		// Write value into backup register
		RTC->BKP0R = RTC_BKP_VALUE;
		
		// Enable RTC registers write protection
		RTC->WPR = 0xFE;
		RTC->WPR = 0x64;
	}
}

void Clock :: set(Time* time)
{
	// Disable RTC registers write protection 
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
		
	// Enter in initialisation mode
	RTC->ISR |= RTC_ISR_INIT;
		
	// Wait initialisation flag
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	
	// Set time
	RTC->TR = 0;
	
	RTC->TR |= (bin2bcd(time->hours) << 16);
	RTC->TR |= (bin2bcd(time->minutes) << 8);
	RTC->TR |= bin2bcd(time->seconds);
		
	// Exit initialisation mode
	RTC->ISR = 0;
		
	// Enable RTC registers write protection
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;
}

void Clock :: set(Date* date)
{
	// Disable RTC registers write protection 
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
		
	// Enter in initialisation mode
	RTC->ISR |= RTC_ISR_INIT;
		
	// Wait initialisation flag
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	
	// Set date
	RTC->DR = 0;
	
	RTC->DR |= (bin2bcd(date->year) << 16);
	RTC->DR |= (bin2bcd(date->month) << 8);
	RTC->DR |= bin2bcd(date->day);
		
	// Exit initialisation mode
	RTC->ISR = 0;
		
	// Enable RTC registers write protection
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;
}

void Clock :: get(Time *time)
{
	Time tmp;
	uint32_t reg = RTC->TR;
		
	// Get hours, minutes, seconds
	tmp.hours = ((reg & (RTC_TR_HT | RTC_TR_HU)) >> 16);
	tmp.minutes = ((reg & (RTC_TR_MNT | RTC_TR_MNU)) >> 8);
	tmp.seconds = (reg & (RTC_TR_ST | RTC_TR_SU));
		
	// Convert in binary
	time->hours = bcd2bin(tmp.hours);
	time->minutes = bcd2bin(tmp.minutes);
	time->seconds = bcd2bin(tmp.seconds);
}

void Clock :: get(Date *date)
{
	Date tmp;
	uint32_t reg = RTC->DR;
	
	// Get year, month, day
	tmp.year = ((reg & (RTC_DR_YT | RTC_DR_YU)) >> 16);
	tmp.month = ((reg & (RTC_DR_MT | RTC_DR_MU)) >> 8);
	tmp.day = (reg & (RTC_DR_DT | RTC_DR_DU));
	
	// Convert in binary
	date->year = bcd2bin(tmp.year);
	date->month = bcd2bin(tmp.month);
	date->day = bcd2bin(tmp.day);
}
