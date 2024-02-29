/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int RED_BTN_PIN = 19;
const int BLUE_BTN_PIN = 18;
const int GREEN_BTN_PIN = 20;
const int YELLOW_BTN_PIN = 21;

const int RED_LED_PIN = 11;
const int BLUE_LED_PIN = 10;
const int GREEN_LED_PIN = 12;
const int YELLOW_LED_PIN = 13;

const int BUZZ_PIN1 = 0;
const int BUZZ_PIN2 = 1;

volatile int red_flag = 0;
volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int yellow_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == RED_BTN_PIN) {
            red_flag = 1;
        } else if (gpio == BLUE_LED_PIN) {
            blue_flag = 1;
        } else if (gpio == GREEN_LED_PIN) {
            green_flag = 1;
        } else if (gpio == YELLOW_LED_PIN) {
            yellow_flag = 1;
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(RED_BTN_PIN);
    gpio_set_dir(RED_BTN_PIN, GPIO_IN);
    gpio_pull_up(RED_BTN_PIN);

    gpio_init(BLUE_BTN_PIN);
    gpio_set_dir(BLUE_BTN_PIN, GPIO_IN);
    gpio_pull_up(BLUE_BTN_PIN);

    gpio_init(GREEN_BTN_PIN);
    gpio_set_dir(GREEN_BTN_PIN, GPIO_IN);
    gpio_pull_up(GREEN_BTN_PIN);

    gpio_init(YELLOW_BTN_PIN);
    gpio_set_dir(YELLOW_BTN_PIN, GPIO_IN);
    gpio_pull_up(YELLOW_BTN_PIN);

    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);

    gpio_init(BLUE_LED_PIN);
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);

    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

    gpio_init(YELLOW_LED_PIN);
    gpio_set_dir(YELLOW_LED_PIN, GPIO_OUT);

    gpio_init(BUZZ_PIN1);
    gpio_set_dir(BUZZ_PIN1, GPIO_OUT);

    gpio_init(BUZZ_PIN2);
    gpio_set_dir(BUZZ_PIN2, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(RED_BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BLUE_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(GREEN_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(YELLOW_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (red_flag) {
            red_flag = 0;
            gpio_put(RED_LED_PIN, !gpio_get(RED_LED_PIN));
        }
        if (blue_flag) {
            blue_flag = 0;
            gpio_put(BLUE_LED_PIN, !gpio_get(BLUE_LED_PIN));
        }
        if (green_flag) {
            green_flag = 0;
            gpio_put(GREEN_LED_PIN, !gpio_get(GREEN_LED_PIN));
        }
        if (yellow_flag) {
            yellow_flag = 0;
            gpio_put(YELLOW_LED_PIN, !gpio_get(YELLOW_LED_PIN));
        }
    }
}
