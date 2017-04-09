#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>



#define RCVBUFSIZE 30   /* Size of send Buffer */

using namespace std;

void DieWithError(char *errorMessage);  /* Error handling function */
string read_news_file();
string search_item_tag(string);

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    string send_message;                /* Send message to client */
    string receive_message = "";              /* receive message */
    int index = 0, i;

    /* Receive message from client */
    while(true) {

        /* clear echoBuffer */
        memset(echoBuffer, '\0', RCVBUFSIZE);
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            break;

        echoBuffer[recvMsgSize] = '\0';

        if(strcmp(echoBuffer, "ack") == 0)
            break;

        string str(echoBuffer);
        receive_message += str;

    }


    /* check what is the request */
    if(receive_message == "news feed") {

        send_message = read_news_file();

    } else { /* if search for item */

        send_message = search_item_tag(receive_message);

    }

    /* assign new content string to char array and get size*/
    char message[send_message.length()];
    strcpy(message, send_message.c_str());
    int size = strlen(message);

    /* Send news string until end of transmission */
    while (index <= size)      /* when index equals to message size, end */
    {
        /* clear echoBuffer and assign 30 characters to it */
        memset(echoBuffer, '\0', sizeof(char) * RCVBUFSIZE);

        for(i = 0; i < RCVBUFSIZE; i++){
            if((i + index) < size) {
                echoBuffer[i] = message[i + index];
            }
            else{
                echoBuffer[i] = '\0';
                break;
            }

        }/* end for loop */

        index += RCVBUFSIZE;
        //cout << echoBuffer << endl;
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

    /* read entire file content */
    buffer << file.rdbuf();
    string str = buffer.str();
    return str;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * this function searches for an item tag
 *
 * @param item the string to search for in file
 * @return the content of item
 */
string search_item_tag(string item){


    string file_content = read_news_file();
    string result = "";


    /* parse file content and look for item */
    for(int i = 0; i < file_content.length(); i++){

        /* check if first character in item match */
        if(file_content[i] == item[0]){

            /* check for all characters in item */
            for(int j = 0, k = i; j < item.length(); j++, k++){

                if(item[j] == file_content[k]){

                    /* if all characters match then, its the item */
                    if(j == item.length() - 1){

                        /* read item and assign to result, until item is done */
                        for(int j = 0, k = i; k < file_content.length();j++, k++){

                            /* if two new lines in sequence its end of item */
                            if(file_content[k] == '\n' && file_content[k+1] == '\n'){

                                result += file_content[k];
                                break;
                            }

                            result += file_content[k];

                        } /* end third inner loop */

                    }

                } else {

                    break;
                }


            }/* end second loop */

        }

    }/* end outer loop */

    return result;

}