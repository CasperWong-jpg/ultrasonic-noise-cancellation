#include <avr/power.h>

unsigned long flag = 0;

void setup() {
  /// TODO: Create hash table that maps each port to corresponding MOLEX output
  /// TODO: To simplify below code, just make all ports (except E) low outputs. Need mapping for IRQ
  
  // Set top 4 MOLEXs as output. This includes everything but pins 0 (rx), 1 (tx), 2-3 (sync), 5 & 14 (empty)
  DDRB = 0xF0 ;  // 4 pins
  DDRD = 0x0F ;  // 4 pins
  DDRF = 0xFF ;  // 8 pins
  DDRH = 0x7B ;  // 6 pins
  DDRJ = 0x03 ;  // 2 pins
  DDRK = 0xFF ;  // 8 pins
  
  // Set bottom 32 digital pins (21-53) as output. These correspond to bottom 4 MOLEXs
  DDRA = 0xFF ;  // 8 pins
  DDRB = 0X0F ;  // 4 pins
  DDRC = 0xFF ;  // 8 pins
  DDRD = 0x80 ;  // 1 pin
  DDRG = 0x07 ;  // 3 pins
  DDRL = 0XFF ;  // 8 pins
  
  // Set all pins to low
  PORTA = PORTB = PORTC = PORTD = PORTF = PORTG = PORTH = PORTJ = PORTK = PORTL = 0x00 ;
  
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
  
  // Initialize timer 1 at 40 Khz
  TCCR1A  = 0             ;  // Reset control registers
  TCCR1B  = 0             ;
  TCNT1   = 0             ;  // Reset counter
  
  TCCR1B |= bit (WGM12)   ;  // CTC mode. # Page 145 #
  OCR1A   = 199           ;  // Count up to 200 (zero relative). # Page 159 #
  TIMSK1 |= bit (OCIE1A)  ;  // Interrupt on Timer 1 compare match. # Page 161 #
  TCCR1B |= bit (CS10)    ;  // Start clock. Prescaler of 1. # Page 157 #

  // Initialize timer 3 at 20 Khz. Timers 3, 4, 5 are identical to timer 1
  TCCR3A  = 0             ;  // Reset control registers
  TCCR3B  = 0             ;
  TCNT3   = 0             ;  // Reset counter
  
  TCCR3B |= bit (WGM32)   ;  // CTC mode. # Page 145 #
  OCR3A   = 399           ;  // Count up to 400 (zero relative). # Page 159 #
  TIMSK3 |= bit (OCIE3A)  ;  // Interrupt on Timer 1 compare match. # Page 161 #
  TCCR3B |= bit (CS30)    ;  // Start clock. Prescaler of 1. # Page 157 #
  
  interrupts()            ;  // enable all interrupts
}


ISR (TIMER1_COMPA_vect)
{
  // Invert signal on bottom pins
  // Using direct port manipulation. Takes 2 clock cycles as opposed to ~50 from digitalWrite
  PORTA ^= 0xFF ;  // 8 pins
  PORTB ^= 0X0F ;  // 4 pins
  PORTC ^= 0xFF ;  // 8 pins
  PORTD ^= 0x80 ;  // 1 pin
  PORTG ^= 0x07 ;  // 3 pins
  PORTL ^= 0XFF ;  // 8 pins
}


ISR (TIMER3_COMPA_vect)
{
  flag++ ;
  // Invert signal on top pins
  PORTB ^= 0xF0 ;  // 4 pins
  PORTD ^= 0x0F ;  // 4 pins
  PORTF ^= 0xFF ;  // 8 pins
  PORTH ^= 0x7B ;  // 6 pins
  PORTJ ^= 0x03 ;  // 2 pins
  PORTK ^= 0xFF ;  // 8 pins
}

void loop() { }
