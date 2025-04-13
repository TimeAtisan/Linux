#include <Myhead.h>

int main()
{
    int ret = alarm(3);
    printf("first ret = %d\n", ret);
    sleep(2);
    ret = alarm(2);
    printf("second ret = %d\n", ret);
    return 0;
}