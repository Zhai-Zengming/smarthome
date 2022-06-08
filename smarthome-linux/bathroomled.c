#include "device.h"

void bathroomledinit(char *pin)
{
    gpio_init(pin);
}

void bathroomledopen(char *pin)
{
    gpio_config("value", "1");
}

void bathroomledclose(char *pin)
{
    gpio_config("value", "0");
}

struct devices bathroomled = {
    .devicename = "bathroomled",
    .pinnum = "1",                  //浴室灯是gpio1
    .init = bathroomledinit,
    .open = bathroomledopen,
    .close = bathroomledclose,

};

struct devices * addBathroomToLink(struct devices *phead)
{
    if(phead == NULL){
        phead = &bathroomled;
    }else{
        bathroomled.next = phead;
        phead = &bathroomled;
    }
}