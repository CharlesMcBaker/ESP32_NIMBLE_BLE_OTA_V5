#include "led.h"


void Setup_Led()
{
    LED_R_INIT();
    LED_G_INIT();
    LED_B_INIT();
    ledc_fade_func_install(0);  // Must be installed after
    printf("LAST MESSAGE\n");
}


void LED_R_BLINK(int loops, int time_duration, int low_val, int high_val)
{
    LED_RGB_OFF();
    for (int i = 0; i <= loops; ++i)
    {
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_R_Channel,low_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);

        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_R_Channel,high_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);
    }
    LED_RGB_OFF();
}


void LED_G_BLINK(int loops, int time_duration, int low_val, int high_val)
{
    LED_RGB_OFF();
    for (int i = 0; i <= loops; ++i)
    {
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_G_Channel,low_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);

        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_G_Channel,high_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);
    }
    LED_RGB_OFF();
}

void LED_B_BLINK(int loops, int time_duration, int low_val, int high_val)
{
    LED_RGB_OFF();
    for (int i = 0; i <= loops; ++i)
    {
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,low_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);

        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,high_val,NULL));
        vTaskDelay(time_duration/portTICK_RATE_MS);
    }
    LED_RGB_OFF();
}

void LED_W_BLINK(int loops, int time_duration, int low_val, int high_val)
{
    for (int i = 0; i <= loops; ++i)
    {
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_R_Channel,low_val,NULL));
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_G_Channel,low_val,NULL));
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,low_val,NULL));

        vTaskDelay(time_duration/portTICK_RATE_MS);

        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_R_Channel,high_val,NULL));
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_G_Channel,high_val,NULL));
        ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,high_val,NULL));

        vTaskDelay(time_duration/portTICK_RATE_MS);
    }
    LED_RGB_OFF();
}

void LED_RGB_OFF()
{
    ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_R_Channel,0,NULL));
    ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_G_Channel,0,NULL));
    ESP_ERROR_CHECK(ledc_set_duty_and_update(RGB_Speed_Mode,LED_B_Channel,0,NULL));
}


void LED_R_INIT()
{
    ledc_timer_config_t R_ledc_timer = 
    {
        .speed_mode       = RGB_Speed_Mode,                 //
        .timer_num        = RGB_Timer,                      //
        .duty_resolution  = RGB_Duty_Res,                   //
        .freq_hz          = RGB_Freq,                       //
        .clk_cfg          = RGB_Clock                       //
    };
    ESP_ERROR_CHECK(ledc_timer_config(&R_ledc_timer));

    ledc_channel_config_t R_ledc_channel = 
    {
        .speed_mode      = RGB_Mode,                         //
        .channel         = LED_R_Channel,                    //   Update
        .timer_sel       = RGB_Timer,                        //
        .intr_type       = RGB_Interupt,                     //
        .gpio_num        = LED_R_GPIO_num,                   //   Update
        .duty            = 0,                                //
        .hpoint          = 0                                 //
    };
    ESP_ERROR_CHECK(ledc_channel_config(&R_ledc_channel));
}

void LED_G_INIT()
{
    ledc_timer_config_t G_ledc_timer = 
    {
        .speed_mode       = RGB_Speed_Mode,                 //
        .timer_num        = RGB_Timer,                      //
        .duty_resolution  = RGB_Duty_Res,                   //
        .freq_hz          = RGB_Freq,                       //
        .clk_cfg          = RGB_Clock                       //
    };
    ESP_ERROR_CHECK(ledc_timer_config(&G_ledc_timer));

    ledc_channel_config_t G_ledc_channel = 
    {
        .speed_mode      = RGB_Mode,                         //
        .channel         = LED_G_Channel,                    //   Update
        .timer_sel       = RGB_Timer,                        //
        .intr_type       = RGB_Interupt,                     //
        .gpio_num        = LED_G_GPIO_num,                   //   Update
        .duty            = 0,                                //
        .hpoint          = 0                                 //
    };
    ESP_ERROR_CHECK(ledc_channel_config(&G_ledc_channel));
}


void LED_B_INIT()
{
    ledc_timer_config_t B_ledc_timer = 
    {
        .speed_mode       = RGB_Speed_Mode,                 //
        .timer_num        = RGB_Timer,                      //
        .duty_resolution  = RGB_Duty_Res,                   //
        .freq_hz          = RGB_Freq,                       //
        .clk_cfg          = RGB_Clock                       //
    };
    ESP_ERROR_CHECK(ledc_timer_config(&B_ledc_timer));

    ledc_channel_config_t B_ledc_channel = 
    {
        .speed_mode      = RGB_Mode,                         //
        .channel         = LED_B_Channel,                    //   Update
        .timer_sel       = RGB_Timer,                        //
        .intr_type       = RGB_Interupt,                     //
        .gpio_num        = LED_B_GPIO_num,                   //   Update
        .duty            = 0,                                //
        .hpoint          = 0                                 //
    };
    ESP_ERROR_CHECK(ledc_channel_config(&B_ledc_channel));
}





