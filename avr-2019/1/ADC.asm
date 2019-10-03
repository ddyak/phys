.include "m2560def.inc"

.org 0X0000
	rjmp reset

reset:
	ldi R16, LOW(RAMEND)
	out SPL, R16
	ldi R16, HIGH(RAMEND)
	out SPH, R16;

	ldi R16, (1<<WGM00)|(1<<WGM01)|(1<<COM0A1) // WGM00 WGM01 == 11 => PWM && t = 0 -> 0, t = A -> 1
	out TCCR0A, R16
	ldi R16, (1<<CS00) 
	out TCCR0B, R16
  
	sbi DDRB, PB7

	ldi R16, (1<<REFS0)|(1<<ADLAR)
	sts ADMUX, R16 

	ldi R16, (1<<ADEN)|(1<<ADSC)|(1<<ADATE)
	sts ADCSRA, R16


main_loop:
	lds R16, ADCH
	out OCR0A, R16
	jmp main_loop
