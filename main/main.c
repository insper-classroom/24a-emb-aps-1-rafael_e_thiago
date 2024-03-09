/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"

const int FITA_LED_1 = 5;
const int FITA_LED_2 = 6;
const int FITA_LED_3 = 7;
const int FITA_LED_4 = 8;
const int FITA_LED_5 = 9;
const int FITA_LED_6 = 14;

const int BLUE_BTN_PIN = 18;
const int RED_BTN_PIN = 19;
const int GREEN_BTN_PIN = 20;
const int YELLOW_BTN_PIN = 21;

const int BLUE_LED_PIN = 13;
const int RED_LED_PIN = 12;
const int GREEN_LED_PIN = 11;
const int YELLOW_LED_PIN = 10;

const int SWITCH_LIGAR = 16;

const int BUZZER = 15;

volatile int red_flag = 0;
volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int yellow_flag = 0;

volatile int timer_flag = 0;
volatile bool alarm_timeout_fired = false;

volatile int ligado = 0;

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
        } if (gpio == SWITCH_LIGAR) {
            ligado = 0;
        }
    } else if (events == 0x8) {
        if (gpio == SWITCH_LIGAR) {
            ligado = 1;
        }
    }
}

void som(uint gpio){
    if (gpio == RED_LED_PIN) {
        int freq = 1000000.0/300.0;
        for(int i = 0; i<200/(1000.0/freq); i++){
            gpio_put(BUZZER, 1);
            sleep_us(150);
            gpio_put(BUZZER, 0);
            sleep_us(150);
        }
    } if (gpio == BLUE_LED_PIN) {
        int freq = 1000000.0/500;
        for(int i = 0; i<200/(1000.0/freq); i++){
            gpio_put(BUZZER, 1);
            sleep_us(250);
            gpio_put(BUZZER, 0);
            sleep_us(250);
        }
    } if (gpio == GREEN_LED_PIN) {
        int freq = 1000000.0/400;
        for(int i = 0; i<200.0/(1000.0/freq); i++){
            gpio_put(BUZZER, 1);
            sleep_us(200);
            gpio_put(BUZZER, 0);
            sleep_us(200);
        }
    } if (gpio == YELLOW_LED_PIN) {
        int freq = 1000000.0/800;
        for(int i = 0; i<200/(1000.0/freq); i++){
            gpio_put(BUZZER, 1);
            sleep_us(400);
            gpio_put(BUZZER, 0);
            sleep_us(400);
        }
    }
}

int64_t alarm_timeout_callback(alarm_id_t id, void *user_data) {
    alarm_timeout_fired = true;
    return 0;
}

// Função para reproduzir uma nota
void play_note(int note_frequency, int duration_ms) {
    if (note_frequency > 0) {
        // Calcula o período em microssegundos
        uint32_t period = 1000000 / note_frequency;
        // Calcula o tempo que a metade do período representa (duty cycle de 50%)
        uint32_t half_period = period / 2;

        while (duration_ms > 0) {
            // Ligar o buzzer
            gpio_put(BUZZER, 1);
            busy_wait_us_32(half_period);
            // Desligar o buzzer
            gpio_put(BUZZER, 0);
            busy_wait_us_32(half_period);
            // Atualizar a duração restante
            duration_ms -= (half_period * 2) / 1000;
        }
    }
}

// Função para tocar uma música
void play_music(int *notes, int *durations, int length) {
    for (int i = 0; i < length; i++) {
        play_note(notes[i], durations[i]);
        sleep_ms(50); // Pausa entre as notas
    }
}

void play_defeat_music() {
    int notes[] = {392, 330, 262, 196, 131, 98, 78, 0}; // Notas decrescentes
    int durations[] = {300, 300, 300, 300, 300, 300, 300, 300}; // Durações das notas

    int length = sizeof(notes) / sizeof(notes[0]);

    gpio_put(RED_LED_PIN, 0);
    gpio_put(BLUE_LED_PIN, 0);
    gpio_put(GREEN_LED_PIN, 0);
    gpio_put(YELLOW_LED_PIN, 0);
    gpio_put(FITA_LED_1, 0);
    gpio_put(FITA_LED_2, 0);
    gpio_put(FITA_LED_3, 0);
    gpio_put(FITA_LED_4, 0);
    gpio_put(FITA_LED_5, 0);
    gpio_put(FITA_LED_6, 0);
    play_music(notes, durations, length);
}

