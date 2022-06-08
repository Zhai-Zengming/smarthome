/* 
 * 语音模块通过串口与开发版
 * 连接，所以该文件主要配置串口
 */
#define _GNU_SOURCE     //在源文件开头定义_GNU_SOURCE宏
#include "input.h"

static struct termios old_cfg;  //用于保存终端的配置参数
static int fd;      //串口终端对应的文件描述符
int rdbuf[10] = {'\0'};

/**
 ** 串口初始化操作
 ** 参数device表示串口终端的设备节点
 **/
static int uart_init(char *devicenode)
{
    /* 打开串口终端 */
    fd = open(devicenode, O_RDWR | O_NOCTTY);
    if (0 > fd) {
        fprintf(stderr, "open error: %s: %s\n", devicenode, strerror(errno));
        return -1;
    }

    /* 获取串口当前的配置参数 */
    if (0 > tcgetattr(fd, &old_cfg)) {
        fprintf(stderr, "tcgetattr error: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    return 0;
}


/**
 ** 串口配置
 ** 参数cfg指向一个uart_cfg_t结构体对象
 **/
static int uart_cfg()
{
    struct termios new_cfg = {0};   //将new_cfg对象清零
    speed_t speed;

    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    /* 使能接收 */
    new_cfg.c_cflag |= CREAD;

    /* 设置波特率 */
    speed = B9600;
   
    if (0 > cfsetspeed(&new_cfg, speed)) {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        return -1;
    }

    /* 设置数据位大小 */
    new_cfg.c_cflag |= CS8;

    /* 设置奇偶校验 */
    new_cfg.c_cflag &= ~PARENB;
    new_cfg.c_iflag &= ~INPCK;

    /* 设置停止位 */
    new_cfg.c_cflag &= ~CSTOPB;

    /* 将MIN和TIME设置为0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;

    /* 清空缓冲区 */
    if (0 > tcflush(fd, TCIOFLUSH)) {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        return -1;
    }

    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(fd, TCSANOW, &new_cfg)) {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        return -1;
    }

    /* 配置OK 退出 */
    return 0;
}

/**
 ** 信号处理函数，当串口有数据可读时，会跳转到该函数执行
 **/
static void io_handler(int sig, siginfo_t *info, void *context)
{
    int ret;
    int n;

    if(SIGRTMIN != sig)
        return;

    /* 判断串口是否有数据可读 */
    if (POLL_IN == info->si_code) {
        ret = read(fd, rdbuf, 8);     //一次最多读8个字节数据
        // printf("[ ");
        // for (n = 0; n < ret; n++)
        //     printf("0x%hhx ", rdbuf[n]);
        // printf("]\n");
    }
}

/**
 ** 异步I/O初始化函数
 **/
static void async_io_init(void)
{
    struct sigaction sigatn;
    int flag;

    /* 使能异步I/O */
    flag = fcntl(fd, F_GETFL);  //使能串口的异步I/O功能
    flag |= O_ASYNC;
    fcntl(fd, F_SETFL, flag);

    /* 设置异步I/O的所有者 */
    fcntl(fd, F_SETOWN, getpid());

    /* 指定实时信号SIGRTMIN作为异步I/O通知信号 */
    fcntl(fd, F_SETSIG, SIGRTMIN);

    /* 为实时信号SIGRTMIN注册信号处理函数 */
    sigatn.sa_sigaction = io_handler;   //当串口有数据可读时，会跳转到io_handler函数
    sigatn.sa_flags = SA_SIGINFO;
    sigemptyset(&sigatn.sa_mask);
    sigaction(SIGRTMIN, &sigatn, NULL);
}

void serialinit(struct cmdctl *ctldevice)
{
    /* 串口初始化 */
    if (uart_init(ctldevice->devicenode))
        exit(EXIT_FAILURE);

    /* 串口配置 */
    if (uart_cfg()) {
        tcsetattr(fd, TCSANOW, &old_cfg);   //恢复到之前的配置
        close(fd);
        exit(EXIT_FAILURE);
    }
}

int getserialcmd(struct cmdctl *ctldevice)
{
    //读串口数据
    async_io_init();	//我们使用异步I/O方式读取串口的数据，调用该函数去初始化串口的异步I/O

    if(*rdbuf != 0){
//        strcpy(ctldevice->cmd, rdbuf);
        *ctldevice->cmd = *rdbuf;
        memset(rdbuf, 0, sizeof(rdbuf));
        return 1;
    }
    return 0;
}

struct cmdctl voice = {
    .ctldevname= "voice",
    .devicenode = "/dev/ttymxc2",
    .init = serialinit,
    .getcmd = getserialcmd,
};

struct cmdctl * addvoiceToLink(struct cmdctl *phead)
{
    if(phead == NULL){
        phead = &voice;
    }else{
        voice.next = phead;
        phead = &voice;
    }
}