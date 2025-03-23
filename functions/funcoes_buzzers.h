#ifndef funcoes_buzzers_h
#define funcoes_buzzers_h

void pwm_init_buzzer(uint pin, int BUZZER_FREQUENCY);
void beep(uint pin, uint duration_ms);
void setup_buzzers(int frequency1, int frequency2);

#endif