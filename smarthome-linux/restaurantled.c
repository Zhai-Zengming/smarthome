#include "device.h"

void restaurantroomledinit(char *pin)
{
    gpio_init(pin);
}

void restaurantroomledopen(char *pin)
{
    gpio_config("value", "1");
}

void restaurantroomledclose(char *pin)
{
    gpio_config("value", "0");
}

struct devices restaurantroomled = {
    .devicename = "restaurantroomled",
    .pinnum = "4",                  //餐厅是gpio4 gpio9不能被导出
    .init = restaurantroomledinit,
    .open = restaurantroomledopen,
    .close = restaurantroomledclose,

};

struct devices * addRestaurantroomToLink(struct devices *phead)
{
    if(phead == NULL){
        phead = &restaurantroomled;
    }else{
        restaurantroomled.next = phead;
        phead = &restaurantroomled;
    }
}