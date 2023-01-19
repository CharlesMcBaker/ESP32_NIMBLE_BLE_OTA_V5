#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"

//FreeRtos
#include "freertos/FreeRTOSConfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

//GPIO
#include <driver/gpio.h>

//Custom Files
#include "utill.h"
#include "gapp.h"
#include "gatt.h"
#include "gpio.h"
#include "ota.h"
#include "led.h"
#include "modes.h"
#include "button.h"

//Battery
#include "driver/adc.h"
#include "esp_adc_cal.h"

// Version 5

void Timed_Task(void *pvParameters);
void Battery_Task(void *pvParameters);
void Led_Task(void *pvParameters);
void Button_Task(void *pvParameters);
void Modes_Task(void *pvParameters);
void Test_Task(void *pvParameters);

TaskHandle_t tim_task_handle = NULL;
TaskHandle_t bat_task_handle = NULL;
TaskHandle_t led_task_handle = NULL;
TaskHandle_t but_task_handle = NULL;
TaskHandle_t mod_task_handle = NULL;

    struct modes 
    {
    bool Bluetooth;
    bool Mode_1;
    bool Mode_2;
    bool Charging;
    };

QueueHandle_t queue;

void app_main(void)
{
    struct modes Createmode;
    queue = xQueueCreate(10, sizeof(Createmode));

    printf("Start\n");
    Pin_Config();
    check_partition();
    check_ota();
    BLE_Config();

    //xTaskCreate(&Timed_Task,"Timed_Task",2048,NULL,5,NULL);
    //xTaskCreate(&Battery_Task,"Battery_Task",2048,NULL,5,NULL);
    xTaskCreate(&Led_Task,"Led_Task",2048,NULL,6,NULL);
    xTaskCreate(&Button_Task,"Led_Task",2048,NULL,6,NULL);
    //xTaskCreate(&Modes_Task,"Modes_Task",2048,NULL,6,NULL);
    xTaskCreate(&Interupt_Task,"Interupt_Task",2048,NULL,6,NULL);
    //xTaskCreate(&Test_Task,"Test_Task",2048,NULL,6,NULL);
}

void Modes_Task(void *pvParameters)
{
    struct modes current_mode;

    current_mode.Bluetooth  = false;
    current_mode.Mode_1     = false;
    current_mode.Mode_2     = false;
    current_mode.Charging   = false;


    while(1)
    {

        printf("Setting Table 1:  0 0 0 0\n");
        current_mode.Bluetooth  = false;
        current_mode.Mode_1     = false;
        current_mode.Mode_2     = false;
        current_mode.Charging   = false;
        xQueueSend(queue, &current_mode, portMAX_DELAY);
        vTaskDelay(1000/portTICK_RATE_MS);


        printf("Setting Table 1:  0 1 1 0\n");
        current_mode.Bluetooth  = false;
        current_mode.Mode_1     = true;
        current_mode.Mode_2     = true;
        current_mode.Charging   = false;
        xQueueSend(queue, &current_mode, portMAX_DELAY);
        vTaskDelay(1000/portTICK_RATE_MS);

        printf("Setting Table 1:  0 0 0 0\n");
        current_mode.Bluetooth  = false;
        current_mode.Mode_1     = false;
        current_mode.Mode_2     = false;
        current_mode.Charging   = true;
        xQueueSend(queue, &current_mode, portMAX_DELAY);
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void Timed_Task(void *pvParameters)
{
    struct modes current_mode;

    while(1)
    {
        xQueueReceive(queue, &current_mode, portMAX_DELAY);
        printf("current_mode.Bluetooth :%d\n", current_mode.Bluetooth);
        printf("current_mode.Mode_1    :%d\n", current_mode.Mode_1);
        printf("current_mode.Mode_2    :%d\n", current_mode.Mode_2);
        printf("current_mode.Charging  :%d\n", current_mode.Charging);

        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void Battery_Task(void *pvParameters)
{
    static const char *TAG = "ADC CONVERSION";
    static esp_adc_cal_characteristics_t adc1_chars;

    uint32_t voltage;
    float charge;

    static uint8_t battery[1];
    struct os_mbuf *om;
    battery[0] = 0;

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11));    

    while(1)
    {
        voltage     =   esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_3), &adc1_chars);
        charge      =   (((float)voltage-1800)/720)*100;
        battery[0]  =   (int)charge;

            if(CONNECTION_STATE == true)
            {
                om = ble_hs_mbuf_from_flat(battery, sizeof(battery));
                ble_gattc_notify_custom(conn_handle,battery_handle,om);
            }

        ESP_LOGI(TAG, "CHARGE..........:%.2f", charge);
        ESP_LOGI(TAG, "ADC1_CHANNEL_6..: %d mV", voltage);
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}

void Led_Task(void *pvParameters)
{
    Setup_Led();
    printf("LAST MESSAGE 2\n");
    while(1)
    {
        while(CONNECTION_STATE == false)
        {
            LED_B_BLINK(1,500,0,200);     
        }
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,200,NULL));
        vTaskDelay(200/portTICK_RATE_MS);
    }
}

void Button_Task(void *pvParameters)
{
    bool button_bool = false;
    bool button_bool_now = false;

    vTaskDelay(4000/portTICK_RATE_MS);
    while(1)
    {
        button_bool_now = false;

        if(gpio_get_level(KEY) == 0)
        {
            button_bool = true;
            button_bool_now = true;
        }
        else if(gpio_get_level(KEY) == 1)
        {
            button_bool = false;
            button_bool_now = false;
        }

            if(button_bool == 1 && button_bool_now == 1)
            {
                LED_W_BLINK(4,50,0,4000);

                gpio_set_level(PWER_ON,0);
                esp_restart();
            }

        vTaskDelay(3000/portTICK_RATE_MS);
    }
}

void Test_Task(void *pvParameters)
{
    vTaskDelay(4000/portTICK_RATE_MS);
    while(1)
    {
        if(gpio_get_level(UP) == 0)
        {
            printf("UP is pressed \n");
        }
        if(gpio_get_level(DOWN) == 0)
        {
            printf("DOWN is pressed \n");
        }
        if(gpio_get_level(KEY) == 0)
        {
            printf("KEY is pressed \n");
        }
        vTaskDelay(100/portTICK_RATE_MS);
    }
}