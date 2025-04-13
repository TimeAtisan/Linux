#include <Myhead.h>
// 如何判断小端大段
int main(int argc, char* argv[])
{
    int i = 0x12345678;
    char* p = (char*) &i;
    printf("%x\n", *p);
    return 0;
}