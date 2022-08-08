#include <util/delay.h>
#include <avr/io.h> /* numarator zecimal decodif 7seg Demo - IO port def */
#include <avr/pgmspace.h> /* aici este definita biblioteca de macroininstructiuni utilizate pentru accesarea memoriei de program!! */
#define F_CPU 2000000UL
/* declararea structurii */
struct Stare {
	unsigned char Afisare;
	unsigned char Iesire;
	unsigned long Timp; /* multiplu de 1 ms */
	unsigned char Urmatoarea[4]; /* o lista cu starile urmatoare */
};

/* declaratie de tip pentru structura Stare */
typedef struct Stare TipStare;
//typedef struct Stare SegStare;

/* definirea starilor - o simpla numerotare */
#define treciN 0 
#define staiN  1
#define treciE 2
#define staiE  3

/*Automatul propriu zis */
TipStare AUTOMAT[4] = {
/* Iesire Timp Urmatoarea [4] */
/* pc 543210 */
{ 0b01100001,0b10100001, 8500, {treciN, staiN, treciN, staiN}},
{ 0b11111111,0b01100010, 6000, {treciE, treciE, treciE, treciE}},
{ 0b11010101,0b10001100, 8500,  {treciE, treciE, staiE, staiE}},
{ 0b11111111,0b01010100, 6000, {treciN, treciN, treciN, treciN}}
};
int main(void) {
	unsigned char Index; // index al starii curente
	unsigned int Intrare;

	/* Initializare porturi */
DDRB =  DDRB  & 0xFC ; // PB1 PB0 intrari
PORTB = PORTB | 0x03 ; // activam pe intarile utilizate rezist, interne de pull up (la Vcc)
DDRD =  DDRD  | 0xFF ; // PC5..PC0 iesiri
PORTD = PORTD & 0x7F ; // LED uri initial stinse
DDRA =  DDRA  | 0xFE ; // PA1..PA7 iesiri
PORTA = PORTA & 0xFE ; // Led-uri segment, initial stinse

Index = treciN; // indexul starea initiala

while(1) {
	PORTA = AUTOMAT[Index].Afisare;			// actualizare iesiri Segment
	PORTD = AUTOMAT[Index].Iesire;  			// actualizare iesiri Leduri
	_delay_ms(AUTOMAT[Index].Timp); 			// intarziere corespunzatoare starii
	Intrare = PINB & 0x03;          			// citire stare senzorii, port B
	Index = AUTOMAT[Index].Urmatoarea[Intrare]; // se trece la urmatoarea stare
	}
}	
