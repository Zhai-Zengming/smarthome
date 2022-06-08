/*
 * 2022/4/27
 * 2022/6/8基本完成
 */

#include "device.h"
#include "input.h"
#include <pthread.h>

int client_num = 0;
int c_fd[6];                 //注意当有新的socket客户端接入时c_fd是否也会随之改变，从而影响之前的客户端
struct devices *pdevicehead;
struct cmdctl *pcmdctlhead;
struct cmdctl *sockettemp = NULL;
extern int face_result;
int face_flag = 0;         //人脸识别标志
int air_flag = 2;          //空调flag
int pipefd[2];
char pipe_wrbuff[10] = "1";
char pipe_rdbuff[10];

struct devices * DeviceFindByName(char *name, struct devices *phead)
{
    struct devices *temp = phead;
    if(temp == NULL){
        return 0;
    }else{
        while(temp){
            if(strcmp(name, temp->devicename) == 0){
                return temp;
            }
            temp = temp->next;
        }
        return 0;
    }
}

struct cmdctl * CmdDevFindByName(char *name, struct cmdctl *phead)
{
    struct cmdctl *temp = phead;
    if(temp == NULL){
        return 0;
    }else{
        while(temp){
            if(strcmp(name, temp->ctldevname) == 0){
                return temp;
            }
            temp = temp->next;
        }
        return 0;
    }
}

