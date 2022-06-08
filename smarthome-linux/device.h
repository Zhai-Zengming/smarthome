/* 可用gpio有1，2，4 */

#ifndef __DEVICE_H
#define __DEVICE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char gpio_path[100];

struct devices
{
    char devicename[128];
    char pinnum[10];
    void (*init)(char *pin);
    void (*open)(char *pin);
    void (*close)(char *pin);

    struct devices *next;
};

int gpio_config(char *attr, char *val);
void gpio_init(char *pin);

struct devices * addBathroomToLink(struct devices *phead);
struct devices * addRestaurantroomToLink(struct devices *phead);
struct devices * addLivingroomToLink(struct devices *phead);
struct devices * addFloor2ToLink(struct devices *phead);
struct devices * addFaceIdentifyToLink(struct devices *phead);


#endif // !__DEVICE_H
