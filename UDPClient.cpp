#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "StructInfo.h"
#include <iostream>
#include <string>


#define ECHOMAX 140     /* Longest string to echo */
#define TRUE 1
#define FALSE 0

using namespace std;

/* prototypes*/
void DieWithError(char *errorMessage);  /* External error handling function */

void login(ClientMessage *);
ClientMessage menu(ClientMessage );
int is_first_login();
int ask_for_id(string);
void display_sample_leaders();
void display_updated_list(ServerMessage *, int);
void post(char *);
void clear_buffer();
unsigned int request_message_feeds();
unsigned int unfollow_request();

int main(int argc, char* argv[]){
	
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                     /* IP address of server */
    string echoString;                /* String to send to echo server */
    int echoStringLen;               /* Length of string to echo */
    string message_temp;                /* to store messages to pass to methods */
    
    ClientMessage send_message;              /* struct for sending */
    ServerMessage recieve_message;           /* struct for reciveing */
    
    
    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }
    
    servIP = argv[1];                   /* First arg: server IP address (dotted quad) */
    strcpy(send_message.message, argv[2]);      /* Second arg: string to echo */
    
    if(strlen(send_message.message) > ECHOMAX){
        DieWithError("Echo word too long");
    }
    
    
    if (argc == 4)
        echoServPort = atoi(argv[3]);   /* Use given port, if any */
    else
        echoServPort = 7;               /* 7 is the well-known port for the echo service */
    
    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port   = htons(echoServPort);     /* Server port */

    /*check if user sent login request when program is first launched*/
    send_message.request_type = ClientMessage::Logout;
    
    /* display message for user */
    cout << "----------------------------------------------------------------" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    cout << "Please login first" << endl << endl;
    
    /* call menu options */
    send_message = menu(send_message);
    
    /* Send the string to the server */
    if (sendto(sock, (ClientMessage*)&send_message, sizeof(send_message), 0,
               (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != sizeof(send_message))
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
    
    cout << "Recieved: " << recieve_message.message << endl;
    
    close(sock);
    exit(0);
    
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function displays menu options for user
 *
 * @param *send_message the struct pointer to send to server
 * @param options the options represented by enum
 * @return a struct
 */
ClientMessage menu(ClientMessage send_message){
    
    int input;
    
    cout << "----------------------------------------------------------------" << endl;
    cout << "----------------------------------------------------------------" << endl << endl;
    cout << "Please choose an option from the following menu\n\n";
    cout << "1: Login\n2: Follow one or more users\n3: Post a message\n";
    cout << "4: Receive message feed from all leaders\n5: search message with tag";
    cout << "\n6: Delete a posted message\n7: Unfollow one or more leaders\n8: logout";
    cout << "\n9: Quit the client program\nEnter your choice here: ";
    cin >> input;
    
    
    //check user choice
    switch(input){
        case 1:     /* login */
            
            if(send_message.request_type == ClientMessage::Logout){
                
                send_message.request_type = ClientMessage::Login;
                send_message.UserID = ask_for_id("Please enter your ID: ");
                return send_message;
            }
            
            send_message.request_type = ClientMessage::LoggedIn;
            break;
        /*case 2:     /* follow */
            /*if(send_message->request_type == Logout){
                printf("Please login first\n");
                return;
            }
            
            send_message->request_type = Follow;
            display_sample_leaders();
            send_message->LeaderID = ask_for_id("Please enter id of leader to follow: ");
            break;
        case 3:     /* post */
            /*if(send_message->request_type == Logout){
                printf("Please login first\n");
                return;
            }
            
            send_message->request_type = Post;
            post(send_message->message);
            break;
        case 4:
            send_message->request_type = Receive;
            printf("This feature does not exist\n");
            break;
        case 5:
            send_message->request_type = Search;
            printf("This feature does not exist\n");
            break;
        case 6:
            send_message->request_type = Delete;
            printf("This feature does not exist\n");
            break;
        case 7:
            /*if(send_message->request_type != LoggedIn){
             printf("Please login first\n");
             return;
             }
             
             send_message->request_type = Unfollow;
             send_message->LeaderID = unfollow_request();*/
           /* printf("This feature does not exist\n");
            break;
        case 8:
            send_message->request_type = Logout; break;
        case 9:
            exit(0);break;
        default:
            printf("Invlaid choice, please try again\n");
            getchar();
            goto start_menu;
            break; */
            
    }

    return send_message;
   
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * asks user for id and returns it
 * @param msg the messsage to display to the user
 */
int ask_for_id(string msg){
    
    int id;
    cout << msg;
    cin >> id;
    return id;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * checks recieved struct from server and displays 
 * appropriate messages and does appropriate computations
 * @param send_message client struct recieve_message the struct sent from server
 * @param srecieve_message the struct sent from server
 * @return send_message the message to send to menu for checking
 * and then to server
 */
ClientMessage recieve_check(ClientMessage send_message, ServerMessage recieve_message){
    
    /* if user did not send a logout request display recieved info */
    if(send_message.request_type != ClientMessage::Logout){
        cout << "Received: " << send_message.message << endl;
        cout << "User id: " << send_message.UserID << endl;
    }
}
