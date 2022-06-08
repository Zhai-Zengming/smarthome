#include "device.h"

/* 配置gpio的属性，如direction, active_low,  */
int gpio_config(char *attr, char *val)
{
    int fd;
    char file_path[128];
    memset(file_path, '\0', sizeof(file_path));
    sprintf(file_path, "%s/%s", gpio_path, attr);

    fd = open(file_path, O_WRONLY);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    if(write(fd, val, strlen(val)) == -1)
    {
        perror("write");
        close(fd);
        return(-1);
    }

    close(fd);

    return 0;
}

void gpio_init(char *pin)
{
    /* 导出gpio引脚 */
    memset(gpio_path, '\0', sizeof(gpio_path));
    sprintf(gpio_path, "/sys/class/gpio/gpio%s", pin);
    if(access(gpio_path, F_OK))
    {
        int fd;
        
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if(fd == -1)
        {
            perror("open export");
            exit(-1);
        }

        if(write(fd, pin, strlen(pin)) == -1)
        {
            perror("write export");
            close(fd);
            exit(-1);
        }
        close(fd);
    }

    /* 默认输出低电平 */
    if(gpio_config("direction", "out"))
        exit(-1);
    if(gpio_config("active_low", "0"))
        exit(-1);
    if(gpio_config("value", "0"))
        exit(-1);
}