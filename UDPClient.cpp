					     /* for printf() and fprintf() */
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
void menu(ClientMessage *);
int is_first_login();
int ask_for_id(char*);
void display_sample_leaders();
void display_updated_list(ServerMessage *, int);
void post(char *);
void clear_buffer();
unsigned int request_message_feeds();
unsigned int unfollow_request();

int main(){
	cout << "This is ahmed aldhaheri" << endl;
	string name = "Ahmed Aldhaheri";
	cout << name << endl;
	return 0;
}