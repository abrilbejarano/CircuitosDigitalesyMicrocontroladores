#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define TICKS_POR_MS (F_CPU/1000)/4

uint32_t reloj = 0;
int8_t posicion_leds, sentido_leds;

void delay_1ms();
void actualizar_leds(uint8_t);
void actualizar_neopixel();

int main(void)
{
    DDRC &= ~ ( (1<<PORTC1) | (1<<PORTC0) ); // PC1 y PC0 son entradas
	DDRB |= (1<<PORTB0); // PB0 es salida
	DDRD = 0xFF; // Todos los pines de D son salidas
	PORTC |= ( (1<<PORTC1) | (1<<PORTC0) );
	
	uint8_t secuencia_leds = 0;	
	uint8_t secuencia_neopixel = 0;
	
    while (1) 
    {
		// LEE BOTONES, CAMBIA LA SECUENCIA
		if ( !(PINC & (1<<PORTC0)) ) {
			if (secuencia_leds){
				posicion_leds = 0;
				secuencia_leds = !secuencia_leds;
			} else {
				posicion_leds = 7;
				sentido_leds = 0;
				secuencia_leds = !secuencia_leds;
			}
			while ( !(PINC & (1<<PORTC0)) );
		}
		if ( !(PINC & (1<<PORTC1)) ) {
			secuencia_neopixel = !secuencia_neopixel;
			while ( !(PINC & (1<<PORTC1)) );
		}
		
		if (!(reloj%100)){
			actualizar_leds(secuencia_leds);
		}
		
		if (!(reloj%150)){
			actualizar_neopixel();
		}
		
		delay_1ms();
    }
}

void delay_1ms() {
	for (uint16_t i=0; i<TICKS_POR_MS; i++);
	
	reloj++;
}

void actualizar_leds(uint8_t secuencia) {
	
	if (!secuencia){	// SECUENCIA A
		PORTD = (1<<posicion_leds);
		posicion_leds++;
		
		if (posicion_leds>7) {
			posicion_leds = 0;
		}
	} else {			// SECUENCIA B
		PORTD = (1<<posicion_leds);
		
		if (!sentido_leds){
			posicion_leds--;
			if (posicion_leds<0) {
				posicion_leds = 1;
				sentido_leds = !sentido_leds;
			}
		} else {
			posicion_leds++;
			if (posicion_leds>7) {
				posicion_leds = 6;
				sentido_leds = !sentido_leds;
			}
		}
	}
}

void actualizar_neopixel(){
	
}
