#ifndef H_BUTTON_
#define H_BUTTON_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "gpio.h"
#include <freertos/queue.h>

static void IRAM_ATTR Button_UP_Interrupt(void *args);
static void IRAM_ATTR Button_DOWN_Interrupt(void *args);
void Interupt_Task(void *pvParameters);
void Setup_Interrupt();

#endif