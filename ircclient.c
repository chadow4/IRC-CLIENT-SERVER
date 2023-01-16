#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define NTP_TIMESTAMP_DELTA 2208988800ull

void stop(char *s) {
    perror(s);
    exit(1);
}

void remove_end_line(char *string, int max_length) {
    for (int i = 0; string[i] != '\0' && i < max_length; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
        }
    }
}

void red() {
    printf("\033[1;31m");
}

void yellow() {
    printf("\033[1;33m");
}

void reset() {
    printf("\033[0m");
}

int main(int argc, char *argv[]) {

    int sockfd;
    struct sockaddr_in cli_addr;
    fd_set read_fds;
    char buffer[1024];


    // Vérifier les arguments de la ligne de commande
    if (argc < 3) {
        fprintf(stderr, "Usage: %s adresse_ip port\n", argv[0]);
        return 1;
    }

    // Récupérer l'adresse IP et le numéro de port du serveur
    const char *ip_address = argv[1];
    uint16_t port = (uint16_t) atoi(argv[2]);

    // Demander un pseudonyme
    printf("\nEntrez votre pseudonyme: ");
    char nickname[32];
    scanf("%s", nickname);
    getchar();
    remove_end_line(nickname, 32);
    // Créer la socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        stop("Erreur lors de la création de la socket");

    }

    // Configurer l'adresse et le port du serveur

    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(port);
    int result = inet_pton(AF_INET, ip_address, &cli_addr.sin_addr);
    if (result <= 0) {
        stop("Erreur lors de la conversion de l'adresse IP");
    }

    // Connecter à la socket du serveur
    result = connect(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
    if (result < 0) {
        stop("Erreur lors de la connexion au serveur ");
    }

    // Envoyer le pseudonyme au serveur
    result = send(sockfd, nickname, strnlen(nickname, 32), 0);
    if (result < 0) {
        stop("Errreur lors de l'envoi du pseudonyme");
    }

    // Initialiser les descripteurs de fichiers à surveiller
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    FD_SET(0, &read_fds); // 0 correspond à stdin

    // Boucle de chat
    for (;;) {
        // Sélectionner les descripteurs de fichiers prêts à être lus
        fd_set rfds = read_fds;
        result = select(sockfd + 1, &rfds, NULL, NULL, NULL);
        if (result < 0) {
            stop("Erreur lors de la selection des descripteurs de fichiers");
        }
        // Vérifier si un message a été reçu en provenance du serveur
        if (FD_ISSET(sockfd, &rfds)) {
            // Recevoir le message
            memset(buffer, '\0', 1024);
            result = recv(sockfd, buffer, 1024, 0);
            if (result < 0) {
                stop("Erreur lors de la réception d'un message");
            } else if (result == 0) {
                // Le serveur a fermé la connexion
                printf("Server connection closed\n");
                break;
            } else {
                // Afficher le message avec l'heure de réception
                time_t timestamp = time(NULL);
                struct tm *timeinfo = localtime(&timestamp);
                char time_string[32];
                strftime(time_string, sizeof(time_string), "%F %T", timeinfo);
                if (buffer[0] == '/') {
                    char *tbuf = strtok(buffer, " ");
                    if (tbuf != NULL && strcmp(tbuf, "/alert")) {
                        tbuf = strtok(NULL, "\0");
                        red();
                        printf("[%s] %s", time_string, tbuf);
                        reset();
                        printf("\n");
                    }
                } else {
                    printf("[%s] %s\n", time_string, buffer);
                }
            }
        }
        // Vérifier si un message a été entré par l'utilisateur
        if (FD_ISSET(0, &rfds)) {
            // Lire le message entré par l'utilisateur
            memset(buffer, '\0', 1024);
            if (fgets(buffer, 1024, stdin)) {
                remove_end_line(buffer, 1024);
                result = send(sockfd, buffer, strnlen(buffer, 1024) + 1, 0);
                if (result < 0) {
                    stop("Erreur lors de l'envoi d'un message");
                }
            }
        }
    }
    close(sockfd);
    return 0;
}