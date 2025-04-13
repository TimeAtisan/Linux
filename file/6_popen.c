#include <Myhead.h>

int main(int argc, char* argv[])
{
    FILE* fp = popen("ls -l", "r");
    char buf[4096] = {0};
    while (1)
    {
        bzero(buf, sizeof(buf));
        int ret = read(fileno(fp), buf, sizeof(buf));
        ERROR_CHECK(ret, -1, "read");

        if (0 == ret)
        {
            break;
        }

        printf("%s", buf);
    }
    pclose(fp);
    return 0;
}
