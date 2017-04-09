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

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    while(true){

        /* Establish the connection to the echo server */
        if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
            DieWithError("connect() failed");

        /* get users choice from menu */
        string message = menu();
        cout << "message: " << message << endl;

        /* Send the string to the server */
        if (send(sock, message.c_str(), strlen(message.c_str()), 0) != strlen(message.c_str()))
            DieWithError("send() sent a different number of bytes than expected");

        /* Receive the same string back from the server */
        totalBytesRcvd = 0;
        cout << "Received: " << endl;                /* Setup to print the echoed string */
        while (true) {
            memset(echoBuffer, '\0', RCVBUFSIZE);
            /* Receive up to the buffer size (minus 1 to leave space for
               a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                //DieWithError("recv() failed or connection closed prematurely");
                break;
            }
            totalBytesRcvd += bytesRcvd;            /* Keep tally of total bytes */
            //cout << "bytes recieved: " << bytesRcvd << endl;
            echoBuffer[bytesRcvd] = '\0';           /* Terminate the string! */
            cout << echoBuffer;             /* Print the echo buffer */

        }
    }

    cout << "\n";    /* Print a final linefeed */

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
            exit(1);
            break;

        default:
            cout << "Invalid choice, please try again" << endl;
            goto start_menu;

    }

    return message;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

string search_keyword(){

    return "";
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * takes string message and displays in a readable format
 * @param message
 */
void display(string message){
    //cout << "string: " << message << endl;
    stringstream str(message);
    istream_iterator<string> begin(str);
    istream_iterator<string> end;
    vector<string> vect_strings (begin, end);
    ostringstream buffer;
    copy(vect_strings.begin(), vect_strings.end(), ostream_iterator<string>(buffer,""));

    for(int i = 0; i < vect_strings.size(); i++){

        cout << vect_strings[i] << endl;
    }


}