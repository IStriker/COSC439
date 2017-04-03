//
// Created by Ahmed Aldhaheri on 3/31/17.
//

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <string>

using namespace std;

/* client message struct */
typedef struct {

    enum {
        NewsFeed, Search
    } request_type;

} ClientMessage;

typedef struct{

   string message;

}ServerMessage;