void play_start_music() {
    int notes[] = {262, 330, 392, 523, 392, 330};

    int durations[] = {300, 300, 300, 300, 300, 300};
    
    int length = sizeof(notes) / sizeof(notes[0]);

    play_music(notes, durations, length);
}

void play_coin_sound() {
    // Frequências das notas para reproduzir o som de uma moeda do Mario
    int notes[] = {659, 784, 523};
    // Durações das notas
    int durations[] = {150, 150, 150};

    int length = sizeof(notes) / sizeof(notes[0]);

    // Tocar a música
    play_music(notes, durations, length);
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    timer_flag = 1;

    // Can return a value here in us to fire in the future
    return 0;
}

void right_led(int acertos){
    if (acertos == 1){
        gpio_put(FITA_LED_1, 1);
    } else if (acertos == 2){
        gpio_put(FITA_LED_4, 1);
    } else if (acertos == 3){
        gpio_put(FITA_LED_5, 1);
    } else if (acertos == 4){
        gpio_put(FITA_LED_3, 1);
    } else if (acertos == 5){
        gpio_put(FITA_LED_2, 1);
    } else if (acertos == 6){
        gpio_put(FITA_LED_6, 1);
        sleep_ms(50);
        play_coin_sound();
        gpio_put(FITA_LED_1, 0);
        gpio_put(FITA_LED_2, 0);
        gpio_put(FITA_LED_3, 0);
        gpio_put(FITA_LED_4, 0);
        gpio_put(FITA_LED_5, 0);
        gpio_put(FITA_LED_6, 0);
    }
}

