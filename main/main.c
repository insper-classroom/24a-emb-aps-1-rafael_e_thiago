/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int BLUE_BTN_PIN = 18;
const int RED_BTN_PIN = 19;
const int GREEN_BTN_PIN = 20;
const int YELLOW_BTN_PIN = 21;

const int BLUE_LED_PIN = 13;
const int RED_LED_PIN = 12;
const int GREEN_LED_PIN = 11;
const int YELLOW_LED_PIN = 10;

const int BUZZ_PIN1 = 0;
const int BUZZ_PIN2 = 1;

volatile int red_flag = 0;
volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int yellow_flag = 0;

volatile int timer_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == RED_BTN_PIN) {
            red_flag = 1;
        } if (gpio == BLUE_BTN_PIN) {
            blue_flag = 1;
        } if (gpio == GREEN_BTN_PIN) {
            green_flag = 1;
        } if (gpio == YELLOW_BTN_PIN) {
            yellow_flag = 1;
        }
    }
}

bool timer_callback(repeating_timer_t *rt) {
    timer_flag = 1;
    return true;
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

    int DEBOUNCE_TIME = 200;

    // srand(to_us_since_boot(get_absolute_time()));
    int sequencia[100];
    int numeros_predeterminados[6] = {13, 11, 13, 10, 12, 11};
    int sequencia_len = 0;
    int acertos = 0;

    u_int32_t time_since_red = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_blue = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_green = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_yellow = to_ms_since_boot(get_absolute_time());

    repeating_timer_t timer;

    while (true) {

        if (timer_flag) {
            timer_flag = 0;
            for (int i = 0; i < sequencia_len; i++) {
                gpio_put(sequencia[i], 1);
                sleep_ms(200);
                gpio_put(sequencia[i], 0);
                sleep_ms(200);
            }
            sleep_ms(500);
            }

        if (sequencia_len == acertos) {
            // sequencia[sequencia_len] = ((double) rand()/__RAND_MAX) * (21 - 18) + 18;
            sequencia[sequencia_len] = numeros_predeterminados[sequencia_len];
            sequencia_len++;
            acertos = 0;
            add_repeating_timer_ms(2000, timer_callback, NULL, &timer);
        }

        if (red_flag) {
            red_flag = 0;
            if  (to_ms_since_boot(get_absolute_time()) - time_since_red > DEBOUNCE_TIME) {
                time_since_red = to_ms_since_boot(get_absolute_time());
                gpio_put(RED_LED_PIN, 1);
                sleep_ms(100);
                gpio_put(RED_LED_PIN, 0);
                cancel_repeating_timer(&timer);
                if (sequencia[acertos] == RED_LED_PIN) {
                    acertos++;
                }
            }
        }
        if (blue_flag) {
            blue_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_blue > DEBOUNCE_TIME) {
                time_since_blue = to_ms_since_boot(get_absolute_time());
                gpio_put(BLUE_LED_PIN, 1);
                sleep_ms(100);
                gpio_put(BLUE_LED_PIN, 0);
                cancel_repeating_timer(&timer);
                if (sequencia[acertos] == BLUE_LED_PIN) {
                    acertos++;
                }
            }
        }
        if (green_flag) {
            green_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_green > DEBOUNCE_TIME) {
                time_since_green = to_ms_since_boot(get_absolute_time());
                gpio_put(GREEN_LED_PIN, 1);
                sleep_ms(100);
                gpio_put(GREEN_LED_PIN, 0);
                cancel_repeating_timer(&timer);
                if (sequencia[acertos] == GREEN_LED_PIN) {
                    acertos++;
                }
            }
        }
        if (yellow_flag) {
            yellow_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_yellow > DEBOUNCE_TIME) {
                time_since_yellow = to_ms_since_boot(get_absolute_time());
                gpio_put(YELLOW_LED_PIN, 1);
                sleep_ms(100);
                gpio_put(YELLOW_LED_PIN, 0);
                cancel_repeating_timer(&timer);
                if (sequencia[acertos] == YELLOW_LED_PIN) {
                    acertos++;
                }
            }
        }
    }
}
