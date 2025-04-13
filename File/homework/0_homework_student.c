#include <Myhead.h>

typedef struct student_s
{
    int num;

    // 使用char
    // name[30](字符数组)，在栈上申请内存，申请了30字节空间（存储到磁盘能够很好存储）
    char name[30];
    // char *name;
    // 在堆上进行内存申请，当student
    // stu时，并没有申请内存。需要自己申请内存和初始化。（很难存储进磁盘）

    float score;
} student_t; // 避免与变量名冲突，提高可读性

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    student_t stu[3] = {{1, "RZC", 97.2}, {2, "YH", 98.2}, {3, "XXXS", 99.9}};

    write(fd, stu, sizeof(stu)); // 文件中是二进制数据
    lseek(fd, 0, SEEK_SET);      // 这个可不能忽略

    student_t arr[3] = {0};
    read(fd, arr, sizeof(stu));
    for (size_t i = 0; i < 3; i++)
    {
        printf("num = %d,name = %s,score = %.1f\n", arr[i].num, arr[i].name, arr[i].score);
    }

    return 0;
}
