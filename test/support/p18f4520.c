/*-------------------------------------------------------------------------
 * PIC18F4520 processor header
 *
 * This software is developed by Microchip Technology Inc. and its
 * subsidiaries ("Microchip").
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 
 * 1.      Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 * 2.      Redistributions in binary form must reproduce the above 
 *         copyright notice, this list of conditions and the following 
 *         disclaimer in the documentation and/or other materials provided 
 *         with the distribution.
 * 3.      Microchip's name may not be used to endorse or promote products
 *         derived from this software without specific prior written 
 *         permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL MICROCHIP BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWSOEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * (c) Copyright 1999-2013 Microchip Technology, All rights reserved
 *-------------------------------------------------------------------------*/

#include "p18f4520.h"

volatile near unsigned char       PORTA;
volatile near union PORTAbits	  PORTAbits;

volatile near unsigned char       PORTB;
volatile near union PORTBbits	  PORTBbits;

volatile near unsigned char       PORTC;
volatile near union PORTCbits	  PORTCbits;

volatile near unsigned char       PORTD;
volatile near union PORTDbits	  PORTDbits;

volatile near unsigned char       PORTE;
volatile near union PORTEbits	  PORTEbits;


volatile near unsigned char       LATA;
volatile near struct LATAbits	  LATAbits;

volatile near unsigned char       LATB;
volatile near struct LATBbits	  LATBbits;

volatile near unsigned char       LATC;
volatile near struct LATCbits	  LATCbits;

volatile near unsigned char       LATD;
volatile near struct LATDbits	  LATDbits;

volatile near unsigned char       LATE;
volatile near struct LATEbits	  LATEbits;


volatile near unsigned char       DDRA;
volatile near union	 DDRAbits	  DDRAbits;	
volatile near unsigned char       TRISA;
volatile near union	TRISAbits	  TRISAbits;

volatile near unsigned char       DDRB;
volatile near union	 DDRBbits	  DDRBbits;	
volatile near unsigned char       TRISB;
volatile near union	TRISBbits	  TRISBbits;

volatile near unsigned char       DDRC;
volatile near union	 DDRCbits	  DDRCbits;	
volatile near unsigned char       TRISC;
volatile near union	TRISCbits	  TRISCbits;

volatile near unsigned char       DDRD;
volatile near union	 DDRDbits	  DDRDbits;	
volatile near unsigned char       TRISD;
volatile near union	TRISDbits	  TRISDbits;

volatile near unsigned char       DDRE;
volatile near union	 DDREbits	  DDREbits;	
volatile near unsigned char       TRISE;
volatile near union	TRISEbits	  TRISEbits;


volatile near unsigned char       OSCTUNE;
volatile near union OSCTUNEbits	  OSCTUNEbits;


volatile near unsigned char       PIE1;
volatile near struct PIE1bits	  PIE1bits;
volatile near unsigned char       PIR1;
volatile near struct PIR1bits	  PIR1bits;
volatile near unsigned char       IPR1;
volatile near struct IPR1bits	 IPR1bits;

volatile near unsigned char       PIE2;
volatile near union PIE2bits	  PIE2bits;
volatile near unsigned char       PIR2;
volatile near union PIR2bits	  PIR2bits;
volatile near unsigned char       IPR2;
volatile near union IPR2bits	 IPR2bits;

volatile near unsigned char       EECON1;
volatile near struct EECON1bits	  EECON1bits;

volatile near unsigned char       EECON2;
volatile near unsigned char       EEDATA;
volatile near unsigned char       EEADR;

volatile near unsigned char       RCSTA;
volatile near union RCSTAbits 	  RCSTAbits;

volatile near unsigned char       TXSTA;
volatile near struct TXSTAbits	  TXSTAbits;

volatile near unsigned char       TXREG;
volatile near unsigned char       RCREG;
volatile near unsigned char       SPBRG;
volatile near unsigned char       SPBRGH;

volatile near unsigned char       T3CON;
volatile near union T3CONbits     T3CONbits;
volatile near unsigned char       TMR3L;
volatile near unsigned char       TMR3H;

volatile near unsigned char       CMCON;
volatile near union CMCONbits 	CMCONbits;
volatile near unsigned char       CVRCON;
volatile near union CVRCONbits 	CVRCONbits;

volatile near unsigned char       ECCP1AS;
volatile near union ECCP1ASbits 	ECCP1ASbits;	
volatile near unsigned char       ECCPAS;
volatile near union ECCPASbits 		ECCPASbits;
volatile near unsigned char       ECCP1DEL;
volatile near union ECCP1DELbits 	ECCP1DELbits;

