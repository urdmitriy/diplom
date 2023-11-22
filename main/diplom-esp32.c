#include <stdio.h>
#include "leds.h"



void app_main(void)
{
    leds_init();

    while (1){
        leds_flash(LED_YELLOW, 1000);
        leds_flash(LED_RED, 500);
        leds_flash(LED_WHITE, 250);
    }
}
