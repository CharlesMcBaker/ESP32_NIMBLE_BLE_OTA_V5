#include "gpio.h"

    void Pin_Config()
    {
        Setup_POWER_ON();  
        Setup_MOTOR();
        Setup_SOLENOID();

        Setup_SCL();
        Setup_SDA();

        Setup_KEY();
        Setup_USB();
        Setup_VBATT();

        Setup_UP();
        Setup_DOWN();

        Setup_SCL();
        Setup_SDA();

        Setup_LED_GREEN();
        Setup_LED_BLUE();
        Setup_LED_RED();
    }
    
    void Setup_POWER_ON()
    {
        gpio_config_t PWER_ON_CONFIG;
        PWER_ON_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        PWER_ON_CONFIG.mode = GPIO_MODE_OUTPUT;
        PWER_ON_CONFIG.pin_bit_mask = (1ULL << PWER_ON); 
        PWER_ON_CONFIG.pull_down_en = 0; 
        PWER_ON_CONFIG.pull_up_en = 1; 
        gpio_config(&PWER_ON_CONFIG); 
        gpio_set_level(PWER_ON,1);
    }
   
    void Setup_USB()
    {
        gpio_config_t USB_pin_CONFIG;
        USB_pin_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        USB_pin_CONFIG.mode = GPIO_MODE_INPUT; 
        USB_pin_CONFIG.pin_bit_mask = (1ULL << USB_pin); 
        USB_pin_CONFIG.pull_down_en = 0; 
        USB_pin_CONFIG.pull_up_en = 1; 
        gpio_config(&USB_pin_CONFIG); 
    }

    void Setup_VBATT()
    {
        gpio_config_t VBATT_CONFIG;
        VBATT_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        VBATT_CONFIG.mode = GPIO_MODE_INPUT; 
        VBATT_CONFIG.pin_bit_mask = (1ULL << VBATT); 
        VBATT_CONFIG.pull_down_en = 0; 
        VBATT_CONFIG.pull_up_en = 1; 
        gpio_config(&VBATT_CONFIG); 
    }

    void Setup_KEY()
    {
        gpio_config_t KEY_CONFIG;
        KEY_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        KEY_CONFIG.mode = GPIO_MODE_INPUT; 
        KEY_CONFIG.pin_bit_mask = (1ULL << KEY); 
        KEY_CONFIG.pull_down_en = 0; 
        KEY_CONFIG.pull_up_en = 1; 
        gpio_config(&KEY_CONFIG); 
    }

    void Setup_UP()
    {
        gpio_config_t UP_CONFIG;
        UP_CONFIG.intr_type = GPIO_INTR_POSEDGE;        // interrupt 
        UP_CONFIG.mode = GPIO_MODE_INPUT; 
        UP_CONFIG.pin_bit_mask = (1ULL << UP); 
        UP_CONFIG.pull_down_en = 0; 
        UP_CONFIG.pull_up_en = 0; 
        gpio_config(&UP_CONFIG); 
    }

    void Setup_DOWN()
    {
        gpio_config_t DOWN_CONFIG;
        DOWN_CONFIG.intr_type = GPIO_INTR_POSEDGE;       // interrupt       
        DOWN_CONFIG.mode = GPIO_MODE_INPUT; 
        DOWN_CONFIG.pin_bit_mask = (1ULL << DOWN); 
        DOWN_CONFIG.pull_down_en = 0; 
        DOWN_CONFIG.pull_up_en = 0; 
        gpio_config(&DOWN_CONFIG); 
    }

    void Setup_MOTOR()
    {
        //gpio_reset_pin(gpio_num_t LED_R);
        gpio_config_t MOTOR_CONFIG;
        MOTOR_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        MOTOR_CONFIG.mode = GPIO_MODE_INPUT; 
        MOTOR_CONFIG.pin_bit_mask = (1ULL << MOTOR); 
        MOTOR_CONFIG.pull_down_en = 0;
        MOTOR_CONFIG.pull_up_en = 0; 
        gpio_config(&MOTOR_CONFIG); 
        gpio_set_level(MOTOR,0);
    }

    void Setup_SOLENOID()
    {
        gpio_config_t SOLENOID_CONFIG;
        SOLENOID_CONFIG.intr_type = GPIO_INTR_DISABLE;
        SOLENOID_CONFIG.mode = GPIO_MODE_INPUT; 
        SOLENOID_CONFIG.pin_bit_mask = (1ULL << SOLENOID); 
        SOLENOID_CONFIG.pull_down_en = 0; 
        SOLENOID_CONFIG.pull_up_en = 0;
        gpio_config(&SOLENOID_CONFIG); 
        gpio_set_level(SOLENOID,0);
    }

    void Setup_LED_RED()
    {
        gpio_reset_pin(LED_R);
        gpio_config_t LED_R_CONFIG;
        LED_R_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        LED_R_CONFIG.mode = GPIO_MODE_OUTPUT; 
        LED_R_CONFIG.pin_bit_mask = (1ULL << LED_R); 
        LED_R_CONFIG.pull_down_en = 1; 
        LED_R_CONFIG.pull_up_en = 0; 
        gpio_config(&LED_R_CONFIG); 
        gpio_set_level(LED_R,0);
    }

    void Setup_LED_BLUE()
    {
        gpio_reset_pin(LED_B);
        gpio_config_t LED_B_CONFIG;
        LED_B_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        LED_B_CONFIG.mode = GPIO_MODE_OUTPUT; 
        LED_B_CONFIG.pin_bit_mask = (1ULL << LED_B); 
        LED_B_CONFIG.pull_down_en = 0; 
        LED_B_CONFIG.pull_up_en = 0; 
        gpio_config(&LED_B_CONFIG); 
        gpio_set_level(LED_B,0);
    }

    void Setup_LED_GREEN()
    {
        gpio_reset_pin(LED_G);
        gpio_config_t LED_G_CONFIG;
        LED_G_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        LED_G_CONFIG.mode = GPIO_MODE_OUTPUT; 
        LED_G_CONFIG.pin_bit_mask = (1ULL << LED_G); 
        LED_G_CONFIG.pull_down_en = 0; 
        LED_G_CONFIG.pull_up_en = 0; 
        gpio_config(&LED_G_CONFIG);
        gpio_set_level(LED_G,0);
    }

    void Setup_SCL()
    {
        gpio_config_t SCL_CONFIG;
        SCL_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        SCL_CONFIG.mode = GPIO_MODE_INPUT; 
        SCL_CONFIG.pin_bit_mask = (1ULL << SCL); 
        SCL_CONFIG.pull_down_en = 0; 
        SCL_CONFIG.pull_up_en = 1; 
        gpio_config(&SCL_CONFIG); 
    }

    void Setup_SDA()
    {
        gpio_reset_pin(SDA);
        gpio_config_t SDA_CONFIG;
        SDA_CONFIG.intr_type = GPIO_INTR_DISABLE; 
        SDA_CONFIG.mode = GPIO_MODE_INPUT; 
        SDA_CONFIG.pin_bit_mask = (1ULL << SDA); 
        SDA_CONFIG.pull_down_en = 0;
        SDA_CONFIG.pull_up_en = 1; 
        gpio_config(&SDA_CONFIG); 
    }
