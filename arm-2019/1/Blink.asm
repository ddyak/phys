.include "m2560def.inc"

.org 0X0000
    rjmp init

.org 0x0028
    jmp TIM1_OVF ; Timer1 Overflow Handler

init:
    ldi R16, LOW(RAMEND)
    out SPL, R16
    ldi R16, HIGH(RAMEND)
    out SPH, R16; stack initialization

    sbi DDRB, PB7
    sbi PORTB, PB7
  
    ldi R17, 0b00000101
    sts TCCR1B, R17 ; divide tacts by 1024

    ldi R17, 0b11111000
    sts TCNT1H, R17
    ldi R17, 0b00000000
    sts TCNT1L, R17

    ldi R17,0b00000100
    sts TIMSK1, R17 ; Разрешить прерывание при переполнении

    sei; Разрешить обрабатывать прерывания прерывания cli - запретить

main_loop:
    nop
    rjmp main_loop

TIM1_OVF:
    cli
    sbic PINB, PB7    ; skip next if PB = 0
        cbi PORTB, PB7
    sbis PINB, PB7
        sbi PORTB, PB7  ; skip next if PB = 1
  
    ldi R17, 0b11100000 
    sts TCNT1H, R17
    ldi R17, 0b11111111 
    sts TCNT1L, R17
    sei
reti
