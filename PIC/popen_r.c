#include <stdio.h>

#include "../Myhead.h"

int main()
{
    FILE* fp = popen("./print", "r");
    ERROR_CHECK(fp, NULL, "popen error!\n");
    char buf[128] = {0};
    fread(buf, 1, sizeof(buf), fp);
    printf("[read from pipe: %s!]\n", buf);
    pclose(fp);
    return 0;
}