.include "m2560def.inc"

.org 0X0000
	rjmp reset
  
.org 0x0028
	jmp TIM1_OVF

reset:
	ldi R16, LOW(RAMEND)
	out SPL, R16
	ldi R16, HIGH(RAMEND)
	out SPH, R16;

	ldi R16, (1<<WGM00) | (1<<WGM01) | (1<<COM0A1) // WGM00 WGM01 == 11 => PWM && t = 0 -> 0, t = A -> 1
	out TCCR0A, R16
	ldi R16, (1<<CS00) 
	out TCCR0B, R16

  // При совпадении счетчика TCNT0 с OCR0A выставляем сигнал
	out OCR0A, R16

	ldi R17, (1<<CS10)
 	sts TCCR1B, R17 
	ldi R17, 0x00
	sts TCNT1H, R17
	ldi R17, 0x00
	sts TCNT1L, R17
	ldi R17, (1<<OCIE1A)
 	sts TIMSK1, R17 
  
	sbi DDRB, PB7
	ldi R19, 1
	ldi R20, 0x00

	sei

main_loop:
	nop
	jmp main_loop  

TIM1_OVF:
	cli
	ldi R16, 1
	cpse R19, R16
 	jmp else1
	inc R20
else1:
	ldi R16, 0
	cpse R19, R16
	jmp else2    
	dec R20
else2:
	ldi R16, 0xFF
	cpse R20, R16 
	jmp next
	ldi R19, 0
next:
	ldi R16, 0x00
	cpse R20, R16 
    jmp next2
	ldi R19, 1
next2:
	out OCR0A, R20
    sei
reti
