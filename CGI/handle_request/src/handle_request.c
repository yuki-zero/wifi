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
}
