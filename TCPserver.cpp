#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/stat.h>

void chomp(char* s);

using namespace std;

int main(int argc, char *argv[])
{

	int count = 0;
	int sock;
	socklen_t serverlen, clientlen;
	struct sockaddr_in echoclient;
	struct sockaddr_in echoserver;
	char buffer[1000];
	ssize_t received;
	const char* dirName = "newDir";


	//create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	//Clear Struct
	memset(&echoserver, 0, sizeof(echoserver));
	//Internet IP
	echoserver.sin_family = AF_INET;
	//IP address
	echoserver.sin_addr.s_addr = INADDR_ANY;
	//server port
	echoserver.sin_port = htons(atoi(argv[1]));

	//bind socket
	serverlen = sizeof(echoserver);
	if(bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0)
	{
		perror("Failed to bind server socket");
		exit(EXIT_FAILURE);
	}

	//listen
	if(listen(sock, 62) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	//While loop that will run until canceled
	while(true)
	{
		//accept
	  clientlen = sizeof(echoclient);
	  int newsock = accept(sock, (struct sockaddr *) &echoclient, &clientlen);
	  pid_t pid = fork();

	  if (newsock < 0)
	  {
		perror("accept failed");
		exit(EXIT_FAILURE);
	  }

	  if(pid == 0){

	//received connection
            if((received = read(newsock, buffer, 256)) <= 0)
	    {
		perror("Failed to receive message");
		exit(EXIT_FAILURE);
	    }

	    buffer[received] = '\0';
	    chomp(buffer);

	    if(dup2(newsock, 1) < 0)
	    {
		perror("socket dup2");
		exit(EXIT_FAILURE);
	    }

	    char* argv[] = {(char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0 };

		//tokenizes string
	    for(char* p = strtok(buffer, " "); p; p = strtok(NULL, " "))
		{
			argv[count++] = p;
		}

	    if(execvp(argv[0], argv) < 0)
	    {
		perror("exec in child after fork");
		exit(EXIT_FAILURE);
	    }

	    //read directory
	    if(argc == 2)
		dirName = argv[1];

            if(mkdir(dirName, 0777) < 0)
	    {
		perror(dirName);
		exit(EXIT_FAILURE);
	    }

	    DIR* dirp;
	    struct dirent* dirEntry;

	    dirp = opendir(argv[1]);
	    if (dirp == 0)
	    {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	    }

	    while((dirEntry = readdir(dirp)) != NULL)
		cout << dirEntry->d_name << endl;

	    closedir(dirp);

	  }//end child
	  else{
	    close(newsock);
	  }
	}//end while
  	close(sock);
  	return 0;
}//end main

//function that removes trailing newline or carriage return from string
void chomp(char *s)
{
	for(char* p = s + strlen(s) -1; *p == '\r' || *p == '\n'; p--)
		*p = '\0';

}

