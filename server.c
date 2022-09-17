#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.

void send_file(FILE *fp, int sockfd){
    int n;
    char data[80] = {0};

    while(fgets(data, 80, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, 80);
    }
}

void func(int connfd, int num_of_rows)
{
    char buff[MAX];
    time_t start, end;

    FILE *fres;
    fres = fopen("/Users/19231279/CLionProjects/untitled/res.txt","w+");
    if(!fres) {
        printf("Error open matrix.txt\n");
        return;
    }

    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));

        if (strcmp(buff, "") != 0) {

            if (strcmp(buff, "exit") == 0) {
                end = clock();
                float diff = ((float)(end - start) / 1000000.0F ) * 1000;
                fprintf(fres, "Время выполнения %f",diff);
                read(connfd, buff, sizeof(buff));
                printf("Буффер %s", buff);
                fprintf(fres, "Размер буффера %s",buff);
                fclose(fres);
            } else {
                fprintf(fres, "%s \n", buff);
            }
        }

        if ((strncmp(buff, "start", 5)) == 0) {
            FILE *f;

            f = fopen("/Users/19231279/CLionProjects/untitled/matrix.txt","r");
            if(!f) {
                printf("Error open matrix.txt\n");
                continue;
            }

            printf("entered \n");
            start = clock();
            
            write(connfd, "start", sizeof("start"));

            char resBuf[80];
            sprintf(resBuf, "%d\n", num_of_rows);
            printf("%s\n", resBuf);
            write(connfd, resBuf, sizeof(resBuf));
            send_file(f, connfd);
            write(connfd, "end", sizeof("end"));
            printf("\n exit \n");
            continue;
        }

        bzero(buff, MAX);

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
{
    FILE *f;
    f = fopen("/Users/19231279/CLionProjects/untitled/matrix.txt","w+b");
    if(!f) {
        printf("Error open matrix.txt\n");
        return -1;
    }

    printf("%lu",
           CLOCKS_PER_SEC);

    int dataInMb;
    scanf("%d", &dataInMb);
    int res = (dataInMb * 1024) / sqrt(3*dataInMb);

    for (int i=0; i<res;i++) {
        for (int j=0; j<res;j++) {
            if(j == res - 1) {
                fprintf(f, "%d",rand()%100);
            } else {
                fprintf(f, "%d ",rand()%100);
            }

        }
        fprintf(f, "\n");
    }

    fclose(f);

    int sockfd, connfd;
    socklen_t len;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for chatting between client and server
    func(connfd, res);

    // After chatting close the socket
    close(sockfd);
}
