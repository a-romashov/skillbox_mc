#include "morse.h"


#define BASE_DELAY 1
#define DOT_DELAY BASE_DELAY
#define DASH_DELAY 3 * BASE_DELAY

#define SAME_SYMBOL_INTERVAL BASE_DELAY
#define SYMBOL_INTERVAL 2 * BASE_DELAY
#define WORD_INTERVAL 4 * BASE_DELAY
#define END_INTERVAL 6 * BASE_DELAY

#define MORSE_SYMBOLS_MAX_BITS 8

//A .-
#define Sym_a 0b101
//B -...
#define Sym_b 0b11000
//C -.-.
#define Sym_c 0b11010
//D	-..
#define Sym_d 0b1100
//E	.
#define Sym_e 0b10
//F	..-.
#define Sym_f 0b10010
//G	--.
#define Sym_g 0b1110
//H	....
#define Sym_h 0b10000
//I	..
#define Sym_i 0b100
//J	.---
#define Sym_j 0b10111
//K	-.-
#define Sym_k 0b1101
//L	.-..
#define Sym_l 0b10100
//M	--
#define Sym_m 0b111
//N	-.
#define Sym_n 0b110
//O	---
#define Sym_o 0b1111
//P	.--.
#define Sym_p 0b10110
//Q	--.-
#define Sym_q 0b11101
//R	.-.
#define Sym_r 0b1010
//S	...
#define Sym_s 0b1000
//T	-
#define Sym_t 0b11
//U	..-
#define Sym_u 0b1001
//V	...-
#define Sym_v 0b10001
//W	.--
#define Sym_w 0b1011
//X	-..-
#define Sym_x 0b11001
//Y	-.--
#define Sym_y 0b11011
//Z --..
#define Sym_z 0b11100

//0 -----
#define Sym_0 0b111111
//1 .----
#define Sym_1 0b101111
//2 ..---
#define Sym_2 0b100111
//3 ...--
#define Sym_3 0b100011
//4 ....-
#define Sym_4 0b100001
//5 .....
#define Sym_5 0b100000
//6 -....
#define Sym_6 0b110000
//7 --...
#define Sym_7 0b111000
//8 ---..
#define Sym_8 0b111100
//9 ----.
#define Sym_9 0b111110

//. .-.-.-
#define Sym_dot 0b1010101
//, --..--
#define Sym_com 0b1110011
//? ..--..
#define Sym_qst 0b1001100
//! -.-.--
#define Sym_exc 0b1101011
/// -..-.
#define Sym_sla 0b110010
//( -.--.
#define Sym_lpt 0b110110
//) -.--.-
#define Sym_rpt 0b1101101
//& .-...
#define Sym_amp 0b101000
//: ---...
#define Sym_col 0b1111000
//; -.-.-.
#define Sym_sem 0b1101010
//= -...-
#define Sym_eql 0b110001
//+ .-.-.
#define Sym_pls 0b101010
//- -....-
#define Sym_min 0b1100001
//_ ..--.-
#define Sym_und 0b1001101
//" .-..-.
#define Sym_dqt 0b1010010
//$ ...-..-
#define Sym_dol 0b10001001
//@ .--.-.
#define Sym_dog 0b1011010


uint8_t getMorseForSymbol(const char sym);
void echoMorseSymbol(const char sym, const MorseManager_i *printer);

/**
 * @param char *message null нультерминированная строка
 * @param uint8_t messageSizeLimit - на всякий случай защита, если не нультерминированную строку передают
 * @param MorseManager_i *manager - ссылка на нашего покорного слугу, который реализует операции вкл, выкл и паузы
 */
void echoMorse(const char *message, uint8_t messageSizeLimit, const MorseManager_i *manager) {
    uint8_t i = 0;

    /**
     * Идем по буквам нашей фразы.
     */
    while (i < messageSizeLimit && message[i] != '\0') {
        if (message[i] == ' ') {
            /** Встретили пробел, значит слово кончилось */
            manager->wait(WORD_INTERVAL);
        } else {
            /** для второго и последующих проверяем с предыдущим на совпадение, у них интервал пауз короче */
            if (i && message[i - 1] == message[i]) {
                manager->wait(SAME_SYMBOL_INTERVAL);
            } else if (i) {
                /** для первого символа интервал нам не нужен, для остальных перед символом ставим паузу */
                manager->wait(SYMBOL_INTERVAL);
            }
            /** вывод символа */
            echoMorseSymbol(message[i], manager);
        }
        i++;
    }
    /** в конце сообщения ставим соотвествующую паузу */
    manager->wait(END_INTERVAL);
}

