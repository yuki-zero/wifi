#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 4096

//根据文件扩展名返回 MIME 类型
const char *get_mime_type(const char *path) {
    if (strstr(path, ".html") || strstr(path, ".htm")) return "text/html";
    if (strstr(path, ".jpg") || strstr(path, ".jpeg")) return "image/jpeg";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".gif")) return "image/gif";
    return "application/octet-stream";
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    char *method, *path;
    int read_size;

    // Read request from client
    read_size = read(client_socket, buffer, sizeof(buffer) - 1);
    if (read_size < 0) {
        perror("Failed to read from client");
        close(client_socket);
        return;
    }

    buffer[read_size] = '\0';

    // Parse the HTTP request (basic parsing for GET method)
    method = strtok(buffer, " "); //将buffer中的字符以空格为界分割，并将第一部分（即请求方法）赋给method
    path = strtok(NULL, " "); //NULL 作为第一个参数，这告诉 strtok 继续从上次分割的位置开始搜索 XXX.html
    
    if (method == NULL || path == NULL || strcmp(method, "GET") != 0) {
        // Only handle GET requests
        close(client_socket);
        return;
    }

    //如果是根路径，默认是index.htnl页面
    // Default to index.html if path is '/'
    if (strcmp(path, "/") == 0) {
        path = "/index.html";
    }

    // Construct file path
    //格式化，将path前面加个 . 放入file_path中 eg. /index.html -> ./index.html
    char file_path[256];
    snprintf(file_path, sizeof(file_path), ".%s", path);

    // Open file
    int file = open(file_path, O_RDONLY);
    if (file < 0) {
        // File not found
        const char *response = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<!DOCTYPE html>"
            "<html lang=\"en\">"
            "<head>"
            "<meta charset=\"UTF-8\">"
            "<title>404 Not Found</title>"
            "</head>"
            "<body>"
            "<h1>404 Not Found</h1>"
            "<p>The requested resource was not found on this server.</p>"
            "</body>"
            "</html>";
        write(client_socket, response, strlen(response));
    } else {
        // File found, send response
        struct stat file_stat; //存储文件状态的结构体
        fstat(file, &file_stat); //获取file的状态，存入file_stat中
        const char *mime_type = get_mime_type(file_path); //获取文件类型

        // Send HTTP header
        char header[256];
        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: close\r\n"
                 "\r\n",
                 mime_type, file_stat.st_size);
        write(client_socket, header, strlen(header));

        // Send file content
        off_t offset = 0;
        ssize_t bytes_read;
        //循环读取file中所有数据
        while ((bytes_read = pread(file, buffer, sizeof(buffer), offset)) > 0) { //pread从offset偏移量读取数据
            write(client_socket, buffer, bytes_read);
            offset += bytes_read;
        }

        close(file);
    }

    // Close the connection
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    //创建socket AF_INET IPv4地址 SOCK_STREAM TCP 0表示默认协议
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //接收来自任何网络接口的IP地址
    server_addr.sin_port = htons(PORT); //将端口号 PORT 转换为网络字节序

    //绑定特定的IP地址和端口
    // Bind the socket to the address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    //监听 5表示待连接请求队列的最大长度
    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        // Handle the client connection
        handle_client(client_socket);
    }

    // Close the server socket
    close(server_socket);
    return 0;
}

