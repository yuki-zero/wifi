/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>


#define MAX_QUERY_LENGTH 1024
const char *json_get_string_value_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *string_json = NULL;

    json_object_object_get_ex(json, p_field, &string_json);
    if (NULL == string_json)
    {
        printf("json_object_object_get error %s", p_field);
        return NULL;
    }

    if (json_type_string == json_object_get_type(string_json))
    {
        return json_object_get_string(string_json);
    }

    return NULL;
}

int json_get_int_value_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *int_json = NULL;

    json_object_object_get_ex(json, p_field, &int_json);
    if (NULL == int_json)
    {
        printf("json_object_object_get error %s", p_field);
        return -1;
    }

    if (json_type_int == json_object_get_type(int_json))
    {
        return (int)json_object_get_int(int_json);
    }

    return -1;
}

const char *json_get_string_value(struct json_object *json)
{
    if (json_type_string == json_object_get_type(json))
    {
        return json_object_get_string(json);
    }

    return NULL;
}

struct json_object *json_get_json_object_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *json_obj = NULL;

    json_object_object_get_ex(json, p_field, &json_obj);
    if (NULL == json_obj)
    {
        printf("json_object_object_get error %s", p_field);
        return NULL;
    }

    return json_obj;
}

int json_is_array(struct json_object *json)
{
    if (json_type_array == json_object_get_type(json))
    {
        return 0;
    }

    return -1;
}


void handle()
{
	char query[2048];
	size_t content_length;
	char  *content_length_str = getenv("CONTENT_LENGTH");

	if(content_length_str == NULL)
	{
		printf("{\"error\":1,\"message\":\"missing COBTENT_LENGTH\"}\n");
	}
	
	content_length = (size_t)atoi(content_length_str);
	if (content_length >= MAX_QUERY_LENGTH) 
	{
		printf("{\"error\":1,\"message\":\"Request too large\"}\n");
		return;
	}
	fread(query, 1, content_length, stdin);
	query[content_length] = '\0'; // Null-terminate
	
	//query = "{"Name":"bai","Id":"123456","Mes":{"age":"20","tel":"123456"}}"
	
	struct json_object *myjson = json_tokener_parse(query);

	const char* name = json_get_string_value_by_field(myjson,"Name");
	const char* id = json_get_string_value_by_field(myjson,"Id");
	if(!strcmp(name,"bai") && !strcmp(id,"123456"))
	{
		printf("{\n"
			"  \"Name\": \"bai\",\n"
			"  \"Id\": \"123456\",\n"
			"  \"Mes\": {\n"
			"    \"age\": \"20\",\n"
			"    \"tel\": \"123456\"\n"
			"  }\n"
			"}\n");
	}
	else
	{
		printf("{\"error\":1,\"message\":\"find  filed\"}\n");
	}
	
	json_object_put(myjson);

}
int main() {
    // Check request method
    const char *method = getenv("REQUEST_METHOD");

    // Print HTTP header
    printf("Content-Type: application/json\n\n");

    if (method != NULL && strcmp(method, "POST") == 0) {
        handle();
    } else {
    } 
    return 0;
}*/


//get dhcp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#define MAX_QUERY_LENGTH 1024
#define MAX_BUFFER 512
//从键值里获取对应的string数据
const char *json_get_string_value_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *string_json = NULL;

    json_object_object_get_ex(json, p_field, &string_json);
    if (NULL == string_json)
    {
        printf("json_object_object_get error %s", p_field);
        return NULL;
    }

    if (json_type_string == json_object_get_type(string_json))
    {
        return json_object_get_string(string_json);
    }

    return NULL;
}

//从键值里获取对应的int数据
int json_get_int_value_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *int_json = NULL;

    json_object_object_get_ex(json, p_field, &int_json);
    if (NULL == int_json)
    {
        printf("json_object_object_get error %s", p_field);
        return -1;
    }

    if (json_type_int == json_object_get_type(int_json))
    {
        return (int)json_object_get_int(int_json);
    }

    return -1;
}

