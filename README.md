# Controle de LEDs RGB com Joystick e Display SSD1306

## Descrição
Este projeto implementa o controle de LEDs RGB utilizando um joystick analógico, um botão adicional e um display SSD1306 para visualização da posição do joystick. A intensidade dos LEDs é ajustada por PWM de acordo com os eixos X e Y do joystick. O sistema também permite alternar a borda do display e ativar/desativar os LEDs utilizando botões físicos.

## Componentes Utilizados
- Raspberry Pi Pico
- Joystick analógico
- Display SSD1306 OLED (I2C)
- LEDs RGB (Vermelho, Azul, Verde)
- Botões físicos

## Funcionalidades
- Controle da intensidade dos LEDs Vermelho e Azul com base nos eixos X e Y do joystick
- Liga/desliga o LED Verde ao pressionar o botão do joystick
- Alterna o estilo da borda do display ao pressionar o botão do joystick
- Ativa/desativa os LEDs PWM com o botão A
- Exibe a posição do joystick no display SSD1306

## Configuração dos Pinos
- **Joystick:**
  - Eixo X: GPIO 26 (ADC0)
  - Eixo Y: GPIO 27 (ADC1)
  - Botão: GPIO 22
- **Botão A:** GPIO 5
- **LEDs:**
  - Vermelho: GPIO 13
  - Azul: GPIO 12
  - Verde: GPIO 11
- **Display SSD1306 (I2C):**
  - SDA: GPIO 14
  - SCL: GPIO 15

## Bibliotecas Necessárias
- `pico/stdlib.h`
- `hardware/adc.h`
- `hardware/pwm.h`
- `hardware/i2c.h`
- `inc/ssd1306.h`
- `inc/font.h`

## Como Executar
1. Compilar o código para o Raspberry Pi Pico
2. Fazer o upload do binário para o Pico
3. Conectar os componentes de acordo com os pinos especificados
4. Alimentar o Raspberry Pi Pico e interagir com o joystick e os botões

## Vídeo 

https://www.canva.com/design/DAGe1igJED8/873geTX9uNIEafeRxRMWXA/watch?utm_content=DAGe1igJED8&utm_campaign=designshare&utm_medium=link2&utm_source=uniquelinks&utlId=h06f0687c82

## Autor
Desenvolvido por Gustavo

---



