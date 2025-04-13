#include <Myhead.h>
#include <mysql/mysql.h>

int main(int argc, char* argv[])
{
    MYSQL* mysql = mysql_init(NULL);

    MYSQL* cret = mysql_real_connect(mysql, "localhost", "root", "NT001AAA", "test54", 0, NULL, 0);

    if (cret == NULL)
    {
        fprintf(stderr, "mysql_real_connect:%s\n", mysql_error(cret));
        return -1;
    }

    char sql[4096] = "select * from pet;";

    int qret = mysql_query(mysql, sql);
    if (qret != 0)
    {
        fprintf(stderr, "mysql query:%s\n", mysql_error(mysql));
    }

    MYSQL_RES* res = mysql_store_result(mysql);
    printf("total row = %lu,total col = %d\n", mysql_num_rows(res), mysql_num_fields(res));

    MYSQL_ROW row; // 是某一行 字符串数组
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        for (size_t i = 0; i < mysql_num_fields(res); i++)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }

    mysql_close(mysql);
    return 0;
}
