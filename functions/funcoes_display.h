#ifndef funcoes_display_h
#define funcoes_display_h

void setup_oled();
void ligar_display_oled(char *text[], int num_linhas);
void desligar_display_oled();
void display_iniciando();
void display_reiniciando();
void testar_buffer();
void display_selecionar_nivel();
void display_nivel_facil();
void display_nivel_dificil();
void display_jogando();

#endif