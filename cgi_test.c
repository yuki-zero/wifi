#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *pQuery = NULL;
	char *pMethod = NULL;
	char output_buff[1024]; // 确保缓冲区足够大来存储输出内容
	int len = 0; // 添加变量len的定义


	//从环境变量中获取请求方法GET/POST
	pMethod = getenv("REQUEST_METHOD");

	//从环境变量中获取查询字符串，看是什么请求

	//GET请求
	if(!strcmp(pMethod,"GET"))
	{
		pQuery = getenv("QUERY_STRING");
	}

	//POST请求
	if(!strcmp(pMethod,"POST"))
	{
		len = atoi(getenv("CONTENT_LENGTH")); //获取数据长度
		fgets(pQuery,len + 1,stdin); //从stdin中读取n个字节的数据
	}

	//输出数据
	sprintf(output_buff, "Content-Type: application/json\r\n\r\n" \
                                 "{" \
                                 "\r\"status\":0," \
                                 "\r\"msg\":username or password incorrect" \
                                 "\r}");
	websWrite(stdout, T("%s"), output_buff);
}