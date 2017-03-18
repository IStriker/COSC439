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
ClientMessage recieve_check(ClientMessage, ServerMessage);
int is_first_login();
int ask_for_id(string);
void display_leaders(unsigned int);
void display_updated_followers(ServerMessage);
string post();
void clear_buffer();
bool is_logged_in(ClientMessage);
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

    while(TRUE){
        /* call menu options */
        send_message = menu(send_message);
    
        /* Send the string to the server */
        if (sendto(sock, (ClientMessage*)&send_message, sizeof(send_message), 0,
               (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != sizeof(send_message))
            DieWithError("Client: sendto() sent a different number of bytes than expected");
    
        
        /* check to see if recieveing multiple messages */
        do {
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
        
            send_message = recieve_check(send_message, recieve_message);
        
        } while(recieve_message.number_of_messages > 0);
            
    }
    
    close(sock);
    exit(0);
    
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
    string message;
    
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
            
            if(!is_logged_in(send_message)){
                
                send_message.request_type = ClientMessage::Login;
                send_message.UserID = ask_for_id("Please enter your ID: ");
                return send_message;
            }
            
            send_message.request_type = ClientMessage::LoggedIn;
            break;
        case 2:     /* follow */
            if(!is_logged_in(send_message)){
                return send_message;
            }
            
            send_message.request_type = ClientMessage::Follow;
            display_leaders(send_message.UserID);
            send_message.LeaderID = ask_for_id("Please enter id of leader to follow: ");
            break;
        case 3:     /* post */
            if(!is_logged_in(send_message)){
                return send_message;
            }
            
            send_message.request_type = ClientMessage::Post;
            message = post();
            strcpy(send_message.message, message.c_str());
            break;
        case 4:
            if(!is_logged_in(send_message)){
                return send_message;
            }
            
            send_message.request_type = ClientMessage::Receive;
            break;
        /*case 5:
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
    if(send_message.request_type != ClientMessage::Logout &&
       strcmp(recieve_message.message, "Login fail") != 0 &&
       send_message.request_type != ClientMessage::Receive){
        cout << "\nReceived: " << recieve_message.message << endl;
        cout << "User id: " << recieve_message.UserID << endl;
    }
    
    
    if(strcmp(recieve_message.message, "Login success") == 0 &&     /* check if login success */
       send_message.request_type != ClientMessage::Logout){
        cout << "\nConnection established" << endl;
        send_message.request_type = ClientMessage::LoggedIn;
        
    } else if(strcmp(recieve_message.message, "Login fail") == 0){  /* check if login failed */
        
        cout << "\nerror: Connection failed, Please try again" << endl;
        send_message.request_type = ClientMessage::Logout;
        
    } else if(strcmp(recieve_message.message, "Follow success") == 0){       /* check if follow success */
        
        display_updated_followers(recieve_message);
        
    } else if(strcmp(recieve_message.message, "Follow failed") == 0){   /* if follow failed */
        
        cout << "\nerror: Leader id does not exist, or already in following list" << endl;
        
    } else if(send_message.request_type == ClientMessage::Receive
              && strcmp(recieve_message.message, "Recieve is successful") != 0){
        
        cout << "\nReceived: " << recieve_message.message << endl;
        cout << "Leader id: " << recieve_message.LeaderID << endl;

    } else if(strcmp(recieve_message.message, "Recieve is successful") == 0){
        
        cout << "\nReceived: " << recieve_message.message << endl;
        cout << "User id: " << recieve_message.UserID << endl;
    }
    
    return send_message;
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * displays the sample leaders id's
 * @param id client id
 */
void display_leaders(unsigned int id){
    
    int i;
    cout << "\n\n";
    int size = sizeof(leaders_ids) / sizeof(leaders_ids[0]);
    for(i = 0; i < size; i++){
        if(leaders_ids[i] != id && leaders_ids[i] != 0)
            cout << "Leader id: " << leaders_ids[i] << endl;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * displays updated list after a follow request
 * @param recieve_message the recieved struct
 *
 */
void display_updated_followers(ServerMessage recieve_message){
    
    int i;
    int size = sizeof(recieve_message.following) / sizeof(recieve_message.following[0]);
    cout << "--------------------------------------------------------\n\n";
    cout << "Updated following list" << endl;
    for(i = 0; i < size; i++){
        if(recieve_message.following[i] != 0)
            cout << "Leader ID: " << recieve_message.following[i] << endl;
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * asks a user for the post message and
 * validates that it is no longer than 140
 * characters
 * @return message the post message
 */
string post(){
 
start_post:
    
    clear_buffer();
    string message;
    cout << "Please enter the message you would like to post: ";
    getline(cin, message);
    
    
    int length = message.size();
    
    if(length > ECHOMAX){
        cout << "Your message exceeded the limit, please try again" << endl;
        goto start_post;
    }
    
    return message;
    
}

//////////////////////////////////////////////////////////////////////////////////////////

/**
 * clears read buffer
 */
void clear_buffer(){
    
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function checks if user is logged in or not
 * @param the client message
 * @return bool
 */
bool is_logged_in(ClientMessage send_message){
    
    if(send_message.request_type == ClientMessage::Logout){
        cout << "Please login first" << endl;
        return false;
    }
    
    return true;
    
}

