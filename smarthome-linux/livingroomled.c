#include "device.h"

void livingroomledinit(char *pin)
{
    gpio_init(pin);
}

void livingroomledopen(char *pin)
{
    gpio_config("value", "1");
}

void livingroomledclose(char *pin)
{
    gpio_config("value", "0");
}

struct devices livingroomled = {
    .devicename = "livingroomled",
    .pinnum = "2",                  //客厅是gpio2
    .init = livingroomledinit,
    .open = livingroomledopen,
    .close = livingroomledclose,

};

struct devices * addLivingroomToLink(struct devices *phead)
{
    if(phead == NULL){
        phead = &livingroomled;
    }else{
        livingroomled.next = phead;
        phead = &livingroomled;
    }
}