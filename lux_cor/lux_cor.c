#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bh1750_light_sensor.h"
#include "ssd1306.h"
#include "font.h"
#include "matriz.h"
#include <math.h>

// Para o sensor de luz BH1750. Endereço 0x23
#define I2C_PORT i2c0               // i2c0 pinos 0 e 1, i2c1 pinos 2 e 3
#define I2C_SDA 0                   // 0 ou 2
#define I2C_SCL 1                   // 1 ou 3
// Definições do sensor GY-33
#define GY33_I2C_ADDR 0x29 // Endereço do GY-33 no barramento I2C
#define I2C_PORT i2c0
#define SDA_PIN 0
#define SCL_PIN 1
// Display na I2C
#define I2C_PORT_DISP i2c1
#define I2C_SDA_DISP 14
#define I2C_SCL_DISP 15
#define endereco 0x3C
// --- Definições dos Pinos ---
const uint BTN_A_PIN = 5;  // Pino para o Botão A
const uint RED_PIN = 13;   // Pino para o LED vermelho
const uint GREEN_PIN = 11; // Pino para o LED verde
const uint BLUE_PIN = 12;  // Pino para o LED azul
// --- Variáveis de Controle ---
volatile int led_state = 0;            // 0=Vermelho, 1=Verde, 2=Azul
volatile uint32_t last_press_time = 0; // Armazena o tempo do último aperto para o debounce
extern float matriz[5][5];
ssd1306_t ssd;
float matriz[5][5] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0}
};

// função para atualizar a matriz conforme lux
void atualiza_matriz(float lux) {
    float x = lux / 500.0f;
    
    if (x > 1.0f) x = 1.0f;   // saturação
    if (x < 0.0f) x = 0.0f;

    matriz[2][2] = x;   // centro
}


// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Verifica se o pino que chamou a interrupção foi o do BOOTSEL
    if (gpio == botaoB)
    {
        reset_usb_boot(0, 0);
    }
    else if (gpio == BTN_A_PIN)
    {

        // Pega o tempo atual em milissegundos
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        // Lógica de Debounce: Ignora o aperto se ocorreu muito rápido (menos de 250ms)
        if (current_time - last_press_time > 250)
        {
            // Atualiza o tempo do último aperto válido
            last_press_time = current_time;

            // Avança para o próximo estado do LED
            led_state++;

            // Se passar do último estado (azul), volta para o primeiro (vermelho)
            if (led_state > 3)
            {
                led_state = 0;
            }
        }
    }
}
// Registros do sensor
#define ENABLE_REG 0x80
#define ATIME_REG 0x81
#define CONTROL_REG 0x8F
#define ID_REG 0x92
#define STATUS_REG 0x93
#define CDATA_REG 0x94 //  "Clear"
#define RDATA_REG 0x96 //  "Red"
#define GDATA_REG 0x98 //  "Green"
#define BDATA_REG 0x9A //  "Blue"

// Função para escrever um valor em um registro do GY-33
void gy33_write_register(uint8_t reg, uint8_t value)
{
    uint8_t buffer[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, GY33_I2C_ADDR, buffer, 2, false);
}

// Função para ler um valor de um registro do GY-33
uint16_t gy33_read_register(uint8_t reg)
{
    uint8_t buffer[2];
    i2c_write_blocking(I2C_PORT, GY33_I2C_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, GY33_I2C_ADDR, buffer, 2, false);
    return (buffer[1] << 8) | buffer[0]; // Combina os bytes em um valor de 16 bits
}

// Inicializa o GY-33
void gy33_init()
{
    gy33_write_register(ENABLE_REG, 0x03);  // Ativa o sensor (Power ON e Ativação do ADC)
    gy33_write_register(ATIME_REG, 0xF5);   // Tempo de integração (ajusta a sensibilidade) D5 => 103ms
    gy33_write_register(CONTROL_REG, 0x00); // Configuração de ganho padrão (1x) (pode ir até 60x)
}

