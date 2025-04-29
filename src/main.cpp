#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRD |= 0xF0;            // PD4–PD7 como salida (decenas)
    DDRB |= 0x0F;            // PB0–PB3 como salida (unidades)

    DDRD &= ~(0x04);         // botón subir
    PORTD |= 0x04;

    DDRD &= ~(0x08);         // botón bajar
    PORTD |= 0x08;

    DDRB &= ~(0x10);         // botón suma
    PORTB |= 0x10;

    DDRB &= ~(0x20);         // botón sumar
    PORTB |= 0x20;

    DDRD &= ~(0x02);         // botón ENTER
    PORTD |= 0x02;

    uint8_t unidades = 0;
    uint8_t decenas = 0;
    uint8_t guardadou = 0;
    uint8_t guardadod = 0;
    uint8_t mate = 0;        // 1 = resta, 2 = suma

    while (1) {
        // Subir
        if (!(PIND & 0x04)) {
            _delay_ms(50);
            while (!(PIND & 0x04));
            _delay_ms(50);

            if (!(decenas == 9 && unidades == 9)) {
                if (unidades >= 9) {
                    unidades = 0;
                    decenas++;
                } else {
                    unidades++;
                }
            }
        }

        // Bajar
        if (!(PIND & 0x08)) {
            _delay_ms(50);
            while (!(PIND & 0x08));
            _delay_ms(50);

            if (!(unidades == 0 && decenas == 0)) {
                if (unidades == 0) {
                    unidades = 9;
                    if (decenas > 0) {
                        decenas--;
                    }
                } else {
                    unidades--;
                }
            }
        }

        // RESTA
        if (!(PINB & 0x20)) {
            _delay_ms(50);
            while (!(PINB & 0x20));
            _delay_ms(50);

            guardadou = unidades;
            guardadod = decenas;

            unidades = 0;
            decenas = 0;

            mate = 1;
        }

        // SUMA
        if (!(PINB & 0x10)) {
            _delay_ms(50);
            while (!(PINB & 0x10));
            _delay_ms(50);

            guardadou = unidades;
            guardadod = decenas;

            unidades = 0;
            decenas = 0;

            mate = 2;
        }

        // ENTER
        if (!(PIND & 0x02)) {
            _delay_ms(50);
            while (!(PIND & 0x02));
            _delay_ms(50);

            uint8_t finalu = unidades;
            uint8_t finald = decenas;

            int resultadou = 0;
            int resultadod = 0;

            if (mate == 1) { // Resta
                resultadou = guardadou - finalu;
                resultadod = guardadod - finald;
            } else if (mate == 2) { // Suma
                resultadou = guardadou + finalu;
                resultadod = guardadod + finald;
            }

            // Ajustes si hay desbordes o negativos
            if (resultadou < 0) {
                if (resultadod > 0) {
                    resultadod--;
                    resultadou += 10;
                } else {
                    resultadod = 0;
                    resultadou = 0;
                }
            }

            if (resultadou > 9) {
                resultadou -= 10;
                resultadod++;
            }

            if (resultadod > 9) resultadod = 9;

            unidades = resultadou;
            decenas = resultadod;
            mate = 0; // reiniciar operación
        }

        // Mostrar valores en display
        PORTD = (PORTD & 0x0F) | (decenas << 4);
        PORTB = (PORTB & 0xF0) | (unidades & 0x0F);
    }
}
