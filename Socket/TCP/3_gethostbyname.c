#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    struct hostent* entry = gethostbyname(argv[1]);
    // 如果域名不存在则会报错，网络上的错误，不会改ERRNO
    if (NULL == entry)
    {
        herror("gethostbyname");
        return -1;
    }
    printf("office name = %s\n", entry->h_name);
    for (int i = 0; entry->h_aliases[i] != NULL; i++)
    {
        printf("\t alias = %s\n", entry->h_aliases[i]);
    }

    for (size_t i = 0; entry->h_addr_list[i] != NULL; i++)
    {
        char ip[1024] = {0};
        inet_ntop(entry->h_addrtype, entry->h_addr_list[i], ip, 1024);
        printf("ip = %s\n", ip);
    }

    return 0;
}
