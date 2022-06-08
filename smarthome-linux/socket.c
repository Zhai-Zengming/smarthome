#include "input.h"

void socketinit(struct cmdctl *ctldevice)
{
    int s_fd;
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));

    //1.socket
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(s_fd == -1)
	{
		perror("socket");
		exit(-1);
	}

    //2.bind
	sock_addr.sin_family = AF_INET;               //select the internet
	sock_addr.sin_port = htons(atoi(ctldevice->port));    //host to net short, change the byte sequence
	inet_aton(ctldevice->ip, &sock_addr.sin_addr);

	bind(s_fd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr_in));

    //3.listen
	listen(s_fd, 10);

    ctldevice->s_fd = s_fd;
}

/* 此函数没用到，在main.c的线程里面实现此函数功能 */
int getsocketcmd(struct cmdctl *ctldevice)
{
    int c_fd;
    int c_len;
    struct sockaddr_in c_addr;

    memset(&c_addr, 0, sizeof(struct sockaddr_in));
    c_len = sizeof(struct sockaddr_in);

    //4.accept
    c_fd = accept(ctldevice->s_fd, (struct sockaddr *)&c_addr, (socklen_t *)&c_len);
    if(c_fd == -1)
    {
        perror("accept");
        exit(-1);
    }

    //5.read
    int n_read;
    n_read = read(c_fd, ctldevice->cmd, sizeof(ctldevice->cmd));
    if(n_read == -1)
    {
        perror("read");
        exit(-1);
    }
    printf("get: %s\n", ctldevice->cmd);
}

struct cmdctl socketctrl = {
    .ctldevname= "socketctrl",
    .ip = "192.168.0.15",
    .port = "1234",
    .init = socketinit,
    .getcmd = getsocketcmd,
};

struct cmdctl * addSocketToLink(struct cmdctl *phead)
{
    if(phead == NULL){
        phead = &socketctrl;
    }else{
        socketctrl.next = phead;
        phead = &socketctrl;
    }
}