#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

/* client message struct */
typedef struct {
    
    enum {
        FirstLogin, Login, Follow, Post, Search,
        Receive, Delete, Unfollow, Logout, LoggedIn
    } request_type;                     /* same size as unsigned int */
    
    unsigned int rquest_id;                      /* request client sends */
    
    unsigned int UserID;                /* unique client identifier */
    
    unsigned int LeaderID;              /* unique client indentifiere */
    
    char message[140];
    
} ClientMessage;

typedef struct{
    
    unsigned int LeaderID ;  /* unique client identifier */
    
    /* store users following, by default its all zeros*/
    int following[10];
    
    unsigned int UserID;    /* unique user id */
    
    char message[140];    /* text message */
    
}ServerMessage;

/* list of sample users you can follow 
 * these users exist in server with sample
 * posted messages you can retrieve.
 * Of course if you login with multiple users to server
 * and know the userID's of these users you can follow these users
 * and retrieve their info while application is running.
 */
int sample_leaders_ids[5] = {70, 75, 80, 85, 90};
