#include <Myhead.h>

typedef struct

    int
    main(int argc, char* argv[])
{
    // ./2_A 1.pipe file
    ARGS_CHECK(argc, 3);
    int fdw_pipe = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw_pipe, -1, "open:");

    // 发送文件名
    write(fdw_pipe, argv[2], strlen(argv[2]));

    int fdr_file = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr_file, -1, "open");
    char buf[4096] = {0};
    ssize_t sret = read(fdr_file, buf, sizeof(buf));
    write(fdw_pipe, buf, sret);

    close(fdr_file);
    close(fdw_pipe);
    return 0;
}
