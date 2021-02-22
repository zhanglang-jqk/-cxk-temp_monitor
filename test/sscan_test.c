#include "stdio.h"

int main()
{
    // char *str = ",0\r\nAT+QIRD=0,1500\r\r\n+QIRD: 1\r\n\r\nOK";
    // char t = 0;
    // sscanf(str, "%*[^A]AT+QIRD=0,1500\r\r\n+QIRD: %d\r\n\r\nOK", &t);
    // printf("t=%d\r\n", (int)t);

    /*6.获取指定字符中间的字符串*/
    // char * string  = "ios<Android>wp7";
    // char buf1[10] = {0};
    // sscanf(string, "%*[^<]<%[^>]", buf1);
    // printf("6.string=%s\n", string);
    // printf("6.buf1=%s\n\n", buf1);
    /*************************************
    6.string=ios<Android>wp7
    6.buf1=Android
    ***************************************/

    char *str = R"({
        "product_key":"a1vhKQ19mpk",
        "device_name":"CkCuGNUkYm6sWVsbGQl5",
        "device_secret","ac8e301a20d0ce5f39c06166a3862435",
        "collect_interval":500,
        "report_count":10,
    })";
    printf("%s", str);
}