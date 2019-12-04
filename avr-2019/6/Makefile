load: out.elf ttyUSB0
	sudo avrdude -D -v -p m2560 -c wiring -P /dev/ttyUSB0 -U flash:w:"out.elf"

ttyUSB0: 
	sudo stty 9600 -F /dev/ttyUSB0 raw -echo

out.elf: main.c spi.c game.c uart.c
	avr-gcc -g -O3 -mmcu=atmega2560 -o out.elf main.c spi.c game.c uart.c


