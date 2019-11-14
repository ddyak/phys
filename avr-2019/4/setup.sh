avr-gcc -g -O1 -mmcu=atmega2560 -o out.elf uart.c
sudo avrdude -D -v -p m2560 -c wiring -P /dev/ttyUSB0 -U flash:w:"out.elf"
sudo stty 9600 -F /dev/ttyUSB0 raw -echo

