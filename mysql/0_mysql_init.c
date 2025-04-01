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

    char sql[4096] = "insert into pet (name,owner,sex) values ('RZC','LGJ','m');";

    // 存在一直SQL注入的漏洞被攻击
    /* char name[]    = "RZC";
    char owner[]   = "LGJ";
    char sex[]     = "m";
    char sql[4096] = {0};
    sprintf(sql, "insert into pet (name,owner,sex) values (%s,%s,%s);", name, owner, sex);
 */
    int qret = mysql_query(mysql, sql);
    if (qret != 0)
    {
        fprintf(stderr, "mysql query:%s\n", mysql_error(mysql));
    }

    MYSQL_RES* mysql_store_result();

    mysql_close(mysql);
    return 0;
}
