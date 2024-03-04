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

const int BUZZER = 22;

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

void som(uint gpio){
    if (gpio == RED_LED_PIN) {
            for(int i = 0; i<600; i++){
                    gpio_put(BUZZER, 1);
                    sleep_us(150);
                    gpio_put(BUZZER, 0);
                    sleep_us(150);
                }
        } if (gpio == BLUE_LED_PIN) {
            for(int i = 0; i<600; i++){
                    gpio_put(BUZZER, 1);
                    sleep_us(250);
                    gpio_put(BUZZER, 0);
                    sleep_us(250);
                }
        } if (gpio == GREEN_LED_PIN) {
            for(int i = 0; i<600; i++){
                    gpio_put(BUZZER, 1);
                    sleep_us(200);
                    gpio_put(BUZZER, 0);
                    sleep_us(200);
                }
        } if (gpio == YELLOW_LED_PIN) {
            for(int i = 0; i<600; i++){
                    gpio_put(BUZZER, 1);
                    sleep_us(400);
                    gpio_put(BUZZER, 0);
                    sleep_us(400);
                }
        }
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    timer_flag = 1;

    // Can return a value here in us to fire in the future
    return 0;
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

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

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

    while (true) {

        if (timer_flag) {
            timer_flag = 0;
            for (int i = 0; i < sequencia_len; i++) {
                gpio_put(sequencia[i], 1);
                som(sequencia[i]);
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
            add_alarm_in_ms(2000, alarm_callback, NULL, false);
        }

        if (red_flag) {
            red_flag = 0;
            if  (to_ms_since_boot(get_absolute_time()) - time_since_red > DEBOUNCE_TIME) {
                time_since_red = to_ms_since_boot(get_absolute_time());
                gpio_put(RED_LED_PIN, 1);
                if (sequencia[acertos] == RED_LED_PIN) {
                    acertos++;
                }
                som(RED_LED_PIN);
                gpio_put(RED_LED_PIN, 0);
            }
        }
        if (blue_flag) {
            blue_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_blue > DEBOUNCE_TIME) {
                time_since_blue = to_ms_since_boot(get_absolute_time());
                gpio_put(BLUE_LED_PIN, 1);
                if (sequencia[acertos] == BLUE_LED_PIN) {
                    acertos++;
                }
                som(BLUE_LED_PIN);
                gpio_put(BLUE_LED_PIN, 0);
            }
        }
        if (green_flag) {
            green_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_green > DEBOUNCE_TIME) {
                time_since_green = to_ms_since_boot(get_absolute_time());
                gpio_put(GREEN_LED_PIN, 1);
                if (sequencia[acertos] == GREEN_LED_PIN) {
                    acertos++;
                }
                som(GREEN_LED_PIN);
                gpio_put(GREEN_LED_PIN, 0);
            }
        }
        if (yellow_flag) {
            yellow_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - time_since_yellow > DEBOUNCE_TIME) {
                time_since_yellow = to_ms_since_boot(get_absolute_time());
                gpio_put(YELLOW_LED_PIN, 1);
                if (sequencia[acertos] == YELLOW_LED_PIN) {
                    acertos++;
                }
                som(YELLOW_LED_PIN);
                gpio_put(YELLOW_LED_PIN, 0);
            }
        }
    }
}
