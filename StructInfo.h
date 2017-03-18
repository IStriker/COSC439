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
 * 
 */
int leaders_ids[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

/* table of logged in users
 */
int logged_in_users[10], logged_in_index = 0;


/* store all users ids */
int user_ids[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


string posted_messages[10][10];
int posted_index [10];

posted_messages[1] = {
    "This is a test string for user id 1",
    "This is test string 2 for user id 1",
    "This is a test string 3 for user id 1"
};

posted_messages[10] = {
    "This is a test string for user id 10",
    "This is a test string 2 for user id 10",
    "This is a test string 3 for user id 10"
};
posted_index[1] = 3;
posted_index[10] = 3;

/* stores users following list */
int following_list[10][10];
int following_index[10];




