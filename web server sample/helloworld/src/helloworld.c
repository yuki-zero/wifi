#include <stdio.h>   // 用于 printf, stdin, fread
#include <stdlib.h>  // 用于 getenv, atoi, malloc
#include <stddef.h>  // 用于 NULL

int main()
{
    char *method;
    char *query_string;
    char *content_length_str;
    int content_length;
    char *post_data;

    //从环境变量中获取请求方法(GET或POST)
    method = getenv("REQUEST_METHOD");

    //从环境变量中获取查询字符串
    query_string = getenv("QUERY_STRING");

    //从环境变量中获取Content-Length头部字段的值
    content_length_str = getenv("CONTENT_LENGTH");
    if (content_length_str != NULL) {
        content_length = atoi(content_length_str);
        post_data = (char *)malloc(content_length + 1);
        fread(post_data, 1, content_length, stdin);
        post_data[content_length] = '\0';
    }

    //生成响应
    printf("Content-type: text/html\r\n\r\n");
    printf("<html><head><title>CGI Test</title></head><body>");
    printf("<h1>CGI Test</h1>");
    printf("<p>Method: %s</p>", method);
    printf("<p>Query String: %s</p>", query_string);
    printf("<p>Post Data: %s</p>", post_data);
    printf("</body></html>");

    return 0;
}

