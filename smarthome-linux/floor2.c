#include "device.h"

void floor2ledinit(char *pin)
{
    gpio_init(pin);
}

void floor2ledopen(char *pin)
{
    gpio_config("value", "1");
}

void floor2ledclose(char *pin)
{
    gpio_config("value", "0");
}

struct devices floor2led = {
    .devicename = "floor2led",
    .pinnum = "6",                  //二楼灯是gpio6
    .init = floor2ledinit,
    .open = floor2ledopen,
    .close = floor2ledclose,

};

struct devices * addFloor2ToLink(struct devices *phead)
{
    if(phead == NULL){
        phead = &floor2led;
    }else{
        floor2led.next = phead;
        phead = &floor2led;
    }
}