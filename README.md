
# MOS 6502 Emulator

### Author
Emanuel Aracena

## Description
An emulator of the MOS-6502 processor written in C++.
	
### (Tested/Target) Platforms 
	Linux 64-bit distributions
		
### Dependencies
	g++ (compiler)
	make (for the Makefile)
	SFML (most up-to-date version)

### Build Instructions
To build everything, type:
	`make clean`	
	`make`

To build debugger only, type:
	`make clean`
	`make debugger`

### Run instructions (using FILE as the name of file)
To run with selected FILE, type:
	`./6502-emu FILE --debug`     (currently, only debug mode is allowed)


### Clean up
To remove all .o and compiled executables, type:
	`make clean`

### Issues
* To be updated when most/all components have been implemented 
