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


#define ECHOMAX 140     /* Longest string to echo */
#define TRUE 1
#define FALSE 0


void DieWithError(char *errorMessage);  /* External error handling function */
ServerMessage check_options(ServerMessage, ClientMessage);
string login(string, unsigned int);
int generate_id();
int update_followers(int *, int, unsigned int);
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
    
    /* check if login */
    if(recieve_message.request_type == ClientMessage::Login){
        string message_temp = login(send_message.message, recieve_message.UserID);
        strcpy(send_message.message, message_temp.c_str());
        if(strcmp(send_message.message, "Login Success") == 0)
            send_message.UserID = recieve_message.UserID;
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
    for(i = 0; i < size; i++){
        
        if(id == user_ids[i])
            return "Login success";
        
    }
    
    return "Login fail";

}
