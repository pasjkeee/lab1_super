#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string from client: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, MAX);
        printf("%s --- \n\n", buff);

        if ((strncmp(buff, "start", sizeof("start"))) == 0) {
            int submit = 0;
            float res_time = 0;
            int** arr;
            int* res_arr;
            int res_buf = 0;
            int index_row = 0;
            int index_col = 0;

            printf("Start from server reciever \n");
            char *val = malloc(sizeof(buff));
            while ((strncmp(buff, "end", sizeof("end"))) != 0) {
                read(sockfd, buff, MAX);
                for (int i = 0; i < strlen(buff); i++) {
                    if(buff[i] == 'e') {
                        memset(val, 0, sizeof(buff));
                        char buf[MAX];

                        time_t start = clock();
                        printf("Start");
                        for(int ii = 0; ii < res_buf; ii++) {
                            int rowsum = 0;
                            for(int jj =0; jj < res_buf; jj++) {
                                rowsum += arr[ii][jj];
                                submit++;
                            }
                            res_arr[ii] = rowsum / res_buf;
                        }
                        time_t end = clock();
                        res_time = res_time + ((float)(end - start) / 1000000.0F ) * 1000;
                        printf("Stop");

                        for (int i = 0; i < res_buf; i++) {
                            sprintf(buf, "%d \n", res_arr[i]);
                            write(sockfd, buf, sizeof(buf));
                        }

                        sprintf(buf, "%d", submit / 1024);
                        write(sockfd, buf, sizeof(buf));
                        sprintf(buf, "%f", res_time);
                        write(sockfd, buf, sizeof(buf));
                        write(sockfd, "exit", sizeof("exit"));
                        printf("Результирующий буффер размер %d \n", res_buf);

                        printf("%d %d %d %d", arr[0][0], arr[1][1],arr[2][2],arr[3][3]);
                        return;
                    }

                    if (buff[i] == ' ') {
                        arr[index_row][index_col] = atoi(val);
                        memset(val, 0, sizeof(buff));
                        index_col++;
                    } else if (buff[i] == '\n') {

                        if (res_buf == 0) {
                            res_buf = atoi(val);
                            printf("результ буффер %d \n", res_buf);
                            res_arr = (int*) malloc(res_buf * sizeof(int));
                            arr = (int**) malloc(res_buf * sizeof(int*));
                            for (int j = 0; j < res_buf; j++) {
                                arr[j] = (int*) malloc(res_buf * sizeof(int));
                            }
                            memset(val, 0, sizeof(buff));
                            continue;
                        }

                        arr[index_row][index_col] = atoi(val);

                        memset(val, 0, sizeof(buff));
                        index_row++;
                        index_col = 0;
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

    int sockfd;
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
