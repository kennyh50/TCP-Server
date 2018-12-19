# TCP-Server

This is a program I created for one of my classes to practice socket programming  with TCP/IP, including the system calls that were learned previously, as well as bind,
listen, and accept.
This program is a C++ program.  The program will essentially be a loop that goes on forever (until it is killed), waiting for a client to connect. When
a client connects, it accepts that connection, calls fork to make a child process, and handles communication from that
client in the child process. The parent process should continue to wait for more connections, accepting them and forking
as necessary. The program will accept two command line parameters:

1. the port number on which the server will be listening
2. the path to a directory that will serve as the root directory of the web server

This is a simple TCP server. If you need a tool to test it, you can use the telnet command to connect to your server
at the specified port. Type your command and hit enter, and the client should display what the server returns.
