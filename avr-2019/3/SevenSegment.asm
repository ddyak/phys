.include "m2560def.inc"

.def temp = r18
.def offset = r20
.def shift = r21
.def current = r16
.def dig1 = r22
.def dig2 = r23
.def dig3 = r24
.def dig4 = r25

.org 0X0000
  rjmp reset

.org 0x001E
  rjmp TIM2_OVF

.org 0x002E
  rjmp TIM1_OVF

reset:
  ldi R16, LOW(RAMEND)
  out SPL, R16
  ldi R16, HIGH(RAMEND)
  out SPH, R16;

  ldi R16, 0b11111111
  out DDRA, R16
  ldi R17, 0b00111111
  out PORTA, R17

  out DDRC, R16
  ldi R16, 0b00001011
  out PORTC, R16

  // Timer for dynamic shift
  ldi R17, 0b00000100
  sts TCCR2B, R17
  ldi R17, 0b00000001
  sts TIMSK2, R17  
  
  // Timer for increment
  ldi R17, 0b00000101
    sts TCCR1B, R17
  ldi R17,0b00000100
    sts TIMSK1, R17
  ldi temp, 0xf5
  sts TCNT1H, temp
  sts TCNT1L, temp

  ldi shift, 0b11111011
  ldi dig1, 0b00111111
  ldi dig2, 0b00000110
  ldi dig3, 0b01011011
  ldi dig4, 0b01001111

  sei

main_loop:
  rjmp main_loop
  
Digits:
.db 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111

TIM1_OVF:
  cli
  ldi temp, 0xe5
  sts TCNT1H, temp
  sts TCNT1L, temp  
  cpi offset, 10
  breq Init

ReadArray:
  ldi ZH, High(Digits*2)
  ldi ZL, Low(Digits*2)
  add ZL, offset      ;прибавление внутр. адреса
  lpm            ;загрузка из ПЗУ в R0
  mov temp,R0
  inc offset
  rjmp Output  

Init: 
  ldi offset, 0
  rjmp ReadArray

Output:
  mov dig4, dig3
  mov dig3, dig2
  mov dig2, dig1
  mov dig1, temp
  sei
  reti

TIM2_OVF:
  cli
  cpi shift, 0b11111110
    brne shift1
  ldi shift, 0b11111101
  jmp sh
shift1:
  cpi shift, 0b11111101
    brne shift2
  ldi shift, 0b11111011
  jmp sh
shift2:
  cpi shift, 0b11111011
    brne shift3
  ldi shift, 0b11110111
  jmp sh
shift3:
  cpi shift, 0b11110111
    brne sh
  ldi shift, 0b11111110

sh:
  cpi shift, 0b11111110
    brne next1
  mov current, dig1
next1:
  cpi shift, 0b11111101
    brne next2
  mov current, dig2
next2:
  cpi shift, 0b11111011
    brne next3
  mov current, dig3
next3:
  cpi shift, 0b11110111
    brne fi
  mov current, dig4
fi:
  out PORTA, current
  out PORTC, shift
  sei  
  reti
