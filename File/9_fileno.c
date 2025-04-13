#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    FILE* fp = fopen(argv[1], "r+");
    ERROR_CHECK(fp, NULL, "fopen");
    size_t sret = write(fp->_fileno, "hello", 5);
    printf("fileno(fp) = %d sret = %ld\n", fp->_fileno, sret);
    // printf("fp->_fileno = %d sret = %ld\n",fp->_fileno,sret);

    fclose(fp);
    return 0;
}
