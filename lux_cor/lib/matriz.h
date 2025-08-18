#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PI0ELS 25
#define WS2812_PIN 7

static inline void put_pi0el(uint32_t pi0el_grb)
{
    pio_sm_put_blocking(pio0, 0, pi0el_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

int i = 0;

double desenho0[25] = {
    0.0, 0.0, 0, 0.0, 0.0,
    0.0, 0, 0, 0, 0.0,
    0, 0, 0, 0, 0,
    0.0, 0, 0, 0, 0.0,
    0.0, 0.0, 0, 0.0, 0.0
};

void mostra_lux(uint16_t lux, uint8_t r, uint8_t g, uint8_t b) {
    // Normaliza lux em 0..255
    if (lux > 500) lux = 500;
    uint8_t intensidade = (lux * 255) / 500;

    // Ajusta cor proporcional
    uint32_t color = urgb_u32((r * intensidade) / 255,
                              (g * intensidade) / 255,
                              (b * intensidade) / 255);

    // Preenche todos os 25 LEDs com essa cor
    for (int i = 0; i < NUM_PI0ELS; i++) {
        put_pi0el(color);
    }
}
