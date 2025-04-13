#include <Myhead.h>

int sendfd(int sockfd, int fdtosend, int exitFlag) // 通过哪个管道去发，发的是哪个文件的对象
{
    struct msghdr hdr;
    bzero(&hdr, sizeof(hdr)); // name-> NULL namelen -> 0

    // 无论正文部分在正文中是否有用，必须有正文
    // char buf[] = "hello"; // 正文数据
    struct iovec vec[1]; // 数组记录离散区域
    // vec[0].iov_base = buf; // 第一个离散碎片首地址
    vec[0].iov_base = &exitFlag;
    vec[0].iov_len = sizeof(int);
    hdr.msg_iov = vec; // 将离散区域的信息放入hdr中
    hdr.msg_iovlen = 1;

    // 控制字段部分，记录要发送的内核数据结构
    struct cmsghdr* pcmsg = (struct cmsghdr*) malloc( // data:fd--> int
        CMSG_LEN(sizeof(int)));

    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS; // 发送文件描述符

    // 通过pcmsg得到data首地址，强转成int*，再解引用。
    *(int*) CMSG_DATA(pcmsg) = fdtosend;
    hdr.msg_control = pcmsg; // 将控制字段放入hdr中
    hdr.msg_controllen = CMSG_LEN(sizeof(int));

    int ret = sendmsg(sockfd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");

    return 0;
}

/*
  接收与发送的区别：
    接收方不知道buf和pcmsg->data的内容
 */
int recvfd(int sockfd, int* pfdtorecv, int* pexitFlag) // 通过哪个管道去发，发的是哪个文件的对象
{
    struct msghdr hdr;
    bzero(&hdr, sizeof(hdr)); // name-> NULL namelen -> 0

    // 无论正文部分在正文中是否有用，必须有正文
    char buf[6] = {0};           // 正文数据
    struct iovec vec[1];         // 数组记录离散区域
    vec[0].iov_base = pexitFlag; // 第一个离散碎片首地址
    vec[0].iov_len = sizeof(int);
    hdr.msg_iov = vec; // 将离散区域的信息放入hdr中
    hdr.msg_iovlen = 1;

    // 控制字段部分，记录要发送的内核数据结构
    struct cmsghdr* pcmsg = (struct cmsghdr*) malloc( // data:fd--> int
        CMSG_LEN(sizeof(int)));

    pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
    pcmsg->cmsg_level = SOL_SOCKET;
    pcmsg->cmsg_type = SCM_RIGHTS; // 发送文件描述符

    hdr.msg_control = pcmsg; // 将控制字段放入hdr中
    hdr.msg_controllen = CMSG_LEN(sizeof(int));

    int ret = recvmsg(sockfd, &hdr, 0);
    ERROR_CHECK(ret, -1, "sendmsg");

    printf("buf = %s,fd = %d\n", buf, *(int*) CMSG_DATA(pcmsg));

    *pfdtorecv = *(int*) CMSG_DATA(pcmsg);

    return 0;
}
