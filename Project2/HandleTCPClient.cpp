#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define RCVBUFSIZE 30   /* Size of send Buffer */

using namespace std;

void DieWithError(char *errorMessage);  /* Error handling function */
string read_news_file();

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    string send_message;                /* Send message to client */
    int index = 0, i;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    echoBuffer[recvMsgSize] = '\0';

    /* check what is the request */
    if(strcmp(echoBuffer, "news feed") == 0) {
        send_message = read_news_file();
    } else if(strcmp(echoBuffer, "#item") == 0){

    }

    /* assign new content string to char array and get size*/
    char message[send_message.length()];
    strcpy(message, send_message.c_str());
    int size = strlen(message);

    /* Send news string until end of transmission */
    while (index <= size)      /* zero indicates end of transmission */
    {
        /* clear echoBuffer and assign 30 characters to it*/
        memset(echoBuffer, '\0', sizeof(char) * RCVBUFSIZE);

        for(i = 0; i < RCVBUFSIZE; i++){
            if((i + index) < size) {
                echoBuffer[i] = message[i + index];
                //cout << "i + index: " << (i + index) << endl;
            }
            else{
                echoBuffer[i] = '\0';
                break;
            }

        }

        index += RCVBUFSIZE;
        cout << echoBuffer << endl;
        recvMsgSize = strlen(echoBuffer);

        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
            DieWithError("send() failed");
        }


    }


    close(clntSocket);    /* Close client socket */
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function responds to the recieve today's
 * news request by read the day news file and return
 * the file content in a string
 *
 * @return string
 */
string read_news_file(){

    /* open file to read */
    string file_name = "News Feed/11-21-2016.txt";
    ifstream file(file_name.c_str());
    stringstream buffer;

    buffer << file.rdbuf();
    string str = buffer.str();
    return str;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function searches for an item tag
 */
