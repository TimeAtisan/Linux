#include "../Myhead.h"

int main(int argc, char* argv[])
{
    FILE* fp = popen("./add", "w");
    ERROR_CHECK(fp, NULL, "popen error!\n");
    fwrite("100 200", 1, 7, fp);
    pclose(fp);
    return 0;
}