const char *json_get_string_value(struct json_object *json)
{
    if (json_type_string == json_object_get_type(json))
    {
        return json_object_get_string(json);
    }

    return NULL;
}

//从键值里获取对应的json数据
struct json_object *json_get_json_object_by_field(struct json_object *json, const char *p_field)
{
    struct json_object *json_obj = NULL;

    json_object_object_get_ex(json, p_field, &json_obj);
    if (NULL == json_obj)
    {
        printf("json_object_object_get error %s", p_field);
        return NULL;
    }

    return json_obj;
}

int json_is_array(struct json_object *json)
{
    if (json_type_array == json_object_get_type(json))
    {
        return 0;
    }

    return -1;
}

void get_dhcp(const char* command, char* arr, int size)
{
    FILE *fp;
    char buffer[MAX_BUFFER];
    size_t current_size = 0;

    //使用popen打开管道，然后执行command传入的指令
    fp = popen(command, "r");
    if(fp == NULL)
    {
        printf("popen failed\n");
        return;
    }

    //从管道中读取一行内容存入到buffer中
    while(fgets(buffer, sizeof(buffer) - 1, fp) != NULL)
    {
        size_t len = strlen(buffer);
        //检查是否有足够空间
        if(current_size + len < size - 1)
        {
            strcpy(arr + current_size, buffer);
            current_size += len;
        }
    }

    buffer[size] = '\0';

    //关闭管道
    if(pclose(fp) == -1)
    {
        printf("pclose failed\n");
        return;
    }

}

void handle_post()
{
    char query[MAX_QUERY_LENGTH];
    size_t content_length;
    char *content_length_str = getenv("CONTENT_LENGTH");
    struct json_object *response = NULL;

    content_length = (size_t)atoi(content_length_str);
    if(content_length >= MAX_QUERY_LENGTH)
    {
        printf("{\"error\":1,\"message\":\"Request too large\"}\n");
        return;
    }

    fread(query, 1, content_length, stdin);
    query[content_length] = '\0'; // Null-terminate
    struct json_object *myjson = json_tokener_parse(query);
    const char* action = json_get_string_value_by_field(myjson,"ATC");
    if(!strcmp(action,"GetDHCP"))
    {
        char ipaddr[MAX_BUFFER] = {0};
        char netmask[MAX_BUFFER] = {0};
        char start[MAX_BUFFER] = {0};
        char limit[MAX_BUFFER] = {0};
        char leasetime[MAX_BUFFER] = {0};

        //获取信息
        get_dhcp("uci get network.lan.ipaddr", ipaddr, MAX_BUFFER);
        get_dhcp("uci get network.lan.netmask", netmask, MAX_BUFFER);
        get_dhcp("uci get dhcp.lan.start", start, MAX_BUFFER);
        get_dhcp("uci get dhcp.lan.limit", limit, MAX_BUFFER);
        get_dhcp("uci get dhcp.lan.leasetime", leasetime, MAX_BUFFER);

        response = json_object_new_object(); //创建对象

        //把信息转为json格式并添加到json对象中
        json_object_object_add(response, "ipaddr", json_object_new_string(ipaddr));
        json_object_object_add(response, "netmask", json_object_new_string(netmask));
        json_object_object_add(response, "start", json_object_new_string(start));
        json_object_object_add(response, "limit", json_object_new_string(limit));
        json_object_object_add(response, "leasetime", json_object_new_string(leasetime));
        json_object_object_add(response, "error", json_object_new_int(0));
        printf("%s\n",json_object_to_json_string(response));
    }
    else
    {
        printf("{\"error\":1,\"message\":\"find  filed\"}\n");
    }
    if(response != NULL)
    {
        json_object_put(response);
    }
    if(myjson != NULL)
    {
        json_object_put(myjson);
    }
}

int main() {
    // Check request method
    const char *method = getenv("REQUEST_METHOD");

    // Print HTTP header
    printf("Content-Type: application/json\n\n");

    if (method != NULL && strcmp(method, "POST") == 0) {
        handle_post();
    } else {
    }
    return 0;
}

