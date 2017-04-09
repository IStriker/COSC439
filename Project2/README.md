Ahmed Aldhaheri
Course: COSC439
Date: 4/10/2017

#Description:
This program connects client to server using TCP socket. Client can ask for news
items to receive from server or the whole daily news file.

to connect to the server and do the described below operations

#Compiling:
Client program: TCPClient.cpp
Server program: TCPServer.cpp

Example Compiling client: g++ -o TCPClient TCPClient.cpp DieWithError.cpp
Example Compiling server: g++ -o TCPServer TCPServer.cpp HandleTCPClient.cpp DieWithError.cpp

#Run
./TCPServer <port>--> (ex: 20000)
./TCPClient 127.0.0.1  <port>--> (ex: 20000)


##Machine:
This was developed, compiled, and ran on a macOS Sierra version 10.12.13

##Program 
When you launch the client program it will display the following menu: 

1. Receive today's news
2. Search keyword news
3. Quit program


#Operations:
When you choose choice 1 from above menu a "news feed" message will be sent
to the server, the server will send back the entire content of the
11-21-2016.txt file provided on canvas. When choosing choice 2 from above menu
the program will prompt you to enter the search keyword (refer to #Sample below)
and it will send it to server.The server will parse the 11-21-2016.txt file and
find the item news and return it to client. When choice 3 is picked, the program
will quit.

#Sample runs for keyword search:
Note: for search to work, you have to enter the whole news head line including #item

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

Please choose one of the following choices:
1. Receive today's news
2. Search keyword news
3. Quit program
Enter you choice: 2
Please enter keyword you want to search: #item Jaguar unveils first electric car

-------------------------------------------------------------------------------

                                   Received News

-------------------------------------------------------------------------------

#item Jaguar unveils first electric car
by Bronte Lord & Richa Naik   @CNNMoney
Jaguar showed off the all-wheel-drive I-Pace at the Los Angeles Motor Show. It's the luxury automakers first electric car and second SUV.

