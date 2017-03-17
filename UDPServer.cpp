#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>       /* srand(), rand() */
#include "StructInfo.h"
#include <iostream>
#include <string>
#include <algorithm>


#define ECHOMAX 140     /* Longest string to echo */
#define TRUE 1
#define FALSE 0

using namespace std;


void DieWithError(char *errorMessage);  /* External error handling function */
ServerMessage check_options(ServerMessage, ClientMessage);
string login(string, unsigned int);
int generate_id();
ServerMessage update_followers(ServerMessage, ClientMessage);
int exist(int*, int, unsigned int);
void post(char*, int);


int main(int argc, char *argv[]){
    
    
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    string echoBuffer;        /* Buffer for echo string */
    unsigned short echoServPort;     /* Server port */
    
    ServerMessage send_message;     /* struct for sending */
    ClientMessage recieve_message;  /* struct for recieveing */
    
    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }
    
    echoServPort = atoi(argv[1]);  /* First arg:  local port */
    
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */
    
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    
    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);
        
        /* Block until receive message from a client */
        if ((recvfrom(sock, (ClientMessage *) &recieve_message, sizeof(recieve_message), 0,
                      (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
            DieWithError("Server: recvfrom() failed");
        
        //printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        cout << "Handling client: " << inet_ntoa(echoClntAddr.sin_addr) << endl;
        
        send_message = check_options(send_message, recieve_message);
        
        /* Send received datagram back to the client */
        if (sendto(sock, (ServerMessage *) &send_message, sizeof(send_message), 0,
                   (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != sizeof(send_message))
            DieWithError("SERVER: sendto() sent a different number of bytes than expected");
    }
    /* NOT REACHED */


    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * checks what option client sent
 * @param send_message the struct that will be sent
 * @param recieve_message the struct recieved from client
 * @return send_message the server struct
 */
ServerMessage check_options(ServerMessage send_message, ClientMessage recieve_message){
    
    
    if(recieve_message.request_type == ClientMessage::Login){           /* check if login */
        
        string message_temp = login(send_message.message, recieve_message.UserID);
        strcpy(send_message.message, message_temp.c_str());
        if(strcmp(send_message.message, "Login success") == 0){
            send_message.UserID = recieve_message.UserID;
            copy(begin(following_list[send_message.UserID]),end(following_list[send_message.UserID]),
                 begin(send_message.following));
            logged_in_users[logged_in_index] = recieve_message.UserID;
            logged_in_index++;
        }
        
    } else if(recieve_message.request_type == ClientMessage::Follow){   /* check if follow */
        
        int size = sizeof(send_message.following) / sizeof(send_message.following[0]);
        send_message.UserID = recieve_message.UserID;
        send_message = update_followers(send_message, recieve_message);
        
    } else if(recieve_message.request_type == ClientMessage::LoggedIn){		/* if logged in */
        
        strcpy(send_message.message, "You are Logged in already!");
        send_message.UserID = recieve_message.UserID;
        
    } else if(recieve_message.request_type == ClientMessage::Post){
        
    }
    
    return send_message;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function checks user id entered,
 * if it exists, if not it will not allow user
 * to login
 * @param id the user id entered
 * @return whether login was success or fail
 */
string login(string word, unsigned int id){
    
    int i;
    int size = sizeof(user_ids) / sizeof(user_ids[0]);
    
    //check if id already in logged in users
    if(exist(logged_in_users, size, id))
        return "You are logged in already";
    
    for(i = 0; i < size; i++){
        
        if(id == user_ids[i])
            return "Login success";
        
    }
    
    return "Login fail";

}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * this updates the leaders list of a client
 * @param send_message the server struct
 * @param recieve_message the client message
 * @return send_message
 */
ServerMessage update_followers(ServerMessage send_message, ClientMessage recieve_message){
    
    int i;
    int size_user_ids = sizeof(user_ids) / sizeof(user_ids[0]);
    int size_leaders_ids = sizeof(leaders_ids) / sizeof(leaders_ids[0]);
    int userid = send_message.UserID;
    
    //check if user exist in leaders_ids array and if user in following list
    if(!exist(leaders_ids, size_user_ids, recieve_message.LeaderID) ||
       exist(following_list[send_message.UserID], size_leaders_ids, recieve_message.LeaderID)){
        strcpy(send_message.message, "Follow failed");
        return send_message;
    }
        
    //else add and make sure user did not accidently put their id, to follow their self
    for(i = 0; i < size_leaders_ids; i++){
        if(leaders_ids[i] == recieve_message.LeaderID
           && leaders_ids[i] != recieve_message.UserID){
            
            //update global 2D following_list array and client following list
            following_list[userid][following_index[userid]] = recieve_message.LeaderID;
            copy(begin(following_list[userid]),end(following_list[userid]), begin(send_message.following));
            following_index[userid]++;
            strcpy(send_message.message, "Follow success");
            return send_message;
            
        }
    }
    
    strcpy(send_message.message, "Follow failed");
    
    return send_message;
    
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function looks if a target exist in array
 * @param array the array
 * @param size the size of array
 * @param target the item to search for
 * @return TRUE or FALSE
 */
int exist(int *array, int size, unsigned int target){
    
    int i;
    for(i = 0; i < size; i++){
        if(array[i] == target)
            return TRUE;
    }
    
    return FALSE;
}

