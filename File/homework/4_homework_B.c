#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr_pipe = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr_pipe, -1, "open");

    char filename[4096] = {0};
    read(fdr_pipe, filename, sizeof(filename));

    char path[8192] = {0};
    char dir[] = "dir1/"; // Linux系统会把多余的 / 合并成一个 /
    mkdir(dir, 0777);
    sprintf(path, "%s%s%s", dir, "/", filename);

    int fdw_file = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);

    char buf[4096] = {0};

    ssize_t sret = read(fdr_pipe, buf, sizeof(buf));
    write(fdw_file, buf, sret);

    close(fdw_file);
    close(fdr_pipe);

    return 0;
}
