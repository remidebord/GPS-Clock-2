#ifndef __TIMER_H
#define __TIMER_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"
#include "GPIO.h"

/* defines ----------------------------------------------------------------- */
#define PWMOUT_DUTYCYCLE_MAX 100 
#define PWMOUT_LOW_FREQUENCY 800

/* class ------------------------------------------------------------------- */
class Timer
{
	protected:
		
		TIM_TypeDef* m_timer;
	
	public:
		
		Timer(TIM_TypeDef* timer);
		void start(void);
		void stop(void);
		void reset(void);
		uint32_t read(void);
		uint32_t read_ms(void);
		uint32_t read_us(void);
	
		void attach(void(*f)(void));
		void detach(void);
};

class Ticker : public Timer
{
	private:
		
	public:
		
		Ticker(TIM_TypeDef* timer);
		void attach_ms(void(*f)(void), uint32_t ms);
		void attach_us(void(*f)(void), uint32_t us);
};

class Timeout : public Timer
{
	private:
	
	public:
		
		Timeout(TIM_TypeDef* timer);
		void attach_ms(void(*f)(void), uint32_t ms);
		void attach_us(void(*f)(void), uint32_t us);
		void detach(void);
		void start(void);
};

class PwmOut : public GPIO, public Timer
{
	private:
		
		TIM_TypeDef* m_timer;
	
		uint32_t m_frequency;
		uint8_t m_dutyCycle;
		TimerChannel m_channel;
	
	public:
		
		PwmOut(PinName pin, uint32_t frequency, TIM_TypeDef* timer, TimerChannel channel);
		void frequency(uint32_t value);     // Modify frequency
		void write(uint8_t value);          // Write
		PwmOut& operator= (uint8_t value);  // Write (shorthand)
		uint8_t read();                     // Read
		operator uint8_t();
};

#endif /* __TIMER_H */
