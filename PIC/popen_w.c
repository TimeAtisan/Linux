#include "../Myhead.h"

int main()
{
    FILE* fp = popen("./aaa", "w");
    ERROR_CHECK(fp, NULL, "popen error!\n");
    fwrite("10  20", 1, 6, fp);
    pclose(fp);
    return 0;
}