#include <Myhead.h>

typedef struct student_s
{
    int id;
    char name[30];
    float socre;
} student_t;

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int fd1 = open(argv[1], O_RDWR);
    int fd2 = open(argv[2], O_RDWR);
    ERROR_CHECK(fd1, -1, "open");
    ERROR_CHECK(fd2, -1, "open");

    student_t stu[3] = {{1, "ZS", 92.3}, {2, "LS", 98.2}, {3, "WW", 94.6}};

    write(fd1, stu, sizeof(stu));
    /* lseek(fd1, 0, SEEK_SET);
    lseek(fd2, 0, SEEK_SET); */
    char buf1[4096];
    char buf2[4096];

    while (1)
    {
        bzero(buf1, sizeof(buf1));
        bzero(buf2, sizeof(buf2));
        int sret1 = read(fd1, buf1, sizeof(buf1));
        printf("sret1 = %d\n", sret1);
        int sret2 = read(fd2, buf2, sizeof(buf2));
        printf("sret2 = %d\n", sret2);

        if (sret1 != sret2)
        {
            printf("not same!\n");
            break;
        }
        else if (memcmp(buf1, buf2, sret1) != 0)
        {
            printf("not same!\n");
            break;
        }
        else if (sret1 == 0)
        {
            printf("is same!");
            break;
        }
    }
    close(fd1);
    close(fd2);

    return 0;
}
