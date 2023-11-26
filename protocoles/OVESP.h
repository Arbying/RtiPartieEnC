#ifndef OVESP_H
#define OVESP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "API_bd.h"

#define NB_MAX_CLIENTS 5

extern int estOk;

bool OVESP(char* requete, char* reponse, int socket);


#endif
