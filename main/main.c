#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define NUM_LEDS 4
#include "ws2812_control.h"
#define GREEN   0xFF0000          
#define RED     0x00FF00          
#define BLUE    0x0000FF

#define PWR_SW_PIN              16
#define GPIO_OUTPUT_PIN_SEL     (1ULL<<PWR_SW_PIN)

#define TOP_LED     3
#define RIGHT_LED   1      
#define LEFT_LED    2
#define CNTR_LED    0

void init_GPIO(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    gpio_set_level(PWR_SW_PIN, 1);
}


// will set all LEDs to the colours data
void config_all_LEDs(uint8_t colours[3])
{
    struct led_state new_state;
    uint32_t LED_state = (colours[1] << 16) | (colours[0] << 8) | (colours[2]);

    new_state.leds[TOP_LED] = LED_state;      // Centre
    new_state.leds[RIGHT_LED] = LED_state;      // Bottom Right
    new_state.leds[LEFT_LED] = LED_state;       // Bottom Left
    new_state.leds[CNTR_LED] = LED_state;      // Top
    ws2812_write_leds(new_state);
}


// change the LEDs to the new colour from the old colour by fading it across
void alt2colour(uint8_t colours[3], uint8_t new_colours[3])
{
    // 0 - no change
    // 1 - increase
    // -1 - decrease
    uint8_t change_RED = 0;
    uint8_t change_GREEN = 0;
    uint8_t change_BLUE = 0;

    uint8_t current_colour = colours;      // RED, GREEN, BLUE

    if(colours[0] > new_colours[0])
    {
        change_RED = -1;
    }
    else if(colours[0] < new_colours[0])
    {
        change_RED = 1;
    }

    if(colours[1] > new_colours[1])
    {
        change_GREEN = -1;
    }
    else if(colours[1] < new_colours[1])
    {
        change_GREEN = 1;
    }
    if(colours[2] > new_colours[2])
    {
        change_BLUE = -1;
    }
    else if(colours[2] < new_colours[2])
    {
        change_BLUE = 1;
    }

    

}


// rainbow colour to mix through the rainbow colours
void rainbow(int delay)
{
    //violet(purple), indigo, blue, green, yellow, orange, red

    // yellow 255 red, green
    // violet red 126, blue 255
    // indigo


    // cycle from one colour to the next in the correct order and then back in reverse back to the start

    uint8_t led_colour[] = {0x00, 0x00, 0x00};      // RED, GREEN, BLUE

    // do voilet, indigo first

    //blue colour
    led_colour[0] = 0x00;
    led_colour[1] = 0x00;
    led_colour[2] = 0xFF;

    // turn from blue into green
    for(int i = 0; i<255; i++)
    {
        led_colour[1] = i;
        led_colour[2] -= i;   
        config_all_LEDs(led_colour); 
        vTaskDelay(delay);
    }

    // turn from green into yellow and then orange
    for(int i = 0; i<255; i++)
    {
        led_colour[0] = i;   
        config_all_LEDs(led_colour); 
        vTaskDelay(delay);
    }

    // turn from orange into red
    for(int i = 0; i<255; i++)
    {
        led_colour[1] = 255 - i;  
        config_all_LEDs(led_colour); 
        vTaskDelay(delay);
    }



}

void app_main()
{
    init_GPIO();
    printf("Hello world!\n");
    ws2812_control_init();
    printf("LEDs setup\n");
    
    struct led_state new_state;
    uint8_t led_colour[] = {0x00, 0x00, 0xFF};      // RED, GREEN, BLUE
    config_all_LEDs(led_colour);
    
    while(1){
        int i =0;
        int j=0;
        int k=0;
        int delay =1;

        for(i=0;i<256;i++)
        {
            led_colour[0] = i;
            led_colour[1] = j;
            led_colour[2] = k;
            config_all_LEDs(led_colour);
            vTaskDelay(delay);
        }
        i=255;
        for(j=0;j<256;j++)
        {
            led_colour[0] = i;
            led_colour[1] = j;
            led_colour[2] = k;
            config_all_LEDs(led_colour);
            vTaskDelay(delay);
        }
        j=255;

        for(k=0;k<256;k++)
        {
            led_colour[0] = i;
            led_colour[1] = j;
            led_colour[2] = k;
            config_all_LEDs(led_colour);
            vTaskDelay(delay);
        }       
    }
}