int main() {
    stdio_init_all();

    bool start = false;
    bool game = true;
    bool game_over = false;

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

    gpio_init(FITA_LED_1);
    gpio_set_dir(FITA_LED_1, GPIO_OUT);

    gpio_init(FITA_LED_2);
    gpio_set_dir(FITA_LED_2, GPIO_OUT);

    gpio_init(FITA_LED_3);
    gpio_set_dir(FITA_LED_3, GPIO_OUT);
    
    gpio_init(FITA_LED_4);
    gpio_set_dir(FITA_LED_4, GPIO_OUT);

    gpio_init(FITA_LED_5);
    gpio_set_dir(FITA_LED_5, GPIO_OUT);

    gpio_init(FITA_LED_6);
    gpio_set_dir(FITA_LED_6, GPIO_OUT);

    gpio_init(SWITCH_LIGAR);
    gpio_set_dir(SWITCH_LIGAR, GPIO_IN);
    gpio_pull_up(SWITCH_LIGAR);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(RED_BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BLUE_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(GREEN_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(YELLOW_BTN_PIN, GPIO_IRQ_EDGE_FALL, true);

    gpio_set_irq_enabled(SWITCH_LIGAR, GPIO_IRQ_EDGE_FALL || GPIO_IRQ_EDGE_RISE, true);

    play_start_music();

    int DEBOUNCE_TIME = 250;

    srand(to_us_since_boot(get_absolute_time()));
    int sequencia[100];
    int sequencia_len = 0;
    int acertos = 0;
    int acerto_led = 0;

    alarm_id_t alarm_timeout = add_alarm_in_ms(6000, alarm_timeout_callback, NULL, false);

    u_int32_t time_since_red = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_blue = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_green = to_ms_since_boot(get_absolute_time());
    u_int32_t time_since_yellow = to_ms_since_boot(get_absolute_time());

    while (true) {
        if (game){

            if (alarm_timeout_fired) {
                alarm_timeout_fired = 0;
                play_defeat_music();
                game = false;
                game_over = true;
            }

            if (sequencia_len == acertos) {
                cancel_alarm(alarm_timeout);
                start = false;
                if (sequencia_len > 0) {
                    acerto_led++;
                }
                sequencia[sequencia_len] = ((double) rand()/__RAND_MAX) * (14 - 10) + 10;
                sequencia_len++;
                acertos = 0;
                if (acerto_led > 6) {
                    acerto_led = 1;
                }
                right_led(acerto_led);
                add_alarm_in_ms(1000, alarm_callback, NULL, false);
            }

            if (timer_flag) {
                timer_flag = 0;
                start = true;
                for (int i = 0; i < sequencia_len; i++) {
                    gpio_put(sequencia[i], 1);
                    som(sequencia[i]);
                    gpio_put(sequencia[i], 0);
                    sleep_ms(200);
                }
                sleep_ms(500);
                alarm_timeout = add_alarm_in_us(10000000, alarm_timeout_callback, NULL, false);
            }

            if (red_flag) {
                red_flag = 0;
                if  (to_ms_since_boot(get_absolute_time()) - time_since_red > DEBOUNCE_TIME && start) {
                    time_since_red = to_ms_since_boot(get_absolute_time());
                    gpio_put(RED_LED_PIN, 1);
                    if (sequencia[acertos] == RED_LED_PIN) {
                        acertos++;
                        cancel_alarm(alarm_timeout);
                        alarm_timeout = add_alarm_in_us(10000000, alarm_timeout_callback, NULL, false);
                    } else {
                        play_defeat_music();
                    }
                    som(RED_LED_PIN);
                    gpio_put(RED_LED_PIN, 0);
                }
            }
            if (blue_flag) {
                blue_flag = 0;
                if (to_ms_since_boot(get_absolute_time()) - time_since_blue > DEBOUNCE_TIME && start) {
                    time_since_blue = to_ms_since_boot(get_absolute_time());
                    gpio_put(BLUE_LED_PIN, 1);
                    if (sequencia[acertos] == BLUE_LED_PIN) {
                        acertos++;
                        cancel_alarm(alarm_timeout);
                        alarm_timeout = add_alarm_in_us(10000000, alarm_timeout_callback, NULL, false);
                    } else {
                        play_defeat_music();
                    }
                    som(BLUE_LED_PIN);
                    gpio_put(BLUE_LED_PIN, 0);
                }
            }
            if (green_flag) {
                green_flag = 0;
                if (to_ms_since_boot(get_absolute_time()) - time_since_green > DEBOUNCE_TIME && start) {
                    time_since_green = to_ms_since_boot(get_absolute_time());
                    gpio_put(GREEN_LED_PIN, 1);
                    if (sequencia[acertos] == GREEN_LED_PIN) {
                        acertos++;
                        cancel_alarm(alarm_timeout);
                        alarm_timeout = add_alarm_in_us(10000000, alarm_timeout_callback, NULL, false);
                    } else {
                        play_defeat_music();
                    }
                    som(GREEN_LED_PIN);
                    gpio_put(GREEN_LED_PIN, 0);
                }
            }
            if (yellow_flag) {
                yellow_flag = 0;
                if (to_ms_since_boot(get_absolute_time()) - time_since_yellow > DEBOUNCE_TIME && start) {
                    time_since_yellow = to_ms_since_boot(get_absolute_time());
                    gpio_put(YELLOW_LED_PIN, 1);
                    if (sequencia[acertos] == YELLOW_LED_PIN) {
                        acertos++;
                        cancel_alarm(alarm_timeout);
                        alarm_timeout = add_alarm_in_us(10000000, alarm_timeout_callback, NULL, false);
                    } else {
                        play_defeat_music();
                    }
                    som(YELLOW_LED_PIN);
                    gpio_put(YELLOW_LED_PIN, 0);
                }
            }
        }

        if (game_over) {
            game_over = false;
            sleep_ms(100);
            for (int i = 0; i < sequencia_len - 1; i++) {
                gpio_put(RED_LED_PIN, 1);
                gpio_put(BLUE_LED_PIN, 1);
                gpio_put(GREEN_LED_PIN, 1);
                gpio_put(YELLOW_LED_PIN, 1);
                som(GREEN_LED_PIN);
                gpio_put(RED_LED_PIN, 0);
                gpio_put(BLUE_LED_PIN, 0);
                gpio_put(GREEN_LED_PIN, 0);
                gpio_put(YELLOW_LED_PIN, 0);
                sleep_ms(200);
            }
        }
    }
}
