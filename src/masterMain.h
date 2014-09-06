#ifndef masterMain_H
#define masterMain_H

#define  configurePorts()   TRISD = TRISD | 0x10
#define  runTargetPin       PORTDbits.RD3
#define  resetTarget        PORTDbits.RD4
#define  stopTarget()       runTargetPin = 0
#define  runTarget()        runTargetPin = 1
#define  BaudratePrescaler  25

#define  enableInterrupt()  do{                     	\
								INTCONbits.PEIE = 1;    \
								INTCONbits.GIE = 1;     \
                            }while(0)

#endif // masterMain_H
