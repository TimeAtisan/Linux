#include <Myhead.h>

int main(int argc, char* argv[])
{
    FILE* fp = popen("./add", "w");
    ERROR_CHECK(fp, NULL, "popen");
    int a = 0, b = 0;
    scanf("%d %d", &a, &b);
    fprintf(fp, "%d %d", a, b);
    pclose(fp);
    return 0;
}
