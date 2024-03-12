#include <avr/power.h>

byte pattern = 0x00;

void setup() {
  // Generate signals in digital pins 2 and 3
  DDRE = 0b00110000 ;  // Set as output port
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
  
  // Initialize timer 1 at 39.8 Khz
  TCCR1A  = 0             ;  // Reset control registers
  TCCR1B  = 0             ;
  TCNT1   = 0             ;  // Reset counter
  
  TCCR1B |= bit (WGM12)   ;  // CTC mode. # Page 145 #
  OCR1A   = 200           ;  // Count up to 199 (zero relative). # Page 159 #
  TIMSK1 |= bit (OCIE1A)  ;  // Interrupt on Timer 1 compare match. # Page 161 #
  TCCR1B |= bit (CS10)    ;  // Start clock. Prescaler of 1. # Page 157 #

  // Initialize timer 3 at 40.2 Khz. Timers 3, 4, 5 are identical to timer 1
  TCCR3A  = 0             ;  // Reset control registers
  TCCR3B  = 0             ;
  TCNT3   = 0             ;  // Reset counter
  
  TCCR3B |= bit (WGM32)   ;  // CTC mode. # Page 145 #
  OCR3A   = 198           ;  // Count up to 201 (zero relative). # Page 159 #
  TIMSK3 |= bit (OCIE3A)  ;  // Interrupt on Timer 1 compare match. # Page 161 #
  TCCR3B |= bit (CS30)    ;  // Start clock. Prescaler of 1. # Page 157 #
  
  interrupts()            ;  // enable all interrupts

  while (true);  // Avoid going into the loop
}


ISR (TIMER1_COMPA_vect)
{
  // Invert signal on pin 2
  // Using direct port manipulation. Takes 2 clock cycles as opposed to ~50 from digitalWrite
  PORTE ^= 0b00010000 ;
}


ISR (TIMER3_COMPA_vect)
{
  // Invert signal on pin 3
  PORTE ^= 0b00100000 ;
}

void loop() { }
