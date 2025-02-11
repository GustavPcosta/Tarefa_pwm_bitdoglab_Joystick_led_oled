

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"//biblioteca do pwm
#include "hardware/i2c.h"//biblioteca do i2c ligada ao led
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PB 22 // GPIO para botão do Joystick
#define Botao_A 5 // GPIO para botão A

#define LED_VERMELHO_PIN 13 // GPIO para LED Vermelho
#define LED_AZUL_PIN 12 // GPIO para LED Azul
#define LED_VERDE_PIN 11 // GPIO para LED Verde

#define BORDAS_SOLIDAS 0
#define BORDAS_VAZADAS 1
ssd1306_t ssd; // Inicializa a estrutura do display

// Função para configurar o PWM
void setup_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);  // Configura o pino como PWM
    uint slice_num = pwm_gpio_to_slice_num(pin);  // Obtém o slice do pino
    pwm_set_clkdiv(slice_num, 64.0f);  // Ajuste da divisão de clock
    pwm_set_wrap(slice_num, 255);  // Define o valor máximo do contador de PWM
    pwm_set_enabled(slice_num, true);  // Habilita o PWM no pino
}

bool cor = true;
bool leds_ativos = true; // Estado dos LEDs

// Função para alternar a borda do display
void alterar_borda(ssd1306_t *ssd, int estilo) {
    if (estilo == BORDAS_SOLIDAS) {
        ssd1306_rect(ssd, 3, 3, 122, 60, cor, !cor); // Borda sólida
    } else {
        ssd1306_rect(ssd, 0, 0, WIDTH, HEIGHT, false, true);  // Borda vazada
    }
}

int main() {
    // Inicialização de I2C e ADC
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    // Configuração de PWM para os LEDs
    setup_pwm(LED_VERMELHO_PIN); // LED Vermelho
    setup_pwm(LED_AZUL_PIN); // LED Azul
    setup_pwm(LED_VERDE_PIN); // LED Verde

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);

    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);

    uint16_t adc_value_x;
    uint16_t adc_value_y;

    int x_pos = WIDTH / 2;
    int y_pos = HEIGHT / 2;

    int borda_estilo = BORDAS_VAZADAS;  // Estilo de borda inicial (vazada)
    bool led_verde_ativo = false;  // LED Verde inicial (desligado)

    bool ultimo_estado_botao = true;
    bool ultimo_estado_botao_A = true;

    while (true) {
        // Lê os valores do joystick (eixos X e Y)
        adc_select_input(0); // Eixo X
        adc_value_x = adc_read();
        adc_select_input(1); // Eixo Y
        adc_value_y = adc_read();

        // Mapeia os valores de ADC para a tela e ajusta o PWM dos LEDs
        x_pos = (adc_value_x - 2048) / 40 + (WIDTH / 2 + 24);
        y_pos = (adc_value_y - 2048) / 40 + (HEIGHT / 2 + 24);

        // Ajusta a intensidade dos LEDs
        uint8_t intensidade_vermelho = (adc_value_x * 255) / 4095;
        uint8_t intensidade_azul = (adc_value_y * 255) / 4095;

        // Controla o LED Verde com o botão
        bool estado_botao = !gpio_get(JOYSTICK_PB);
        if (estado_botao && !ultimo_estado_botao) {
            led_verde_ativo = !led_verde_ativo;
            pwm_set_gpio_level(LED_VERDE_PIN, led_verde_ativo ? 255 : 0);
            borda_estilo = (borda_estilo == BORDAS_SOLIDAS) ? BORDAS_VAZADAS : BORDAS_SOLIDAS;
        }
        ultimo_estado_botao = estado_botao;

        // Alterna estado dos LEDs PWM com o Botão A
        bool estado_botao_A = !gpio_get(Botao_A);
        if (estado_botao_A && !ultimo_estado_botao_A) {
            leds_ativos = !leds_ativos;
        }
        ultimo_estado_botao_A = estado_botao_A;

        // Se os LEDs estiverem ativos, ajusta o PWM normalmente
        pwm_set_gpio_level(LED_VERMELHO_PIN, leds_ativos ? intensidade_vermelho : 0);
        pwm_set_gpio_level(LED_AZUL_PIN, leds_ativos ? intensidade_azul : 0);

        // Garante que o quadrado fique dentro da tela
        if (x_pos < 0) x_pos = 0;
        if (x_pos > WIDTH - 8) x_pos = WIDTH - 8;
        if (y_pos < 0) y_pos = 0;
        if (y_pos > HEIGHT - 8) y_pos = HEIGHT - 8;

        // Limpa a tela e desenha o quadrado
        ssd1306_fill(&ssd, false);

        // Se o botão estiver pressionado, desenha a borda
        if (estado_botao) {
            alterar_borda(&ssd, borda_estilo);
        }

        // Desenha o quadrado na tela
        ssd1306_rect(&ssd, x_pos, y_pos, 8, 8, true, false);
        ssd1306_send_data(&ssd);

        sleep_us(50); // Delay para evitar flickering
    }
}
