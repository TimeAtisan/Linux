#include <Myhead.h>

int main(int argc, char* argv[])
{
    short port = 0x1234;
    short big_endian_port = htons(port);
    printf("big_endian_port = %x\n", big_endian_port);
    return 0;
}
