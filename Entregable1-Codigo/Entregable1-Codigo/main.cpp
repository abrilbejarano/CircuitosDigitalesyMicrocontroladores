#define F_CPU 16000000UL
#define TICKS_POR_MS (F_CPU/1000)/4

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

uint32_t reloj = 0;
int8_t posicion_leds, sentido_leds;
uint8_t posicion_neopixel, paridad_neopixel;

void delay_1ms();
void actualizar_leds(uint8_t);
void actualizar_neopixel(uint8_t);

#define NOP() __asm__ __volatile__("nop")


void led_azul()
{
	int i;
	for(i=0;i<16;i++){
		PORTB |= (1<<PB0);
		NOP();NOP(); // ~0.4us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.85us
	}

	for(i=0;i<8;i++){
		PORTB |= (1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.8us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP(); // ~0.45us
	}
}
void led_verde()
{
	int i;
	for(i=0;i<8;i++){
		PORTB |= (1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.8us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP(); // ~0.45us
	}
	for(i=0;i<16;i++){
		PORTB |= (1<<PB0);
		NOP();NOP(); // ~0.4us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.85us
	}
}
void led_rojo(){
	int i;
	for(i=0;i<8;i++){
		PORTB |= (1<<PB0);
		NOP();NOP(); // ~0.4us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.85us
	}
	for(i=0;i<8;i++){
		PORTB |= (1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.8us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP(); // ~0.45us
	}
	for(i=0;i<8;i++){
		PORTB |= (1<<PB0);
		NOP();NOP(); // ~0.4us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.85us
	}
}
void led_negro(){
	int i;
	for(i=0;i<24;i++){
		PORTB |= (1<<PB0);
		NOP();NOP(); // ~0.4us
		PORTB &= ~(1<<PB0);
		NOP();NOP();NOP();NOP();NOP();NOP(); // ~0.85us
	}
}


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
			} else {
				posicion_leds = 7;
				sentido_leds = 0;
				
			}
			secuencia_leds = !secuencia_leds;
			while ( !(PINC & (1<<PORTC0)) );
		}
		if ( !(PINC & (1<<PORTC1)) ) {
			if (secuencia_neopixel){
				paridad_neopixel=0;
			} else {
				posicion_neopixel = (1<<7);
			}
			secuencia_neopixel = !secuencia_neopixel;
			while ( !(PINC & (1<<PORTC1)) );
		}
		
		if (!(reloj%100)){
			actualizar_leds(secuencia_leds);
		}
		
		if (!(reloj%150)){
			actualizar_neopixel(secuencia_neopixel);
		}
		
		delay_1ms();
    }
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

void actualizar_neopixel(uint8_t secuencia){
	if (!secuencia) {	// SECUENCIA C
		if (paridad_neopixel) {
			for(uint8_t j=0;j<4;j++){
				led_negro();
				led_rojo();
			}
		} else {
			for(uint8_t j=0;j<4;j++){
				led_azul();
				led_negro();
			}
		}
		
		paridad_neopixel = !paridad_neopixel;
		
	} else {			// SECUENCIA D
		
		for (uint8_t i=0; i<8; i++) {
			if ( (posicion_neopixel&(1<<i)) ){
				led_verde();
			} else {
				led_negro();
			}
		}
		if (posicion_neopixel==1) {
			posicion_neopixel=(1<<7);
		} else {
			posicion_neopixel>>=1;
		}
	}
}

void delay_1ms() {
	_delay_ms(1);
	reloj++;
}