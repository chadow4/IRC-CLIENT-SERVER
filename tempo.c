// check command
//                    } else if (buffer[0] == '/') {
//                        char *tbuf = strtok(buffer, " ");
//                        char *tbufMp = tbuf;
//                        char *tbufAlert = tbuf;
//                        node_t *checking_node = head;
                        // /nickname command
                        if (tbuf != NULL && !strcmp(tbuf, "/nickname")) {
                            char nickname[32] = {0};
                            tbuf = strtok(NULL, "\n");
                            if (tbuf != NULL && strcmp(tbuf, "") != 0) {
                                strncpy(nickname, tbuf, sizeof(nickname) - 1);
                            }
                            if (strcmp(nickname, "") == 0) {
                                memset(buffer, '\0', 1024);
                                snprintf(buffer, sizeof(buffer),
                                         "You dont have enter pseudo, please retry /nickname new_pseudo :");
                                if (send(current_client->sockfd, buffer, sizeof(buffer), 0) < 0) {
                                    stop("send");
                                }
                            } else {
                                if (check_if_pseudo_exist(checking_node, nickname) == -1) {
                                    memset(buffer, '\0', 1024);
                                    snprintf(buffer, sizeof(buffer),
                                             "Sorry, pseudo is already use, please retry /nickname new_pseudo :");
                                    if (send(current_client->sockfd, buffer, sizeof(buffer), 0) < 0) {
                                        stop("send");
                                    }
                                } else {
                                    strncpy(current_client->pseudo, nickname, sizeof(current_client->pseudo) - 1);
                                    memset(buffer, '\0', 1024);
                                    yellow();
                                    printf("set new pseudo %s for socket %d \n", current_client->pseudo,
                                           current_client->sockfd);
                                    reset();
                                    snprintf(buffer, sizeof(buffer), "your new pseudo is %s", current_client->pseudo);
                                    if (send(current_client->sockfd, buffer, sizeof(buffer), 0) < 0) {
                                        stop("send");
                                    }
                                }
                            }
//                        }
//                        // /date command
//                        if (tbuf != NULL && !strcmp(tbuf, "/date")) {
//                            time_t timestamp = time(NULL);
//                            char time_string[32];
//                            struct tm *timeinfo = localtime(&timestamp);
//                            strftime(time_string, sizeof(time_string), "%X", timeinfo);
//                            snprintf(buffer, 1024, "Time on the server : %s", time_string);
//                            if (send(current_client->sockfd, buffer, strlen(buffer), 0) < 0) {
//                                stop("send");
//                            }
//
//                        }
//                        // /exit command
                        if (tbuf != NULL && !strcmp(tbuf, "/exit")) {
                            memset(buffer, '\0', 1024);
                            sprintf(buffer, "You are now disconnected !");
                            if (send(current_client->sockfd, buffer, 1024, 0) < 0) {
                                stop("send");
                            }
                            close(current_client->sockfd);
                            yellow();
                            printf("%s with socket %d is disconnected\n", current_client->pseudo,
                                   current_client->sockfd);
                            reset();
                            current_client->sockfd = -1;
                        }
//                        // /mp command
                        if (tbufMp != NULL && !strcmp(tbufMp, "/mp")) {
                            char privateDestName[32] = {0};
                            char privateDestMessage[256] = {0};
                            char errorMessage[256] = {0};
                            tbufMp = strtok(NULL, " ");
                            if (tbufMp != NULL && strcmp(tbufMp, "") != 0) {
                                strncpy(privateDestName, tbufMp, 32);
                            }
                            if (strcmp(privateDestName, "") == 0) {
                                printf("Hello\n");
                                snprintf(errorMessage, 256, "You dont have enter a name in your command, please retry");
                                if (send(current_client->sockfd, errorMessage, 256, 0) < 0) {
                                    stop("send");
                                }
                            } else {
                                tbufMp = strtok(NULL, "\n");
                                if (tbufMp != NULL && strcmp(tbufMp, "") != 0) {
                                    strncpy(privateDestMessage, tbufMp, 256);
                                }
                                memset(errorMessage, '\0', 256);
                                if (strcmp(privateDestMessage, "") == 0) {
                                    snprintf(errorMessage, 256,
                                             "You dont have enter message in your command, please retry");
                                    if (send(current_client->sockfd, errorMessage, 256, 0) < 0) {
                                        stop("send");
                                    }
                                } else {
                                    if (check_if_pseudo_exist(checking_node, privateDestName) == -1) {
                                        memset(buffer, '\0', 1024);
                                        snprintf(buffer, 1024, " private message from %s : %s", current_client->pseudo,
                                                 privateDestMessage);
                                        node_t *sending_private_node = head;
                                        send_private_message(sending_private_node, buffer, privateDestName);
                                    } else {
                                        snprintf(buffer, 1024, "Pseudo not found, please retry !");
                                        if (send(current_client->sockfd, buffer, 1024, 0) < 0) {
                                            stop("send");
                                        }
                                    }
                                }
                            }
                        }
                        if (tbufAlert != NULL && !strcmp(tbufAlert, "/alerte")) {
                            char pseudoOrMessage[256] = {0};
                            tbufAlert = strtok(NULL, " ");
                            if (tbufAlert != NULL && strcmp(tbufAlert, "") != 0) {
                                strncpy(pseudoOrMessage, tbufAlert, 256);
                                node_t *alert_node = head;
                                if (check_if_pseudo_exist(alert_node, pseudoOrMessage) == -1) {
                                    printf("le pseudo existe\n");
                                } else {
                                    memset(tempbuff, '\0', 1024);
                                    printf("/alerte %s\n", pseudoOrMessage);
                                }
                            }
                        }