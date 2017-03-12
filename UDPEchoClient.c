#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "StructInfo.h"

#define ECHOMAX 140     /* Longest string to echo */
#define TRUE 1
#define FALSE 0

void DieWithError(char *errorMessage);  /* External error handling function */
void login(ClientMessage *);
void menu(ClientMessage *);
int is_first_login();
int ask_for_id(char*);
void display_sample_leaders();
void display_updated_list(ServerMessage *, int);

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    int echoStringLen;               /* Length of string to echo */
    
    ClientMessage send_message;              /* struct for sending */
    ServerMessage recieve_message;           /* struct for reciveing */
    
    
    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    strcpy(send_message.message, argv[2]);       /* Second arg: string to echo */
    

    
    if((strlen(send_message.message)) > ECHOMAX)   /* Check input length */
        DieWithError("Echo word too long");

    if (argc == 4)
        echoServPort = atoi(argv[3]);  /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */
    
    /*check if user sent login request when program is first launched*/
    send_message.request_type = Logout;
    
    printf("----------------------------------------------------------------\n");
    printf("----------------------------------------------------------------\n\n");
    printf("Please login first\n\n");
    
    /* infinite loop */
    while(TRUE){
        
        
        menu(&send_message);
       
        /* Send the string to the server */
        if (sendto(sock, (ClientMessage*)&send_message, sizeof(send_message), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != sizeof(send_message))
            DieWithError("Client: sendto() sent a different number of bytes than expected");

        /* Recv a response */
        fromSize = sizeof(fromAddr);
        if ((recvfrom(sock, (ServerMessage *) &recieve_message, sizeof(recieve_message), 0,
                      (struct sockaddr *) &fromAddr, &fromSize)) < 0){
            DieWithError("Client: recvfrom() failed");
        }

        if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
            fprintf(stderr,"Error: received a packet from unknown source.\n");
            exit(1);
        }

        printf("Received: %s\n", recieve_message.message);    /* Print the echoed arg *///
        printf("user id:  %d\n", recieve_message.UserID);
        
        /* check if login success or not */
        if(strcmp(recieve_message.message, "Login success") == 0){
            printf("Connection established\n");
        }
        else if(strcmp(recieve_message.message, "Login fail") == 0){
            printf("Connection failed, please try again\n");
            send_message.request_type = Logout;
        }
        else if(strcmp(recieve_message.message, "Follow success") == 0){
            int size = sizeof(recieve_message.following) / sizeof(int);
            display_updated_list(&recieve_message, size);
        }
        else if(strcmp(recieve_message.message, "Follow failed") == 0){
            display_sample_leaders();
            send_message.LeaderID = ask_for_id("Please enter id of leader to follow: ");
            continue;
        }
        
        //menu(&send_message);
    }//end while
    
    
    
    close(sock);
    exit(0);
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * checks wether it is first login or not
 * @return whether it is true or false
 */
int is_first_login(){
    
    char input[5];
    printf("Do you have a user id ((yes or YES) or (no or NO)): ");
    scanf("%s", input);
    
    if(strcmp(input, "NO") == 0 || strcmp(input, "no") == 0)
        return TRUE;
    return FALSE;
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * asks user for id and returns it
 * @param msg the messsage to display to the user
 */
int ask_for_id(char *msg){
    int id;
    printf("%s", msg);
    scanf("%d", &id);
    return id;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function displays menu options for user
 *
 * @param *send_message the struct pointer to send to server
 * @param options the options represented by enum
 */
void menu(ClientMessage *send_message){
    
    int input, choice;
    printf("----------------------------------------------------------------\n");
    printf("----------------------------------------------------------------\n\n");
    printf("Please choose an option from the following menu\n\n");
    printf("1: Login\n2: Follow one or more users\n3: Post a message\n");
    printf("4: Receive message feed from all leaders\n5: search message with tag");
    printf("\n6: Delete a posted message\n7: Unfollow one or more leaders\n8: logout");
    printf("\n9: Quit the client program\nEnter your choice here: ");
    scanf("%d", &input);
    
    //check user choice
    switch(input){
        case 1:     /* login */
            
            if(send_message->request_type == Logout){
                
                if(is_first_login()){
                    send_message->request_type = FirstLogin;
                    return;
                }
                else{
                    send_message->request_type = Login;
                    send_message->UserID = ask_for_id("Please enter your ID: ");
                    return;
                }

            }
            
            send_message->request_type = LoggedIn;
            break;
        case 2:     /* follow */
            send_message->request_type = Follow;
            display_sample_leaders();
            send_message->LeaderID = ask_for_id("Please enter id of leader to follow: ");
            break;
        case 3:     /* post */
            send_message->request_type = Post; break;
        case 4:
            send_message->request_type = Receive; break;
        case 5:
           send_message->request_type = Search; break;
        case 6:
            send_message->request_type = Delete; break;
        case 7:
            send_message->request_type = Unfollow; break;
        case 8:
            send_message->request_type = Logout; break;
        case 9:
            exit(0);break;
        default:
            break;
            
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * displays the sample leaders id's
 */
void display_sample_leaders(){
    
    int i;
    int size = sizeof(sample_leaders_ids) / sizeof(int);
    for(i = 0; i < size; i++)
        printf("leader id: %d\n", sample_leaders_ids[i]);
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * displays updated list after a follow request
 * @param recieve_message the recieved struct
 * @param size the size of the list
 */
void display_updated_list(ServerMessage *recieve_message, int size){
    
    int i;
    printf("--------------------------------------------------------\n\n");
    printf("Updated following list\n");
    for(i = 0; i < size; i++){
        if(recieve_message->following[i] != 0)
            printf("Leader ID: %d\n", recieve_message->following[i]);
    }
    
}
