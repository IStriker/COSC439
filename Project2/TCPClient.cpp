#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <iostream>     /* for cout, cin*/
#include <string>
#include <sstream>
#include <iterator>
#include <vector>


#define RCVBUFSIZE 30    /* Size of receive buffer */

using namespace std;


void DieWithError(char *errorMessage);  /* Error handling function */
string menu();        /* Display menu for user */
string search_keyword();
void display(string);
void send_message(int, string);
void check_buffer();
void clear_buffer();
void ack(int);

int screen_width = 0;           /* terminal width is 80 columns (0-79) */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    //echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */
    string full_message = "";

    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    //echoString = argv[2];         /* Second arg: string to echo */

    if (argc == 3)
        echoServPort = atoi(argv[2]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */



    while(true){

        /* Create a reliable, stream socket using TCP */
        if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            DieWithError("socket() failed");

        /* Construct the server address structure */
        memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
        echoServAddr.sin_family      = AF_INET;             /* Internet address family */
        echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
        echoServAddr.sin_port        = htons(echoServPort); /* Server port */

        /* Establish the connection to the echo server */
        if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
            DieWithError("connect() failed");


        /* get users choice from menu */
        string message = menu();

        /* if user chose to exit program */
        if(message == "exit")
            break;

        /* check size of message */
        if(message.length() <= RCVBUFSIZE) {

            /* Send the string to the server */
            if (send(sock, message.c_str(), strlen(message.c_str()), 0) != strlen(message.c_str()))
                DieWithError("send() sent a different number of bytes than expected");
            usleep(100);
            ack(sock);

        } else {    /* if greater than 30 characters than call send_message function */

            send_message(sock, message);

        }

        /* print received message */
        cout << "\n-------------------------------------------------------------------------------" << endl << endl;
        cout << "                                   Received News                                   " << endl << endl;
        cout << "-------------------------------------------------------------------------------" << endl << endl;


        while (true) {

            /* clear echoBuffer */
            memset(echoBuffer, '\0', RCVBUFSIZE);
            /* Receive up to the buffer size (minus 1 to leave space for
               a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                break;
            }

            echoBuffer[bytesRcvd] = '\0';           /* Terminate the string! */
            cout << echoBuffer;             /* Print the echo buffer */

        }

        cout << endl << endl;       /* print new line after recieved */
    }


    close(sock);
    exit(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function displays the menu to the user
 * to choose an option from
 *
 */
string menu(){

    start_menu:

    cout << "-------------------------------------------------------------------------------" << endl;
    cout << "-------------------------------------------------------------------------------" << endl << endl;

    int user_choice;
    string message;

    /* ask user, read user input and clear buffer */
    cout << "Please choose one of the following choices: " << endl;
    cout << "1. Receive today's news\n2. Search keyword news\n3. Quit program" << endl;
    cout << "Enter you choice: ";
    cin >> user_choice;


    switch(user_choice){

        case 1: /* todays news */
            message = "news feed";
            break;

        case 2: /* search keyword */
            message = search_keyword();
            break;

        case 3: /* quit the program */
            message = "exit";
            break;

        default:
            cout << "Invalid choice, please try again" << endl;
            goto start_menu;

    }

    return message;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function asks user for news topic they want to search
 *
 * @return string of the topic
 */
string search_keyword(){

    string keyword;

    /* ask user, read user input and clear buffer */
    check_buffer();
    cout << "Please enter keyword you want to search: ";
    getline(cin, keyword);
    return keyword;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function sends message longer than 30 characters to server
 *
 * @param sock the TCP socket
 * @param message the message to send to server
 */
 void send_message(int sock, string message){

    /* declare required variables */
    int index = 0, i, sendMsgSize;
    char echoBuffer[RCVBUFSIZE];

    /* assign new content string to char array and get size*/
    char char_array[message.length()];
    strcpy(char_array, message.c_str());
    int size = message.length();

    /* Send news string until end of transmission */
    while (index <= size)      /* when index equals to message size, end */
    {
        /* clear echoBuffer and assign 30 characters to it */
        memset(echoBuffer, '\0', sizeof(char) * RCVBUFSIZE);

        for (i = 0; i < RCVBUFSIZE; i++) {
            if ((i + index) < size) {
                echoBuffer[i] = char_array[i + index];
            } else {
                echoBuffer[i] = '\0';
                break;
            }

        }/* end for loop */

        index += RCVBUFSIZE;
        sendMsgSize = strlen(echoBuffer);

        /* Send the string to the server */
        if (send(sock, echoBuffer, sendMsgSize, 0) != sendMsgSize)
            DieWithError("send() sent a different number of bytes than expected");
    }

    usleep(100);
    ack(sock);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function checks keyboard buffer
 * and calls clear_buffer if anything in buffer
 */
void check_buffer(){

    if(cin)
        clear_buffer();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * clears read buffer
 */
void clear_buffer(){

    cin.ignore();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this functions sends ack to let server know client
 * is finished sending
 */
 void ack(int sock){

    char echoBuffer[] = "ack";
    int size = strlen(echoBuffer);
    if (send(sock, echoBuffer, size, 0 ) != size)
        DieWithError("send() sent a different number of bytes than expected");

}
