/*
 * 访问翔云官网人脸识别
 * 
 * 2022/4/30
 */

#if 0
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef unsigned int bool;

/* 识别后的结果存放于此 */
char buf[1024] = {'\0'};

size_t readData( void *ptr, size_t size,size_t nmemb, void *stream)
{
	strcpy(buf, ptr);
}

char *getBase64(char *filepath)
{
	char *img;
	char cmd[64] = {'\0'};

	sprintf(cmd, "base64 %s > tmp", filepath);
	system(cmd);
	int fd = open("./tmp", O_RDWR);
	int filesize = lseek(fd, 0, SEEK_END);
	img = (char *)malloc(filesize + 2);
	lseek(fd, 0, SEEK_SET);
	memset(img, 0, filesize + 2);
	read(fd, img, filesize);
	close(fd);
	system("rm tmp -f");

	return img;
}

bool postUrl()
{
    CURL *curl;
    CURLcode res;
	char *key = "D8F9qBLs95JjyWDys43VB2";
	char *secret = "9f1357eba3aa45d496cec005c7425544";
	int typeId = 21;
	char *format = "xml";
	char *str;

	char *img1 = getBase64("./reba1.jpg");
	char *img2 = getBase64("./woman.jpg");

	/* 拼接发送给网页的字符串 */
	int strlength = strlen(key) + strlen(secret) + strlen(img1) + strlen(img2) + 128;
	str = (char *)malloc(strlength);
	memset(str, '\0', strlength);
	sprintf(str, "&img1=%s&img2=%s&key=%s&secret=%s&typeId=%d&format=%s",
					img1, img2, key, secret, typeId, format);
    
	curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);    // 指定post内容
        curl_easy_setopt(curl, CURLOPT_URL, "https://netocr.com/api/faceliu.do");   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData); //设置回调函数
        res = curl_easy_perform(curl);

		printf("return = %d\n", res);
		if(strstr(buf, "是") != NULL){     //如果是同一个人，输出结果中有“是”
			printf("the same person\n");
		}else{
			printf("different person\n");
		}

        curl_easy_cleanup(curl);
    }
    return true;
}
#endif

#include "device.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef unsigned int bool;

/* 识别后的结果存放于此 */
char buf[1024] = {'\0'};
int face_result = 0;

size_t readData( void *ptr, size_t size,size_t nmemb, void *stream)
{
	strcpy(buf, ptr);
}

char *getBase64(char *filepath)
{
	char *img;
	char cmd[64] = {'\0'};

	sprintf(cmd, "base64 %s > tmp", filepath);
	system(cmd);
	int fd = open("./tmp", O_RDWR);
	int filesize = lseek(fd, 0, SEEK_END);
	img = (char *)malloc(filesize + 2);
	lseek(fd, 0, SEEK_SET);
	memset(img, 0, filesize + 2);
	read(fd, img, filesize);
	close(fd);
	system("rm tmp -f");

	return img;
}

bool postUrl()
{
    CURL *curl;
    CURLcode res;
	char *key = "D8F9qBLs95JjyWDys43VB2";
	char *secret = "9f1357eba3aa45d496cec005c7425544";
	int typeId = 21;
	char *format = "xml";
	char *str;

	char *img1 = getBase64("./local.jpg");
	char *img2 = getBase64("./photo.jpg");

	/* 拼接发送给网页的字符串 */
	int strlength = strlen(key) + strlen(secret) + strlen(img1) + strlen(img2) + 128;
	str = (char *)malloc(strlength);
	memset(str, '\0', strlength);
	sprintf(str, "&img1=%s&img2=%s&key=%s&secret=%s&typeId=%d&format=%s",
					img1, img2, key, secret, typeId, format);
    
	curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);    // 指定post内容
        curl_easy_setopt(curl, CURLOPT_URL, "https://netocr.com/api/faceliu.do");   // 指定url
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData); //设置回调函数
        res = curl_easy_perform(curl);

		printf("return = %d\n", res);
		if(strstr(buf, "是") != NULL){     //如果是同一个人，输出结果中有“是”
			face_result = 1;
			printf("the same person\n");
		}else{
			face_result = 0;
			printf("different person\n");
		}

        curl_easy_cleanup(curl);
    }
    return true;
}

void faceIdentifyInit(char *pin)
{
    
}

void faceIdentifyOpen(char *pin)
{
    postUrl();
}

void faceIdenifyClose(char *pin)
{
    
}

struct devices faceIdentify = {
    .devicename = "faceidentify",
    .init = faceIdentifyInit,
    .open = faceIdentifyOpen,
    .close = faceIdenifyClose,

};

struct devices * addFaceIdentifyToLink(struct devices *phead)
{
    if(phead == NULL){
        phead = &faceIdentify;
    }else{
        faceIdentify.next = phead;
        phead = &faceIdentify;
    }
}
