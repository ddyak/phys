.include "./m2560def.inc"

.equ PRE_MAX_COLUMNS = 1
.equ MAX_COLUMNS = 2
.equ MAX_STRINGS = 2

.def buttons = r16
.def temp = r17
.def temp2 = r18
.def leds = r19
.def prev = r20
.def column = r21
.def prevColumn = r22

.org 0x0
  jmp Reset

.org 0x001e
  jmp TIM2_OVF

Reset:
    ldi temp, 0xFF
    out DDRA, temp

    ldi temp, 0b00000111
    sts TCCR2B, temp

    ldi temp, 0b00000001
    sts TIMSK2, temp
   
    ldi leds, 0x00

  ldi temp, 0x00
  out DDRC, temp
  out PORTC, temp
    sei

Loop:
    rjmp Loop

TIM2_OVF:
    cli
  ldi buttons, 0
  ldi column, 0b00000001
  ldi prevColumn, 0b00000010

next_column:
////////INIT_PORT_STATE_FOR_COLUMN/////////////////
  in temp, DDRD
  or temp, column
  out DDRD, temp

  in temp, PORTD
  or temp, column
  out PORTD, temp

  in temp, PORTD
  mov temp2, prevColumn
  com temp2
  and temp, temp2
  out PORTD, temp

  in temp, DDRD
  mov temp2, prevColumn
  com temp2
  and temp, temp2
  out DDRD, temp
/////////////////DUMP_COLUMN_STATE////////////
  in temp, PINC
  eor buttons, temp
////////////////PREPARE FOR NEXT ITERATION///////////////////
  lsl column
  lsl prevColumn
  sbrc column, MAX_COLUMNS
    jmp output
  sbrc column, PRE_MAX_COLUMNS
    ldi prevColumn, 0x01

  ldi temp, MAX_STRINGS
string_shifter:
  lsl buttons
  dec temp
  brne string_shifter

  jmp next_column

output:
    mov temp, buttons

  ldi temp2, 0x0F
  and prev, temp2
  com prev
  and buttons, prev
  
  eor leds, buttons
    out PORTA, leds
  mov prev, temp

  sei
    reti

delay:
    ldi  r22, 104
    ldi  r21, 229
L1: dec  r21
    brne L1
    dec  r22
    brne L1
  ret

  
//  call delay
//  sbic PINC, PC5
//    sbrs buttons, 2 //0b00000100
//      cbr buttons, 0b00000100
