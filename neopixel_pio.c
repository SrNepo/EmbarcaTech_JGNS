#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include <time.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_i2c.h"
#include "hardware/i2c.h"
#include "functions/funcoes_matriz.h"
#include "functions/funcoes_buzzers.h"
#include "functions/funcoes_display.h"

#define button_a 5
#define button_b 6

#define BUZZER1_PIN 21
#define BUZZER2_PIN 10
#define BUZZER1_FREQUENCY 2200
#define BUZZER2_FREQUENCY 8000

int lado(int lista[], int quant) {
	for (int i = 0; i < quant; i++) {
		lista[i] = 2;
	}
	srand(time(NULL));
	for (int i = 0; i < quant; i++) {
		int esq_dir = (rand() % 2);
		switch (esq_dir) {
		    case 0:
			    lista[i] = 0;
			    break;
		    case 1:
			    lista[i] = 1;
			    break;
		}
	}
}

int esperar_botao() {
  while (true) {
      if (gpio_get(button_a) == 0) {
          return 0;
      }
      if (gpio_get(button_b) == 0) {
          return 1;
      }
  }
}

int botao_com_mensagem() {
  while (true) {
    display_selecionar_nivel();
    sleep_ms(400);

    if (gpio_get(button_a) == 0) {
      return 7;
    }
    if (gpio_get(button_b) == 0) {
      return 20;
    }

    display_nivel_facil();
    sleep_ms(400);

    if (gpio_get(button_a) == 0) {
      return 7;
    }
    if (gpio_get(button_b) == 0) {
      return 20;
    }

    display_nivel_dificil();
    sleep_ms(400);

    if (gpio_get(button_a) == 0) {
      return 7;
    }
    if (gpio_get(button_b) == 0) {
      return 20;
    }
  }
}


int main() {

  // Inicializa entradas e saídas.
  gpio_init(button_a);
  gpio_set_dir(button_a, GPIO_IN);
  gpio_pull_up(button_a); // Ativa o pull-up interno

  gpio_init(button_b);
  gpio_set_dir(button_b, GPIO_IN);
  gpio_pull_up(button_b); // Ativa o pull-up interno

  setup_buzzers(BUZZER1_FREQUENCY, BUZZER2_FREQUENCY);

  setupMATRIZ();

  setup_oled();

  int tamanho = 1;

  while (true) {

    tamanho = botao_com_mensagem();
    int direcao_lista[tamanho];

    lado(direcao_lista, tamanho);

    sleep_ms(1000);
    display_iniciando();
    matriz_iniciando();
    display_jogando();

    for (int i = 0; i < tamanho; i++){
      for (int j = 0; j <= i; j++) {
        if (direcao_lista[j] == 0) {
          matriz_a();
          sleep_ms(200);
        }
        if (direcao_lista[j] == 1) {
          matriz_b();
          sleep_ms(200);
        } 
      }
      bool erro = false;
      for (int p = 0; p <= i; p++) {
        int escolha = esperar_botao();
        if (gpio_get(button_a) == 0 || gpio_get(button_b) == 0){
          if (escolha == direcao_lista[p]) {
            matriz_acerto();
            sleep_ms(300);
          } else {
            matriz_erro();
            sleep_ms(50);
            matriz_erro();
            sleep_ms(1000);
            erro = true;
            break;
          }
        }
      }
      if (erro) break;
  }
    display_reiniciando();
    matriz_reiniciando();
    desligar_display_oled();
    sleep_ms(1000);
  }
}