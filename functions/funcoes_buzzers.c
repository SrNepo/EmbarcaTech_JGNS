
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER1_PIN 21
#define BUZZER2_PIN 10

// Inicializa o PWM para um buzzer
void pwm_init_buzzer(uint pin, int frequency) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Emite um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048); // Ativa som
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0); // Desliga som
    sleep_ms(50);
}

void setup_buzzers(int frequency1, int frequency2) {
    pwm_init_buzzer(BUZZER1_PIN, frequency1);
    pwm_init_buzzer(BUZZER2_PIN, frequency2);
}
