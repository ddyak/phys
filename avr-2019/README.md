atmega2560

avrdude.exe -D -p m2560 -c wiring -P COM5 -U flash:w:Blink.hex:a