void echoMorseSymbol(const char sym, const MorseManager_i *manager) {

    uint8_t code = getMorseForSymbol(sym);
    uint8_t stack[MORSE_SYMBOLS_MAX_BITS];
    uint8_t i = MORSE_SYMBOLS_MAX_BITS;

    /**
     * поскольку биты считаются с конца, сначала положим биты в стек
     */
    for (;code >> 1; code = code >> 1) {
        stack[--i] = code % 2;
    }
    
    for (; i < MORSE_SYMBOLS_MAX_BITS; i++) {
        /**
         * включаем, 
         * затем ждем, в зависимости от того точка это или тире определенный интервал
         * выключаем
         */
        manager->enable();
        manager->wait(stack[i] ? DASH_DELAY : DOT_DELAY);
        manager->disable();
        /**
         * Интервал пауз между символами точек и тире, 
         * для последнего не нужно, т.к. там межсимвольный интервал
         */
        if (i < MORSE_SYMBOLS_MAX_BITS - 1) {
            manager->wait(BASE_DELAY);
        }
    }
}

/**
 * Получаем перевод символа в азбуку морзе
 * Попытка сэкономить ресурсы
 * Первый бит является отсечкой(чтобы символы, начинающиеся с точки, также работали),
 * последующие биты 0 - для точки, 1 для тире
 * @param char sym
 * @return uint8_t число, по факту в двоичной системе
 */
uint8_t getMorseForSymbol(const char sym) {
    switch (sym) {
        case 'a':
        case 'A':
            return Sym_a;
        case 'b':
        case 'B':
            return Sym_b;
        case 'c':
        case 'C':
            return Sym_c;
        case 'd':
        case 'D':
            return Sym_d;
        case 'e':
        case 'E':
            return Sym_e;
        case 'f':
        case 'F':
            return Sym_f;
        case 'g':
        case 'G':
            return Sym_g;
        case 'h':
        case 'H':
            return Sym_h;
        case 'i':
        case 'I':
            return Sym_i;
        case 'k':
        case 'K':
            return Sym_k;
        case 'l':
        case 'L':
            return Sym_l;
        case 'm':
        case 'M':
            return Sym_m;
        case 'n':
        case 'N':
            return Sym_n;
        case 'o':
        case 'O':
            return Sym_o;
        case 'p':
        case 'P':
            return Sym_p;
        case 'q':
        case 'Q':
            return Sym_q;
        case 'r':
        case 'R':
            return Sym_r;
        case 's':
        case 'S':
            return Sym_s;
        case 't':
        case 'T':
            return Sym_t;
        case 'u':
        case 'U':
            return Sym_u;
        case 'v':
        case 'V':
            return Sym_v;
        case 'w':
        case 'W':
            return Sym_w;
        case 'x':
        case 'X':
            return Sym_x;
        case 'y':
        case 'Y':
            return Sym_y;
        case 'z':
        case 'Z':
            return Sym_z;

        case '0':
            return Sym_0;
        case '1':
            return Sym_1;
        case '2':
            return Sym_2;
        case '3':
            return Sym_3;
        case '4':
            return Sym_4;
        case '5':
            return Sym_5;
        case '6':
            return Sym_6;
        case '7':
            return Sym_7;
        case '8':
            return Sym_8;
        case '9':
            return Sym_9;

        case '.':
            return Sym_dot;
        case ',':
            return Sym_com;
        case '?':
            return Sym_qst;
        case '!':
            return Sym_exc;
        case '/':
            return Sym_sla;
        case '(':
            return Sym_lpt;
        case ')':
            return Sym_rpt;
        case '&':
            return Sym_amp;
        case ':':
            return Sym_col;
        case ';':
            return Sym_sem;
        case '=':
            return Sym_eql;
        case '+':
            return Sym_pls;
        case '-':
            return Sym_min;
        case '_':
            return Sym_und;
        case '"':
            return Sym_dqt;
        case '$':
            return Sym_dol;
        case '@':
            return Sym_dog;

    }
    return 0;
}