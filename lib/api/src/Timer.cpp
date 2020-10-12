/*!
 * \file Timer.cpp
 * \brief Timer API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 13 janvier 2017
 *
 * Timer library (PWM, Ticker, TimeOut).
 *
 */

#include "Timer.h"

extern "C"
{
	void (*updateCallback[8])(void); // 7 + 1 basic timer
}

Timer :: Timer(TIM_TypeDef* timer)
{ 	
	m_timer = timer;
	
	// Enable timer clock
	switch((uint32_t)timer)
	{
		case TIM1_BASE : RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; break;
		case TIM2_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; break;
		case TIM3_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; break;
		case TIM6_BASE : RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; break;
		case TIM14_BASE: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; break;
		case TIM15_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; break;
		case TIM16_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM16EN; break;
		case TIM17_BASE: RCC->APB2ENR |= RCC_APB2ENR_TIM17EN; break;
		default: break;
	}
	
	// Center-aligned mode: edge aligned
	m_timer->CR1 &= ~TIM_CR1_CMS;
	
	// Direction: upcounter
	m_timer->CR1 &= ~TIM_CR1_DIR;
	
	// Clock division: default
	m_timer->CR1 &= ~TIM_CR1_CKD;
	
	// Prescaler: 48 MHz / 1000000 (1us)
	m_timer->PSC = (SystemCoreClock / 1000000) - 1;
	
	// Autoreload value
	m_timer->ARR = 0xFFFFFFFF;
	
	// Repetition counter
	m_timer->RCR = 0;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
}

void Timer :: start(void)
{
	m_timer->CR1 |= TIM_CR1_CEN;
}

void Timer :: stop(void)
{
	m_timer->CR1 &= ~TIM_CR1_CEN;
}

void Timer :: reset(void)
{
	m_timer->CNT = 0;
}

uint32_t Timer :: read(void)
{
	return (this->read_ms() / 1000);
}

uint32_t Timer :: read_ms(void)
{
	return (this->read_us() / 1000);
}

uint32_t Timer :: read_us(void)
{
	return m_timer->CNT;
}

void Timer :: attach(void(*f)(void))
{
	IRQn_Type irq = TIM1_BRK_UP_TRG_COM_IRQn;
	uint8_t i = 0;
	
	switch((uint32_t)m_timer)
	{
		case TIM1_BASE : i = 0; irq = TIM1_BRK_UP_TRG_COM_IRQn; break;
		case TIM2_BASE : i = 1; irq = TIM2_IRQn; break;
		case TIM3_BASE : i = 2; irq = TIM3_IRQn; break;
		case TIM6_BASE : i = 3; irq = TIM6_DAC_IRQn; break;
		case TIM14_BASE: i = 4; irq = TIM14_IRQn; break;
		case TIM15_BASE: i = 5; irq = TIM15_IRQn; break;
		case TIM16_BASE: i = 6; irq = TIM16_IRQn; break;
		case TIM17_BASE: i = 7; irq = TIM17_IRQn; break;
		default: break;
	}
	
	// Set callback
	updateCallback[i] = f;
	
	// NVIC configuration
	NVIC_SetPriority(irq, 1); // High: 0, Low: 3
	NVIC_EnableIRQ(irq);
	
	// Enable update interrupt
	m_timer->DIER |= TIM_DIER_UIE;
}

void Timer :: detach(void)
{
	// Disable update interrupt
	m_timer->DIER &= ~TIM_DIER_UIE;
	
	// Clear update flag
	m_timer->SR &= ~TIM_SR_UIF;
}

/////////////////////

Ticker :: Ticker(TIM_TypeDef* timer) : Timer(timer)
{
	// Nothing to do
}

void Ticker :: attach_ms(void(*f)(void), uint32_t ms)
{
	// Prescaler: 48 MHz / 10000 (100 us)
	m_timer->PSC = (SystemCoreClock / 10000) - 1; 
	
	// Autoreload value
	m_timer->ARR = (ms * 10) - 1;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
	
	this->attach(f);

	// Enable timer
	this->start();
}

void Ticker :: attach_us(void(*f)(void), uint32_t us)
{
	// Prescaler: 48 MHz / 1000000 (1us)
	m_timer->PSC = (SystemCoreClock / 1000000) - 1;
	
	// Autoreload value
	m_timer->ARR = us - 1;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
	
	this->attach(f);

	// Enable timer
	this->start();
}

/////////////////////

Timeout :: Timeout(TIM_TypeDef* timer) : Timer(timer)
{
	// One pulse mode
	m_timer->CR1 |= TIM_CR1_OPM;
	
	// Update only on counter underflow/overflow
	m_timer->CR1 |= TIM_CR1_URS;
	
	// Clear update flag
	m_timer->SR &= ~TIM_SR_UIF;
}

void Timeout :: attach_ms(void(*f)(void), uint32_t ms)
{
	// Prescaler: 48 MHz / 10000 (100 us)
	m_timer->PSC = (SystemCoreClock / 10000) - 1; 
	
	// Autoreload value
	m_timer->ARR = (ms * 10) - 1;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
	
	this->attach(f);
}

void Timeout :: attach_us(void(*f)(void), uint32_t us)
{
	// Prescaler: 48 MHz / 1000000 (1us)
	m_timer->PSC = (SystemCoreClock / 1000000) - 1;
	
	// Autoreload value
	m_timer->ARR = us - 1;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
	
	this->attach(f);
}

