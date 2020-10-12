#ifndef __ANALOG_H
#define __ANALOG_H

/* includes ---------------------------------------------------------------- */
#include "GPIO.h"

/* defines ----------------------------------------------------------------- */
#define ADC_CHANNELS_MAX 19
#define ADC_CHANNEL_GPIOA_OFFSET 0  /* IN[0:7] */
#define ADC_CHANNEL_GPIOB_OFFSET 8  /* IN[8:9] */
#define ADC_CHANNEL_GPIOC_OFFSET 10 /* IN[10:15] */

/* class ------------------------------------------------------------------- */
class AnalogIn : public GPIO
{
	private:
		
		uint8_t m_channel;
		
		static __IO uint8_t m_channels;
		static __IO uint8_t m_rank[ADC_CHANNELS_MAX];
		static __IO uint16_t m_value[ADC_CHANNELS_MAX];
		
		static void adc(void);
		static void dma(void);
		static void sort(uint8_t* buffer, uint8_t size);
		static uint8_t channel(PinName pin);
	
	public:
	
		AnalogIn(PinName pin);
	
		uint16_t read_b();
		uint16_t read();
		operator uint16_t();	// Read (shorthand)
};

class AnalogOut : public GPIO
{
	private:
	
	public:
	
		AnalogOut(PinName pin);
	
		void write(uint16_t value);
		uint16_t read(void);
		AnalogOut& operator= (uint16_t value);  // Write (shorthand)
		operator uint16_t();                    // Read (shorthand)
};

#endif