volatile near unsigned char       PWM1CON;
volatile near union PWM1CONbits	PWM1CONbits;
volatile near unsigned char       BAUDCON;
volatile near union BAUDCONbits BAUDCONbits;
volatile near unsigned char       BAUDCTL;
volatile near union BAUDCTLbits BAUDCTLbits;
volatile near unsigned char       CCP2CON;
volatile near union CCP2CONbits CCP2CONbits;
volatile near unsigned            CCPR2;
volatile near unsigned char       CCPR2L;
volatile near unsigned char       CCPR2H;
volatile near unsigned char       CCP1CON;
volatile near union CCP1CONbits CCP1CONbits;
volatile near unsigned            CCPR1;
volatile near unsigned char       CCPR1L;
volatile near unsigned char       CCPR1H;
volatile near unsigned char       ADCON2;
volatile near union ADCON2bits ADCON2bits;
volatile near unsigned char       ADCON1;
volatile near union ADCON1bits ADCON1bits;
volatile near unsigned char       ADCON0;
volatile near union ADCON0bits	ADCON0bits;
volatile near unsigned            ADRES;
volatile near unsigned char       ADRESL;
volatile near unsigned char       ADRESH;
volatile near unsigned char       SSPCON2;
volatile near union SSPCON2bits SSPCON2bits;
volatile near unsigned char       SSPCON1;
volatile near union SSPCON1bits SSPCON1bits;
volatile near unsigned char       SSPSTAT;
volatile near union SSPSTATbits	SSPSTATbits;
volatile near unsigned char       SSPADD;
volatile near unsigned char       SSPBUF;
volatile near unsigned char       T2CON;
volatile near union T2CONbits T2CONbits;
volatile near unsigned char       PR2;
volatile near unsigned char       TMR2;
volatile near unsigned char       T1CON;
volatile near union T1CONbits T1CONbits;
volatile near unsigned char       TMR1L;
volatile near unsigned char       TMR1H;
volatile near unsigned char       RCON;
volatile near union RCONbits RCONbits;
volatile near unsigned char       WDTCON;
volatile near union WDTCONbits WDTCONbits;
volatile near unsigned char       HLVDCON;
volatile near union HLVDCONbits HLVDCONbits;
volatile near unsigned char       LVDCON;
volatile near union LVDCONbits LVDCONbits;
volatile near unsigned char       OSCCON;
volatile near union OSCCONbits OSCCONbits;
volatile near unsigned char       T0CON;
volatile near union T0CONbits T0CONbits;
volatile near unsigned char       TMR0L;
volatile near unsigned char       TMR0H;
         near unsigned char       STATUS;
         near struct STATUSbits STATUSbits;

          near unsigned            FSR2;
          near unsigned char       FSR2L;
          near unsigned char       FSR2H;
 volatile near unsigned char       PLUSW2;
 volatile near unsigned char       PREINC2;
 volatile near unsigned char       POSTDEC2;
 volatile near unsigned char       POSTINC2;
          near unsigned char       INDF2;
          near unsigned char       BSR;
          near unsigned            FSR1;
          near unsigned char       FSR1L;
          near unsigned char       FSR1H;
 volatile near unsigned char       PLUSW1;
 volatile near unsigned char       PREINC1;
 volatile near unsigned char       POSTDEC1;
 volatile near unsigned char       POSTINC1;
          near unsigned char       INDF1;
          near unsigned char       W;
          near unsigned char       WREG;
          near unsigned            FSR0;
          near unsigned char       FSR0L;
          near unsigned char       FSR0H;
 volatile near unsigned char       PLUSW0;
 volatile near unsigned char       PREINC0;
 volatile near unsigned char       POSTDEC0;
 volatile near unsigned char       POSTINC0;
          near unsigned char       INDF0;
 volatile near unsigned char       INTCON3;
 volatile near union INTCON3bits	INTCON3bits; 
 
 volatile near unsigned char       INTCON2;
 volatile near union INTCON2bits	INTCON2bits;
 
 volatile near unsigned char       INTCON;
 volatile near union INTCONbits		INTCONbits;
 
          near unsigned            PROD;
          near unsigned char       PRODL;
          near unsigned char       PRODH;
 volatile near unsigned char       TABLAT;
 volatile near unsigned SHORT_LONG TBLPTR;
 volatile near unsigned char       TBLPTRL;
 volatile near unsigned char       TBLPTRH;
 volatile near unsigned char       TBLPTRU;
 volatile near unsigned SHORT_LONG PC;
 volatile near unsigned char       PCL;
 volatile near unsigned char       PCLATH;
 volatile near unsigned char       PCLATU;
 volatile near unsigned char       STKPTR;
 volatile near union STKPTRbits		STKPTRbits;
 
          near unsigned SHORT_LONG TOS;
          near unsigned char       TOSL;
          near unsigned char       TOSH;
          near unsigned char       TOSU;