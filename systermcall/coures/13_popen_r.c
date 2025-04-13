#include "../Myhead.h"

int main(int argc, char* argv[])
{
    FILE* fp = popen("./print", "r");
    ERROR_CHECK(fp, NULL, "popen error!\n");
    char buf[1024];
    fread(buf, 1, sizeof(buf), fp);
    printf("%s\n", buf);
    return 0;
}