avr-gcc -g -O1 -mmcu=atmega2560 -o out.elf blink.c
sudo avrdude -D -v -p m2560 -c wiring -P /dev/ttyACM0 -U flash:w:"out.elf"
