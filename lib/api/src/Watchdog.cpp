/*!
 * \file Watchdog.cpp
 * \brief Watchdog API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 04 janvier 2016
 *
 * Watchdog library (IWDG).
 *
 */

#include "Watchdog.h"


Watchdog :: Watchdog(uint32_t ms)
{
	uint32_t q = 0;

	// Enable watchdog registers write access
	IWDG->KR = IWDG_KR_WRITE;
	
	// Prevent IWDG reset in debug
	DBGMCU->APB1FZ = DBGMCU_APB1_FZ_DBG_IWDG_STOP;
	
	// Clock LSI = 40kHz, prescaler = 256
	IWDG->PR = IWDG_PR_PR;
		
	// quantum = 1 / (40kHz / 256) = 1 / 156 Hz (6.4 ms)
	// Keep precision! (q base is 100us)
	q = 10000 / (LSI_VALUE >> 8);
	
	// Reload value can be 6.4ms to 6.4 x 4096 = 26.21s
	IWDG->RLR = (ms * 10) / q;
	
	// Wait reload value flag
	while((IWDG->SR & IWDG_SR_RVU) == 0);
	
	// Enable watchdog (Force LSI clock ON)
	IWDG->KR = IWDG_KR_ENABLE;
	
	// Reload watchdog counter
	this->reload();
}


void Watchdog :: reload(void)
{
	IWDG->KR = IWDG_KR_RELOAD;
}
