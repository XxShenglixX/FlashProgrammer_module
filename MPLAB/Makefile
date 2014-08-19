# MPLAB IDE generated this makefile for use with GNU make.
# Project: main.mcp
# Date: Sun Aug 17 18:42:50 2014

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

main.cof : spiMaster.o masterMain.o DelayedWrite.o FlashBuffer.o SerialInterrupt.o UART.o
	$(LD) /p18F4520 /l"C:\Program Files (x86)\Microchip\mplabc18\v3.47\lib" "spiMaster.o" "masterMain.o" "DelayedWrite.o" "FlashBuffer.o" "SerialInterrupt.o" "UART.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"main.cof" /M"main.map" /W

spiMaster.o : spiMaster.c ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/spi.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/delays.h FlashBuffer.h spiMaster.h spiMaster.c ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/pconfig.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h Utils.h
	$(CC) -p=18F4520 "spiMaster.c" -fo="spiMaster.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

masterMain.o : masterMain.c ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/spi.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/delays.h FlashBuffer.h spiMaster.h DelayedWrite.h UART.h SerialInterrupt.h masterMain.c ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/P18F4520.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/pconfig.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h Utils.h
	$(CC) -p=18F4520 "masterMain.c" -fo="masterMain.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

DelayedWrite.o : DelayedWrite.c FlashBuffer.h DelayedWrite.h DelayedWrite.c Utils.h
	$(CC) -p=18F4520 "DelayedWrite.c" -fo="DelayedWrite.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

FlashBuffer.o : FlashBuffer.c FlashBuffer.h spiMaster.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/Delays.h FlashBuffer.c Utils.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18cxxx.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h
	$(CC) -p=18F4520 "FlashBuffer.c" -fo="FlashBuffer.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

SerialInterrupt.o : SerialInterrupt.c UART.h SerialInterrupt.h SerialInterrupt.c Utils.h ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h
	$(CC) -p=18F4520 "SerialInterrupt.c" -fo="SerialInterrupt.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

UART.o : UART.c UART.h UART.c ../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.46/h/p18f4520.h Utils.h
	$(CC) -p=18F4520 "UART.c" -fo="UART.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "spiMaster.o" "masterMain.o" "DelayedWrite.o" "FlashBuffer.o" "SerialInterrupt.o" "UART.o" "main.cof" "main.hex"

