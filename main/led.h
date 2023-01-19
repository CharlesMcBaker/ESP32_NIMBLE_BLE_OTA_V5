#ifndef H_LED_
#define H_LED_

#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "gpio.h"

//FreeRtos
#include "freertos/FreeRTOSConfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#define RGB_Timer            LEDC_TIMER_0
#define RGB_Speed_Mode       LEDC_LOW_SPEED_MODE
#define RGB_Duty_Res         LEDC_TIMER_13_BIT
#define RGB_Mode             LEDC_LOW_SPEED_MODE
#define RGB_Freq             (5000)
#define RGB_Clock            LEDC_AUTO_CLK
#define RGB_Interupt         LEDC_INTR_DISABLE

#define LED_R_GPIO_num       (LED_R)
#define LED_G_GPIO_num       (LED_G)
#define LED_B_GPIO_num       (LED_B)

#define LED_R_Channel        LEDC_CHANNEL_0
#define LED_G_Channel        LEDC_CHANNEL_1
#define LED_B_Channel        LEDC_CHANNEL_2




void LED_R_INIT();
void LED_G_INIT();
void LED_B_INIT();

void LED_R_BLINK(int loops, int time_duration, int low_val, int high_val);
void LED_G_BLINK(int loops, int time_duration, int low_val, int high_val);
void LED_B_BLINK(int loops, int time_duration, int low_val, int high_val);

void LED_W_BLINK(int loops, int time_duration, int low_val, int high_val);

void LED_RGB_OFF();
void Setup_Led();


#endif