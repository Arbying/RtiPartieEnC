#ifndef TCP_H
#define TCP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>

#define TAILLE_MAX_DATA 10000


int ServerSocket(int port);
int Accept(int sEcoute,char *ipClient);
int ClientSocket(char* ipServeur,int portServeur);
int Send (int sSocket, char* data, int taille);
int Receive (int sSocket, char* data);

#endif