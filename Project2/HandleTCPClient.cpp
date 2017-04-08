#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>

#define RCVBUFSIZE 23   /* Size of receive buffer */

using namespace std;

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    char *message;
    int index = 0, i;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    cout << "before assignment" << endl;
    /* check what is the request */
    if(strcmp(echoBuffer, "news feed") == 0) {
        cout << "inside true" << endl;
        message = "Hello this ahmed aldhaheri and I am testing my TCP program if it works\0";
    }
    int size = strlen(message);
    cout << "after assignment" << endl;
    cout << "size: " << size << endl;
    cout << "message: " << message << endl;
    /** @test */
    /* Send received string and receive again until end of transmission */
    while (index <= size)      /* zero indicates end of transmission */
    {

        for(i = 0; i < RCVBUFSIZE; i++){
            if((i + index) < size) {
                echoBuffer[i] = message[i + index];
                cout << "i + index: " << (i + index) << endl;
            }
            else{
                for(i = i; i < RCVBUFSIZE; i++)
                    echoBuffer[i] = '\0';
            }

        }
        index += RCVBUFSIZE;
        cout << echoBuffer << endl;
        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
            //DieWithError("send() failed");
            cout << "Error happened when sending to client" << endl;
        }


    }


    close(clntSocket);    /* Close client socket */
}
