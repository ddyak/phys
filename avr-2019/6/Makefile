load: out.elf
	sudo avrdude -D -v -p m2560 -c wiring -P /dev/ttyUSB0 -U flash:w:"out.elf"

out.elf: main.c spi.c game.c
	avr-gcc -g -O3 -mmcu=atmega2560 -o out.elf main.c spi.c game.c


