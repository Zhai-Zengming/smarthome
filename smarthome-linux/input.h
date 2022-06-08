/* 控制工厂，包含语言，socket控制 */

#ifndef __INPUT_H
#define __INPUT_H

#include <stdio.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

struct cmdctl{
    char ctldevname[128];       //区分语音控制还是网络控制
    char devicenode[128];       //控制设备的节点，如/dev/ttymxc2
    int cmd[10];                //接收到的命令存放在此
    int s_fd;                   //socket fd
    char ip[32];
    char port[12];
    void (*init)(struct cmdctl *ctldevice);
    int (*getcmd)(struct cmdctl *ctldevice);

    struct cmdctl *next;
};

struct cmdctl * addvoiceToLink(struct cmdctl *phead);
struct cmdctl * addSocketToLink(struct cmdctl *phead);


#endif