/* 语音线程函数 */
void * voice_func(void *arg)
{
    struct cmdctl *voicetemp = NULL;
    struct devices *devicetemp = NULL;

    voicetemp = CmdDevFindByName("voice", pcmdctlhead);
    if(voicetemp){
        voicetemp->init(voicetemp);

        while(1){                       //不断读取语音命令
            if(voicetemp->getcmd(voicetemp)){

                /* 对不同命令做出处理 */
                printf("voice_get: [%#x]\n", *voicetemp->cmd);
                if(*voicetemp->cmd == 0x10){
                    printf("voice: close bathroomled\n");
                    devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                }else if(*voicetemp->cmd == 0x11){
                    printf("voice: open bathroomled\n");
                    devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x20){
                    printf("voice: close livingroomled\n");
                    devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x21){
                    printf("voice: open livingroomled\n");
                    devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x30){
                    printf("voice: close restaurantroomled\n");
                    devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x31){
                    printf("voice: open restaurantroomled\n");
                    devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x40){
                    printf("voice: close all led\n");
                    devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                    devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                    devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->close(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x41){
                    printf("voice: open all led\n");
                    devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                    devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                    devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                    if(devicetemp){
                        devicetemp->init(devicetemp->pinnum);
                        devicetemp->open(devicetemp->pinnum);
                    }
                }
                else if(*voicetemp->cmd == 0x50){
                    printf("voice: face identification\n");
                    devicetemp = DeviceFindByName("faceidentify", pdevicehead);
                    if(devicetemp){
                        close(pipefd[0]);     //close reading when writting
                        write(pipefd[1], pipe_wrbuff, strlen(pipe_wrbuff));
                        sleep(3);             //等待摄像头线程拍摄完成

                        devicetemp->open(NULL);
                        if(face_result){
                            /*人脸识别通过，开锁*/
                            write(c_fd[0], "71", strlen("71"));
                            write(c_fd[1], "71", strlen("71"));
                            write(c_fd[2], "71", strlen("71"));
                        }
                    }
                }
                else if(*voicetemp->cmd == 0x60){
                    printf("voice: close air conditioner\n");    //关闭空调
                    write(c_fd[0], "60", strlen("60"));
                    write(c_fd[1], "60", strlen("60"));
                    write(c_fd[2], "60", strlen("60"));
                }
                else if(*voicetemp->cmd == 0x61){
                    printf("voice: open air conditioner\n");     //打开空调
                    write(c_fd[0], "61", strlen("61"));
                    write(c_fd[1], "61", strlen("61"));
                    write(c_fd[2], "61", strlen("61"));
                }
            }
        }
    }
}

/* socket客户端接入后读取客户端命令线程函数 */
void * rdwrSocket_func(void *arg)
{
    int client_temp;         //由于会接入多个客户端，此变量存储当前客户端序号
    int n_read;
    struct devices *devicetemp = NULL;

    client_temp = client_num;
    while(1){
        memset(sockettemp->cmd, 0, sizeof(sockettemp->cmd));
        n_read = read(c_fd[client_temp], sockettemp->cmd, sizeof(sockettemp->cmd));
        if(n_read == -1)
        {
            perror("read");
            exit(-1);
        }
        else if(n_read)
        {
            /* 对不同命令做出处理 */
            printf("socket_get: [%#x]\n", *sockettemp->cmd);
            
            if(*sockettemp->cmd == 0x10){
                printf("socket: close bathroomled\n");
                devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
            }else if(*sockettemp->cmd == 0x11){
                printf("socket: open bathroomled\n");
                devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
            }else if(*sockettemp->cmd == 0x20){
                printf("socket: close livingroomled\n");
                devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x21){
                printf("socket: open livingroomled\n");
                devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x30){
                printf("socket: close restaurantroomled\n");
                devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x31){
                printf("socket: open restaurantroomled\n");
                devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x40){
                printf("socket: close all led\n");
                devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
                devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
                devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->close(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x41){
                printf("socket: open all led\n");
                devicetemp = DeviceFindByName("livingroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
                devicetemp = DeviceFindByName("restaurantroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
                devicetemp = DeviceFindByName("bathroomled", pdevicehead);
                if(devicetemp){
                    devicetemp->init(devicetemp->pinnum);
                    devicetemp->open(devicetemp->pinnum);
                }
            }
            else if(*sockettemp->cmd == 0x50){
                printf("socket: face identification\n");
                devicetemp = DeviceFindByName("faceidentify", pdevicehead);
                if(devicetemp){             //通过网络收到人脸识别的命令先向管道写“1”通知摄像头进程拍照
                    close(pipefd[0]);       //close reading when writting of pipe
                    write(pipefd[1], pipe_wrbuff, strlen(pipe_wrbuff));
                    sleep(3);               //等待摄像头线程拍照完成

                    devicetemp->open(NULL);
                    if(face_result){
                        /*人脸识别通过，开锁*/
                        write(c_fd[0], "71", strlen("71"));
                        write(c_fd[1], "71", strlen("71"));
                        write(c_fd[2], "71", strlen("71"));
                    }
                }
            }
            else if((*sockettemp->cmd == 0x60) || (air_flag == 0)){
                write(c_fd[0], "60", strlen("60"));
                write(c_fd[1], "60", strlen("60"));
                write(c_fd[2], "60", strlen("60"));
            }
            else if((*sockettemp->cmd == 0x61) || (air_flag)){
                write(c_fd[0], "61", strlen("61"));
                write(c_fd[1], "61", strlen("61"));
                write(c_fd[2], "61", strlen("61"));
            }
        }
    }
}

/* socket线程函数 */
void * socket_func(void *arg)
{
    int c_len;
    struct sockaddr_in c_addr;
    pthread_t rdwrSocket_thread;

    sockettemp = CmdDevFindByName("socketctrl", pcmdctlhead);
    if(sockettemp){
        sockettemp->init(sockettemp);
    
        memset(&c_addr, 0, sizeof(struct sockaddr_in));
        c_len = sizeof(struct sockaddr_in);

        while(1){                       //不断监听是否有客户端接入
            c_fd[client_num] = accept(sockettemp->s_fd, (struct sockaddr *)&c_addr, (socklen_t *)&c_len);
            if(c_fd[client_num] == -1)
            {
                perror("accept");
                exit(-1);
            }
            /* 有客户端接入后，创建线程读写命令 */
            pthread_create(&rdwrSocket_thread, NULL, rdwrSocket_func, NULL);
            printf("client%d accept!\r\n", client_num);
            sleep(1);
            client_num++;
        }
    }
}

/* 摄像头线程 */
// void * camera_func(void *arg)
// {
//     while(1){
//         if(face_flag){
//             face_flag = 0;
//             /* 照片名存为photo.jpg，与local.jpg对比 */
//             system("gst-launch-1.0 v4l2src num-buffers=1 device=/dev/video1 ! jpegenc ! filesink location=photo.jpg");
//         }
//     }
// }

int main()
{
    pid_t fork_ret;
    pthread_t voice_thread;
    pthread_t socket_thread;
    pthread_t camera_thread;

    /* 1. 指令工厂初始化 */
    /* 将控制设备加入链表 */
    pcmdctlhead = addvoiceToLink(pcmdctlhead);
    pcmdctlhead = addSocketToLink(pcmdctlhead);

    /* 2. 设备工厂初始化 */
    /* 将设备加入链表 */
    pdevicehead = addBathroomToLink(pdevicehead);
    pdevicehead = addLivingroomToLink(pdevicehead);
    pdevicehead = addRestaurantroomToLink(pdevicehead);
    pdevicehead = addFaceIdentifyToLink(pdevicehead);
    
    /* 进程间通过管道通信 */
    pipe(pipefd);
    fork_ret = fork();
    if(fork_ret){
        /* 摄像头拍照进程 */
        // pthread_create(&camera_thread, NULL, camera_func, NULL);
        while(1){
            close(pipefd[1]);
            memset(pipe_rdbuff, '\0', sizeof(pipe_rdbuff));
            read(pipefd[0], pipe_rdbuff, 10);          //不断读取管道是否收到拍照的命令
            if(strcmp(pipe_rdbuff, "1") == 0){
                /* 照片名存为photo.jpg，与local.jpg对比 */
                //system("gst-launch-1.0 v4l2src num-buffers=1 device=/dev/video1 ! jpegenc ! filesink location=photo.jpg &");
                system("./picture &");
            }
        }
    }
    else{
        /* 3. 建立线程池 */
        /* 3.1 语音 */
        pthread_create(&voice_thread, NULL, voice_func, NULL);
        /* 3.2 socket */
        pthread_create(&socket_thread, NULL, socket_func, NULL);
    }

    /* 等待线程退出 */
    pthread_join(socket_thread, NULL);
    pthread_join(voice_thread, NULL);
    // pthread_join(camera_thread, NULL);
    
    return 0;
}
