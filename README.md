Ahmed Aldhaheri
Course: COSC439
Date: 3/18/2017

#Description:
This program is not complete: (delete does not work)
This is a multithreaded program, which allows mutliple clients
to connect to the server and do the described below operations

#Compiling:
Client program: UDPClient.cpp
Server program: UDPServer.cpp

Example Compiling client: g++ -o UDPClient UDPClient.cpp DieWithError.cpp
Example Compiling server: g++ -o UDPServer UDPServer.cpp DieWithError.cpp

##Machine:
This was developed, compiled, and ran on a macOS Sierra version 10.12.13

##Program 
When you launch the client program it will display the following menu: 

1: Login
2: Follow one or more users
3: Post a message
4: Receive message feed from all leaders
5: search message with tag
6: Delete a posted message
7: Unfollow one or more leaders
8: logout
9: Quit the client program

#Note: 
Option 6 (Delete a posted message) is not working, but still will display,
if clicked on, it will display message and lauch back the menu

#Operation:
You first need to login with a user id that already exist in the 
StructInfo.h in the user_ids array (see below). The array has user ids from 1-10.
You can choose any number from 1-10 to login. once logged in, you can do the following:

1: Login
2: Follow one or more users
3: Post a message
4: Receive message feed from all leaders
5: search message with tag
7: Unfollow one or more leaders
8: logout
9: Quit the client program

#Extra Features:
The program saves client information even when they quit the program.
The only way it loses the client information when server program quits.

#below
/* store all users ids */
int user_ids[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
