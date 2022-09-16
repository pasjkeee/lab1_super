#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX];
    char mybyf[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string from client: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, MAX);
        printf("%s --- \n\n", buff);
        int sum = 0;
        int count = 0;
        if ((strncmp(buff, "start", sizeof("start"))) == 0) {
            printf("Start from server reciever \n");
            char *val = malloc(sizeof(buff));
            while ((strncmp(buff, "end", sizeof("end"))) != 0) {
                read(sockfd, buff, MAX);
                printf("%s \n", buff);
                for (int i = 0; i < strlen(buff); i++) {

                    if(buff[i] == 'e') {
                        sum += atoi(val);
                        count++;
                        memset(val, 0, sizeof(buff));
                        printf("%d \n",  sum / count);
                        return;
                    }
                    if (buff[i] == ' ') {
                        sum += atoi(val);
                        count++;
                        memset(val, 0, sizeof(buff));
                    } else if (buff[i] == '\n') {
                        sum += atoi(val);
                        count++;
                        memset(val, 0, sizeof(buff));
                        printf("%d \n", sum / count);
                        sum = 0;
                        count = 0;
                    } else if (buff[i] != '\t'){
                        sprintf(val, "%s%c", val, buff[i]);
                    }
                }
            }
            continue;
        }

        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

//сформировать результирующий вектор как среднее по каждой строке исходной квадратной матрицы
int main() {
    FILE *f;
    f = fopen("/Users/19231279/CLionProjects/untitled/matrix.txt","r");
    if(!f) {
        printf("Error open matrix.txt\n");
        return -1;
    }


    ssize_t read;
    char * line = NULL;
    size_t len = 0;

//    while ((read = getline(&line, &len, f)) != -1) {
//        char *val = malloc(len);
//        for (int i = 0; i < strlen(line); i++) {
//            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\r' ) {
//                sum += atoi(val);
//                count++;
//                memset(val, 0, len);
//
//                continue;
//            }
//
//            sprintf(val, "%s%c", val, line[i]);
//        }
//        sum += atoi(val);
//        count++;
//        memset(val, 0, len);
//
//        printf("%d \n", sum / count);
//
//        sum =0;
//        count=0;
//
//    }

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
