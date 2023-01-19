#ifndef H_GPIO_
#define H_GPIO_

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>     

#define LED_R       6
#define LED_G       7
#define LED_B       10

#define SOLENOID    18
#define MOTOR       19

#define UP          4 
#define DOWN        8

#define KEY         1
#define PWER_ON     0
#define USB_pin     5

#define SCL         2
#define SDA         9

#define VBATT       3


void Pin_Config();
void Setup_POWER_ON();
void Setup_LED_RED();
void Setup_LED_GREEN();
void Setup_LED_BLUE();
void Setup_KEY();
void Setup_USB();
void Setup_VBATT();
void Setup_UP();
void Setup_DOWN();

void Setup_SCL();
void Setup_SDA();

void Setup_SOLENOID();
void Setup_MOTOR();

#endif