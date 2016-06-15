#include "../unp/unp.h"
#include "../unp/apueerror.h"

//http://tf.nist.gov/tf-cgi/servers.cgi  IP服务列表
/*
 * 包裹函数
 */
int Socket(int family, int type, int protocol)
{
    int n;
    if ((n = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");
    return n;
}

int main1(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_quit("usage: a.out <IPaddress>");

    //创建一个网络 字节流
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof (servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1330); /* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (SA *) & servaddr, sizeof (servaddr)) < 0)
        err_sys("connect error");

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; /* null terminate */
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error");
    }
    if (n < 0)
        err_sys("read error");

    exit(0);
}