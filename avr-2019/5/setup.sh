avr-gcc -u vfprintf -lprintf_flt -lm -g -O1 -mmcu=atmega2560 -o out.elf i2c.c
sudo avrdude -D -v -p m2560 -c wiring -P /dev/ttyUSB0 -U flash:w:"out.elf"
sudo stty 9600 -F /dev/ttyUSB0 raw -echo

