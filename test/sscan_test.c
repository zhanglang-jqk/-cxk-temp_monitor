#include "stdio.h"

int main()
{
    char *str = ",0\r\nAT+QIRD=0,1500\r\r\n+QIRD: 1\r\n\r\nOK";
    char t = 0;
    sscanf(str, "%*[^A]AT+QIRD=0,1500\r\r\n+QIRD: %d\r\n\r\nOK", &t);
    printf("t=%d\r\n", (int)t);

    /*6.��ȡָ���ַ��м���ַ���*/
    // char * string  = "ios<Android>wp7";
    // char buf1[10] = {0};
    // sscanf(string, "%*[^<]<%[^>]", buf1);
    // printf("6.string=%s\n", string);
    // printf("6.buf1=%s\n\n", buf1);
    /*************************************
    6.string=ios<Android>wp7
    6.buf1=Android
    ***************************************/
}