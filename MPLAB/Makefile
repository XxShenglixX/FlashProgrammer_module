# MPLAB IDE generated this makefile for use with GNU make.
# Project: main.mcp
# Date: Sat Sep 06 02:21:07 2014

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

main.cof : DelayedWrite.o FlashBuffer.o masterMain.o SerialInterrupt.o spiMaster.o UART.o TLV.o
	$(LD) /p18F4520 /l"C:\Program Files (x86)\Microchip\mplabc18\v3.47\lib" "DelayedWrite.o" "FlashBuffer.o" "masterMain.o" "SerialInterrupt.o" "spiMaster.o" "UART.o" "TLV.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"main.cof" /M"main.map" /W

DelayedWrite.o : ../src/DelayedWrite.c ../src/FlashBuffer.h ../src/spiMaster.h ../src/DelayedWrite.h ../src/DelayedWrite.c ../src/Utils.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\DelayedWrite.c" -fo="DelayedWrite.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

FlashBuffer.o : ../src/FlashBuffer.c ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/Delays.h ../src/FlashBuffer.h ../src/spiMaster.h ../src/FlashBuffer.c ../src/Utils.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\FlashBuffer.c" -fo="FlashBuffer.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

masterMain.o : ../src/masterMain.c ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/spi.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/delays.h ../src/FlashBuffer.h ../src/spiMaster.h ../src/DelayedWrite.h ../src/UART.h ../src/SerialInterrupt.h ../src/TLV.h ../src/masterMain.c ../src/18c.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/P18F4520.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/pconfig.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h ../src/masterMain.h ../src/Utils.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\masterMain.c" -fo="masterMain.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

SerialInterrupt.o : ../src/SerialInterrupt.c ../src/UART.h ../src/FlashBuffer.h ../src/SerialInterrupt.h ../src/DelayedWrite.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stdio.h ../src/SerialInterrupt.c ../src/Utils.h ../src/18c.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/P18F4520.h ../src/masterMain.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stdarg.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stddef.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\SerialInterrupt.c" -fo="SerialInterrupt.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

spiMaster.o : ../src/spiMaster.c ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/spi.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/delays.h ../src/FlashBuffer.h ../src/spiMaster.h ../src/spiMaster.c ../src/18c.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/P18F4520.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/pconfig.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h ../src/Utils.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\spiMaster.c" -fo="spiMaster.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

UART.o : ../src/UART.c ../src/UART.h ../src/UART.c ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h ../src/Utils.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\UART.c" -fo="UART.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

TLV.o : ../src/TLV.c ../src/UART.h ../src/FlashBuffer.h ../src/SerialInterrupt.h ../src/TLV.h ../src/DelayedWrite.h ../src/spiMaster.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stdio.h ../src/TLV.c ../src/Utils.h ../src/18c.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/P18F4520.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stdarg.h ../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/stddef.h
	$(CC) -p=18F4520 "C:\TDD Mini Project\FlashProgrammer_module\src\TLV.c" -fo="TLV.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "DelayedWrite.o" "FlashBuffer.o" "masterMain.o" "SerialInterrupt.o" "spiMaster.o" "UART.o" "TLV.o" "main.cof" "main.hex"