// Lê os valores RGB e Clear do GY-33
void gy33_read_color(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c)
{
    *c = gy33_read_register(CDATA_REG);
    *r = gy33_read_register(RDATA_REG);
    *g = gy33_read_register(GDATA_REG);
    *b = gy33_read_register(BDATA_REG);
}
void inicia_pino(){
        // Para ser utilizado o modo BOOTSEL com botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
   // Fim do trecho para modo BOOTSEL com botão B
    // Configuração dos pinos GPIO do LED RGB
    gpio_init(RED_PIN);
    gpio_init(GREEN_PIN);
    gpio_init(BLUE_PIN);

    gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // --- Inicialização do Pino do Botão ---
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN); // Usa o resistor de pull-up interno
    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    stdio_init_all();
    sleep_ms(2000);
    // I2C do Display pode ser diferente dos sensores. Funcionando em 400Khz.
    i2c_init(I2C_PORT_DISP, 400 * 1000);

    gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);                    
    gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);                    
    gpio_pull_up(I2C_SDA_DISP);                                        
    gpio_pull_up(I2C_SCL_DISP);                                        
                                                      
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT_DISP); 
    ssd1306_config(&ssd);                                              
    ssd1306_send_data(&ssd);                                           

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa o I2C0
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    // Inicializa o sensor de luz BH1750
    bh1750_power_on(I2C_PORT);
    printf("Iniciando GY-33...\n");
    gy33_init();

}
int main()
{
    inicia_pino(); // Inicializa os pinos e o display
    char str_red[5];   // Buffer para armazenar a string
    char str_green[5]; 
    char str_blue[5];  
    char str_clear[5]; 

    bool cor = true;
    char str_lux[10];  // Buffer para armazenar a string
    while (1)
    {    uint16_t r, g, b, c;
        gy33_read_color(&r, &g, &b, &c);
        printf("Cor detectada - R: %d, G: %d, B: %d, Clear: %d\n", r, g, b, c);

        sprintf(str_red, "%d R", r);   // Converte o inteiro em string
        sprintf(str_green, "%d G", g); 
        sprintf(str_blue, "%d B", b);  
        sprintf(str_clear, "%d C", c); 
        // Leitura do sensor de Luz BH1750
        uint16_t lux = bh1750_read_measurement(I2C_PORT);
        printf("Lux = %d\n", lux);

 
        sprintf(str_lux, "%d Lux", lux);  // Converte o inteiro em string
    // Escolhe a cor dominante
        uint16_t max_val = r;
        char cor_dominante = 'R';

        if (g > max_val) {
            max_val = g;
            cor_dominante = 'G';
        }
        if (b > max_val) {
            max_val = b;
            cor_dominante = 'B';
        }

        uint8_t rr=0, gg=0, bb=0;
        if (cor_dominante == 'R') rr = 255;
        if (cor_dominante == 'G') gg = 255;
        if (cor_dominante == 'B') bb = 255;
        if( r < 15 && b < 15 && g < 15) {
            rr = 0; gg = 0; bb = 0; // Se todos os valores forem baixos, desliga os LEDs
        }
        mostra_lux(lux, rr, gg, bb);


        
        // cor = !cor;
        //  Atualiza o conteúdo do display com animações
        ssd1306_fill(&ssd, !cor);                          // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
        ssd1306_line(&ssd, 3, 25, 123, 25, cor);           // Desenha uma linha
        ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
        ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
        ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
        ssd1306_draw_string(&ssd, "Cores   RGB", 10, 28);  // Desenha uma string
        ssd1306_line(&ssd, 63, 25, 63, 60, cor);           // Desenha uma linha vertical
        ssd1306_draw_string(&ssd, str_red, 14, 41);        // Desenha uma string
        ssd1306_draw_string(&ssd, str_green, 14, 52);      // Desenha uma string
        ssd1306_draw_string(&ssd, str_blue, 73, 41);       // Desenha uma string
        ssd1306_draw_string(&ssd, str_clear, 73, 52);      // Desenha uma string
        ssd1306_send_data(&ssd);                           // Atualiza o display

        if (led_state == 0)
        { // Estado 0: Ligar Vermelho
            gpio_put(RED_PIN, 1);
            gpio_put(GREEN_PIN, 0);
            gpio_put(BLUE_PIN, 0);
        }
        else if (led_state == 1)
        { // Estado 1: Ligar Amarelo
            gpio_put(RED_PIN, 1);
            gpio_put(GREEN_PIN, 1);
            gpio_put(BLUE_PIN, 0);
        }        
        else if (led_state == 2)
        { // Estado 1: Ligar Verde
            gpio_put(RED_PIN, 0);
            gpio_put(GREEN_PIN, 1);
            gpio_put(BLUE_PIN, 0);
        }
        else
        { // Estado 2: Ligar Azul
            gpio_put(RED_PIN, 0);
            gpio_put(GREEN_PIN, 0);
            gpio_put(BLUE_PIN, 1);
        }
        sleep_ms(500);
    }

    return 0;
}