#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>


#define NTP_TIMESTAMP_DELTA 2208988800ull

/* Stop Function */
void stop(char *s) {
    perror(s);
    exit(1);
}

/* Remove EndLine Function */
void remove_end_line(char *string, int max_length) {
    for (int i = 0; string[i] != '\0' && i < max_length; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
    }
}

/* Color Functions */
void red() {
    printf("\033[1;31m");
}

void yellow() {
    printf("\033[1;33m");
}

void reset() {
    printf("\033[0m");
}

/* fileExist Function */
int fileExist(char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    int sockfd;
    struct sockaddr_in cli_addr;
    fd_set read_fds;
    char buffer[1024];
    // get the time for chat receive
    time_t timestamp = time(NULL);
    struct tm *timeinfo = localtime(&timestamp);
    char time_string[32];
    strftime(time_string, sizeof(time_string), "%F %T", timeinfo);

    // check number of arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s adresse_ip port\n", argv[0]);
        return 1;
    }

    // get IP and PORT
    const char *ip_address = argv[1];
    uint16_t port = (uint16_t) atoi(argv[2]);

    // get pseudo
    printf("\nEnter your pseudo: ");
    char nickname[32];
    scanf("%s", nickname);
    getchar();
    remove_end_line(nickname, 32);

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        stop("Error : creation of socket");

    }

    // Config IP and PORT
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(port);
    int result = inet_pton(AF_INET, ip_address, &cli_addr.sin_addr);
    if (result <= 0) {
        stop("Error : convert IP adress");
    }

    // Connect socket to the Server
    result = connect(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
    if (result < 0) {
        stop("Error : connect to the server");
    }

    // Send pseudo to the Server
    result = send(sockfd, nickname, strnlen(nickname, 32), 0);
    if (result < 0) {
        stop("Error send the pseudo to the server");
    }
    // Initialize the file descriptor to be monitored
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    FD_SET(0, &read_fds); // 0 correspond to stdin

    // Chat Loop
    for (;;) {
        // Select file descriptors ready to be read
        fd_set rfds = read_fds;
        result = select(sockfd + 1, &rfds, NULL, NULL, NULL);
        if (result < 0) {
            stop("Error: select descriptor files");
        }
        // Check if a message has been received from the server
        if (FD_ISSET(sockfd, &rfds)) {
            // Receive the message
            memset(buffer, '\0', 1024);
            result = recv(sockfd, buffer, 1024, 0);
            if (result < 0) {
                stop("Error : receiv message");
            } else if (result == 0) {
                // The server has closed the connection
                printf("Server connection closed\n");
                break;
            } else {
                // Display the message with the time of reception
                if (buffer[0] == '/') {
                    char *tbuf = strtok(buffer, " ");
                    // check if the message is an alert
                    if (tbuf != NULL && !strcmp(tbuf, "/alerte")) {
                        tbuf = strtok(NULL, "\0");
                        red();
                        printf("[%s] %s", time_string, tbuf);
                        reset();
                        printf("\n");
                    }
                    // check if the message is a send file
                    if(tbuf != NULL && !strcmp(tbuf,"/send")){
                        char receivePathFile[64] = {0};
                        char receiveFileContain[256] = {0};
                        char finalPath[64] = {0};
                        tbuf = strtok(NULL," ");
                        if (tbuf != NULL && strcmp(tbuf, "") != 0) {
                            strncpy(receivePathFile, tbuf, 64);
                            tbuf = strtok(NULL,"\n");
                            if (tbuf != NULL && strcmp(tbuf, "") != 0) {
                                strncpy(receiveFileContain, tbuf, 64);
                            }
                            char * delimReceivPathFile = strtok(receivePathFile,"/");
                            if(delimReceivPathFile != NULL && strcmp(delimReceivPathFile,"") != 0){
                                delimReceivPathFile = strtok(NULL,"\n");
                                if(delimReceivPathFile != NULL && strcmp(delimReceivPathFile,"") != 0) {
                                    printf("[%s] you have receveid file : %s\n", time_string, delimReceivPathFile);
                                    snprintf(finalPath,64,"receivedFiles/%s",delimReceivPathFile);
                                    // creating file received
                                    FILE * fp = fopen(finalPath,"w");
                                    fprintf(fp,"%s", receiveFileContain);
                                     fclose(fp);
                                }
                            }
                        }
                    }
                } else {
                    printf("[%s] %s\n", time_string, buffer);
                }
            }
        }
        // Vérifier si un message a été entré par l'utilisateur
        if (FD_ISSET(0, &rfds)) {
            // Check if a message has been entered by the user
            memset(buffer, '\0', 1024);
            if (fgets(buffer, 1024, stdin)) {
                char *tempbuff = calloc(sizeof(char), 1024);
                strncpy(tempbuff, buffer, 1024);
                char *tbufsend = strtok(tempbuff, " ");
                // Check if message is prefixed by /send
                if (tbufsend != NULL && !strcmp(tbufsend, "/send")) {

                    char *nicknameReceiver = calloc(sizeof(char), 32);
                    char *fileContainer = calloc(sizeof(char), 256);
                    char *pathFile = calloc(sizeof(char), 32);

                    tbufsend = strtok(NULL, " ");
                    // get nicknameReceiver
                    if (tbufsend != NULL && tbufsend[0] != '\0') {
                        strncpy(nicknameReceiver, tbufsend, 32);
                    } else {
                        printf("[%s] No user specified for sending file\n", time_string);
                        continue;
                    }
                    // get pathFile
                    tbufsend = strtok(NULL, " ");
                    if (tbufsend != NULL && tbufsend[0] != '\0') {
                        strncpy(pathFile, tbufsend, 32);
                        remove_end_line(pathFile, 32);
                    } else {
                        printf("[%s] No path specified for sending file\n", time_string);
                        continue;
                    }
                    // check if File exist
                    memset(tempbuff, '\0', 1024);
                    if (fileExist(pathFile) == 1) {
                        printf("[%s] File doesn't exist\n", time_string);
                    }
                    // open File
                    FILE *fp = fopen(pathFile, "rb");
                    if (fp == NULL) {
                        printf("[%s] Error opening File\n", time_string);
                        continue;
                    }
                    // read contain of file
                    fread(fileContainer, 1, 512, fp);
                    fclose(fp);
                    snprintf(tempbuff, 1024, "/send %s %s %s", nicknameReceiver, pathFile, fileContainer);
                    remove_end_line(tempbuff, 1024);
                    result = send(sockfd, tempbuff, 1024, 0);
                    if (result < 0) {
                        stop("Error: sending message");
                    }

                } else {
                    remove_end_line(buffer, 1024);
                    result = send(sockfd, buffer, 1024, 0);
                    if (result < 0) {
                        stop("Error: sending message");
                    }
                }
            }
        }
    }
    close(sockfd);
    return 0;
}