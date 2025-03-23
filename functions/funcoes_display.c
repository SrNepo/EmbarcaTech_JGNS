#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306_i2c.h"
#include "hardware/i2c.h"


const uint I2C_SDA = 14;
const uint I2C_SCL = 15;


void setup_oled() {
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();
}

void ligar_display_oled(char *text[], int num_linhas) {
    ssd1306_send_command(0xAF);
    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)

    int y = 0;
    for (uint i = 0; i < num_linhas; i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void desligar_display_oled() {
    ssd1306_send_command(0xAE); 
}

void display_iniciando() {
    char *iniciando[] = {
        "               ",
        "   Iniciando   ",
        "               "
    };
    ligar_display_oled(iniciando, 3);
}

void display_reiniciando() {
    char *reiniciando[] = {
        "               ",
        "  Reiniciando  ",
        "               "
    };
    ligar_display_oled(reiniciando, 3);
}

void display_selecionar_nivel() {
    char *mensagem[] = {
        "   Selecione   ",
        "               ",
        "   seu nivel   ",
        "               "
    };
    ligar_display_oled(mensagem, 4);
}

void display_nivel_facil() {
    char *mensagem[] = {
        "   < Botao A   ",
        "               ",
        "     Facil     "
    };
    ligar_display_oled(mensagem, 3);
}

void display_nivel_dificil() {
    char *mensagem[] = {
        "   Botao B >   ",
        "               ",
        "   Dificil     "
    };
    ligar_display_oled(mensagem, 3);
}

void display_jogando() {
    char *mensagem[] = {
        "               ",
        "   jogando     "
    };
    ligar_display_oled(mensagem, 2);
}