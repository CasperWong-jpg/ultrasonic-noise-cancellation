#include <avr/power.h>

byte pattern = 0x00;

void setup() {
  // Generate signal of 40khz in pin 2
  DDRE = 0b00010000 ;  // Set as output port
  PORTE = pattern ;  // Set as low signal
  
  // disable everything that we do not need
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  power_usart1_disable();
  power_usart2_disable();
  power_usart3_disable();
  power_usart0_disable();
  
  // 16 MHz clock (125 ns per 2 ticks / full clock cycle). Count up to 200 for 40 kHz. Interrupt on compare match.
  noInterrupts()          ;  // Disable all interrupts
  
  // Initialize timer 2
  TCCR2A  = 0             ;  // Reset control registers
  TCCR2B  = 0             ;
  TCNT2   = 0             ;  // Reset counter
  
  TCCR2A |= bit (WGM21)   ;  // CTC mode. # Page 184 #
  OCR2A   = 199           ;  // Count up to 200 (zero relative). # Page 186 #
  TIMSK2 |= bit (OCIE2A)  ;  // Interrupt on Timer 2 compare match. # Page 188.
  TCCR2B |= bit (CS20)    ;  // Start clock. Prescaler of 1. # Page 185 #
  
  interrupts()            ;  // enable all interrupts

  while (true);  // Avoid going into the loop
}


ISR (TIMER2_COMPA_vect)
{
  // Invert signal on pin 2
  pattern ^= 0b00010000 ;
  PORTE = pattern ;
}


void loop() { }
