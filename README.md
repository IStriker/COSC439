Ahmed Aldhaheri
Course: COSC439
Date: 3/13/2017

This program is not complete.

Client program: UDPEchoClient.c
Server program: UDPEchoServer.c

Example Compiling: gcc -o UDPEchoClient UDPEchoClient.c DieWithError.can

When you launch the client program it will display a menu. 
You first need to login with a user id that already exist in the 
StructInfo.h in the user_ids array. The array has user ids from 0-9.
You can choose any number from 0-9 to login. once logged in, you can do the followin:
1. follow users
2. post message
3. Logout 
4. quit program

Note: I really spent so much time on this project. First I used a mac os and program broke
because I tried all the tricks to read a string input, but failed. Switched to emunix
and had to refactor a large portion of my program, because it wouldn't compile. If I make
progress one step, I am back 10 steps because of the pointer errors it was generating.
Emunix gave me so much trouble and slowed me down a lot.
I was hoping for extension, but now my only hope is for you to be lenient when grading.
