#include <stdio.h>
// 2.0
typedef struct train_s
{
    int length;      // 火车头 |---> 如果是8字节对齐则改long
    char data[1000]; // 火车车厢，// 可能存在对齐问题
                     // 1000 不是火车车厢的实际大小，是上限。
                     // char 数组不是表示一个字符串，他可以保存任何数据  // char占用1一个字节，方便调节实际大小
} train_t;

int main()
{
    train_t t;
    printf("%ld\n", sizeof(train_t)); // 1004
    return 0;
}