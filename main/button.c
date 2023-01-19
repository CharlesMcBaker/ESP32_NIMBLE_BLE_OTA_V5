#include "button.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

QueueHandle_t Button_UP_Queue;
QueueHandle_t Button_DOWN_Queue;

static void IRAM_ATTR Button_UP_Interrupt(void *args)
{
    int int_to_send_up = 2;
    xQueueSendFromISR(Button_UP_Queue, &int_to_send_up, NULL);
}

static void IRAM_ATTR Button_DOWN_Interrupt(void *args)
{
    int int_to_send_down = 2;
    xQueueSendFromISR(Button_DOWN_Queue, &int_to_send_down, NULL);
}

void Setup_Interrupt()
{
    gpio_install_isr_service(0);    // could be cause. 
    gpio_isr_handler_add(UP, Button_UP_Interrupt, (void *)UP);
    gpio_isr_handler_add(DOWN, Button_DOWN_Interrupt, (void *)DOWN);
}

void Interupt_Task(void *pvParameters)
{
    Setup_Interrupt();

    Button_UP_Queue     =   xQueueCreate(20, sizeof(int));
    Button_DOWN_Queue   =   xQueueCreate(20, sizeof(int));

    int recieved_up     =   20;
    int recieved_down   =   20;

    while(1)
    {
        if (xQueueReceive(Button_UP_Queue, &recieved_up, portMAX_DELAY))
        {
            printf("Interrupt triggered on UP button\n");
        }

        if (xQueueReceive(Button_DOWN_Queue, &recieved_down, portMAX_DELAY))
        {
            printf("Interrupt triggered on DOWN button\n");
        }
        vTaskDelay(200/portTICK_RATE_MS);
    }
}