void Timeout :: detach(void)
{
	// Disable update interrupt
	m_timer->DIER &= ~TIM_DIER_UIE;
	
	// Clear update flag
	m_timer->SR &= ~TIM_SR_UIF;

	// Disable timer
	m_timer->CR1 &= ~TIM_CR1_CEN;
}

void Timeout :: start(void)
{	
	// Reset counter
	m_timer->CNT = 0;
	
	// Enable timer	
	m_timer->CR1 |= TIM_CR1_CEN;
}

/////////////////////

PwmOut :: PwmOut(PinName pin, uint32_t frequency, TIM_TypeDef* timer, TimerChannel channel) : GPIO(pin, Pin_AF), Timer(timer)
{
	uint32_t prescaler = 1;
	uint8_t index = 0;

	m_timer = timer;
	m_frequency = frequency;
	m_dutyCycle = 0;
	m_channel = channel;

	// GPIO configuration
	this->type(Push_Pull);
	this->pull(Pull_Up);
	
	switch((uint32_t)m_timer)
	{
		case TIM1_BASE : this->af(AF_2); break;
		case TIM2_BASE : this->af(AF_2); break;
		case TIM3_BASE : this->af(AF_1); break;
		case TIM14_BASE: this->af(AF_4); break;
		case TIM15_BASE: this->af(AF_0); break;
		case TIM16_BASE: this->af(AF_2); break;
		case TIM17_BASE: this->af(AF_2); break;
		default: break;
	}
	
	// Timer configuration
	if((m_frequency < PWMOUT_LOW_FREQUENCY)) prescaler = (SystemCoreClock / 100000);
	
	m_timer->PSC = prescaler - 1;
	m_timer->ARR = ((SystemCoreClock / prescaler) / m_frequency) - 1;
	
	// Channel configuration
	index = (m_channel > 2) ? 1 : 0;
			
	// Capture compare mode: PWM1
	m_timer->CCMR[index] |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2) << ((~m_channel & 0x01) * 8);
			
	// Output compare polarity: low
	m_timer->CCER |= TIM_CCER_CC1P << ((m_channel - 1) * 4);
			
	// Output state: enabled
	m_timer->CCER |= TIM_CCER_CC1E << ((m_channel - 1) * 4);
			
	// Set duty cycle
	this->write(m_dutyCycle);

	// Enable timer	
	m_timer->CR1 |= TIM_CR1_CEN;
	
	// Enable main output
	m_timer->BDTR |= TIM_BDTR_MOE;
}

void PwmOut :: frequency(uint32_t value)
{
	uint32_t prescaler = 1;
	
	// Timer configuration
	if((m_frequency < PWMOUT_LOW_FREQUENCY)) prescaler = (SystemCoreClock / 100000);
	
	m_timer->PSC = prescaler - 1;
	m_timer->ARR = ((SystemCoreClock / prescaler) / m_frequency) - 1;
	
	// reload prescaler and repetition counter
	m_timer->EGR |= TIM_EGR_UG;
	
	// !important
	this->write(m_dutyCycle);
}

void PwmOut :: write(uint8_t value)
{
	uint32_t pulseWidth = 0;
	
	// Overflow protection
	if(value >= PWMOUT_DUTYCYCLE_MAX) value = PWMOUT_DUTYCYCLE_MAX;
	
	m_dutyCycle = value;
	
	// Compute pulse width
	pulseWidth = (100 - m_dutyCycle) * (m_timer->ARR - 1) / 100;

	m_timer->CCR[(m_channel-1)] = pulseWidth;
}

PwmOut& PwmOut :: operator= (uint8_t value)
{
	this->write(value);
	
	return *this;
}

uint8_t PwmOut :: read()
{
	return m_dutyCycle;
}

PwmOut :: operator uint8_t()
{
	return this->read();
}

extern "C"
{	
	void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
	{		
		if((TIM1->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[0] != 0)
			{
				(*updateCallback[0])();
			}
			
			TIM1->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM2_IRQHandler(void)
	{
		if((TIM2->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[1] != 0)
			{
				(*updateCallback[1])();
			}
			
			TIM2->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM3_IRQHandler(void)
	{
		if((TIM3->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[2] != 0)
			{
				(*updateCallback[2])();
			}
			
			TIM3->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM6_DAC_IRQHandler(void)
	{
		if((TIM6->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[3] != 0)
			{
				(*updateCallback[3])();
			}
			
			TIM6->SR &= ~TIM_SR_UIF;
		}		
	}
	
	void TIM14_IRQHandler(void)
	{	
		if((TIM14->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[4] != 0)
			{
				(*updateCallback[4])();
			}
			
			TIM14->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM15_IRQHandler(void)
	{	
		if((TIM15->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[5] != 0)
			{
				(*updateCallback[5])();
			}
			
			TIM15->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM16_IRQHandler(void)
	{	
		if((TIM16->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[6] != 0)
			{
				(*updateCallback[6])();
			}
			
			TIM16->SR &= ~TIM_SR_UIF;
		}
	}
	
	void TIM17_IRQHandler(void)
	{	
		if((TIM17->SR & TIM_SR_UIF) != 0)
		{
			// Callback ?
			if(updateCallback[7] != 0)
			{
				(*updateCallback[7])();
			}
			
			TIM17->SR &= ~TIM_SR_UIF;
		}
	}
}
