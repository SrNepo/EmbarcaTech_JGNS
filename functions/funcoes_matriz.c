#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "functions/funcoes_buzzers.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

#define LED_COUNT 25
#define LED_PIN 7

#define BUZZER1_PIN 21
#define BUZZER2_PIN 10

// Definição de pixel GRB
struct pixel_t {
    uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
  
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.
  
// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];
  
// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

    // Cria programa PIO.
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
  
    // Toma posse de uma máquina PIO.
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
    }
  
    // Inicia programa na máquina PIO obtida.
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
  
    // Limpa buffer de pixels.
    for (uint i = 0; i < LED_COUNT; ++i) {
      leds[i].R = 0;
      leds[i].G = 0;
      leds[i].B = 0;
    }
}
  
/**
* Atribui uma cor RGB a um LED.
*/
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}
  
/**
* Limpa o buffer de pixels.
*/
void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}
  
/**
* Escreve os dados do buffer nos LEDs.
*/
void npWrite() {
    // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}
  
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

void setLED(int matriz[5][5][3]) {
    for(int linha = 0; linha < 5; linha++) {
        for(int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
        }
    }
}

void setupMATRIZ() {
    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();

    // Aqui, você desenha nos LEDs.
    npWrite(); // Escreve os dados nos LEDs.
}

void matriz_a() {
    int matriz_button_a[5][5][3] = {
      {{0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 100, 0}, {0, 100, 0}, {0, 100, 0}, {0, 100, 0}, {0, 100, 0}},
      {{0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}}
    };
    
    setLED(matriz_button_a);
  
    npWrite();
    sleep_ms(500);
    npClear();
    npWrite();
    npClear();
  
}
    
void matriz_b() {
      int matriz_button_b[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}},
        {{0, 100, 0}, {0, 100, 0}, {0, 100, 0}, {0, 100, 0}, {0, 100, 0}}, 
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}}
    };
  
    setLED(matriz_button_b);
  
    npWrite();
    sleep_ms(500);
    npClear();
    npWrite();
    npClear();
}
    
void inicializacao_a() {
    int inicializacao_a[5][5][3] = {
      {{0, 0, 100}, {0, 0, 100}, {0, 0, 100}, {0, 0, 100}, {0, 0, 100}},
      {{0, 0, 100}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 100}},
      {{0, 0, 100}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 100}},
      {{0, 0, 100}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 100}},
      {{0, 0, 100}, {0, 0, 100}, {0, 0, 100}, {0, 0, 100}, {0, 0, 100}}       
    };
  
    setLED(inicializacao_a);
  
    npWrite();
    sleep_ms(300);
    npClear();
    npWrite();
    npClear();
}
  
void inicializacao_b() {
    int inicializacao_b[5][5][3] = {
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 20}, {0, 0, 20}, {0, 0, 20}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 100}, {0, 0, 0}, {0, 0, 100}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 100}, {0, 0, 100}, {0, 0, 100}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}       
    };
  
    setLED(inicializacao_b);
  
    npWrite();
    sleep_ms(300);
    npClear();
    npWrite();
    npClear();
}
  
void inicializacao_c() {
    int inicializacao_c[5][5][3] = {
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 20}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}  
    };
  
    setLED(inicializacao_c);
  
    npWrite();
    sleep_ms(300);
    npClear();
    npWrite();
    npClear();
}
  
void matriz_acerto() {
    int matriz_acerto[5][5][3] = {
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 100, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}},
      {{0, 100, 0}, {0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 100, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}        
    };
  
    setLED(matriz_acerto);
  
    npWrite();
    beep(BUZZER2_PIN, 100);
    sleep_ms(200);
    npClear();
    npWrite();
    npClear();
}
  
void matriz_erro() {
    int matriz_erro[5][5][3] = {
      {{100, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {100, 0, 0}},
      {{0, 0, 0}, {100, 0, 0}, {0, 0, 0}, {100, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {0, 0, 0}, {100, 0, 0}, {0, 0, 0}, {0, 0, 0}},
      {{0, 0, 0}, {100, 0, 0}, {0, 0, 0}, {100, 0, 0}, {0, 0, 0}},
      {{100, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {100, 0, 0}}    
    };
  
    setLED(matriz_erro);
  
    npWrite();
    beep(BUZZER1_PIN, 150);
    sleep_ms(50);
    npClear();
    npWrite();
    npClear();
}

void matriz_iniciando() {
    inicializacao_a();
    sleep_ms(50);
    inicializacao_b();
    sleep_ms(50);
    inicializacao_c();
    sleep_ms(50);
    inicializacao_a();
    sleep_ms(50);
    inicializacao_b();
    sleep_ms(50);
    inicializacao_c();
    sleep_ms(50);
}

void matriz_reiniciando() {
    inicializacao_c();
    sleep_ms(50);
    inicializacao_b();
    sleep_ms(50);
    inicializacao_a();
    sleep_ms(50);
    inicializacao_c();
    sleep_ms(50);
    inicializacao_b();
    sleep_ms(50);
    inicializacao_a();
    sleep_ms(50);
}