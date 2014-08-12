#ifndef _TCPSERVER_H
#define _TCPSERVER_H



#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>



#define	  SERVERPORT			5555
#define   MAX_NUM_BUF			1024

class Server
{
public:
	Server();
	~Server();

public:
	int CSocket();
	int CBind();
	int CListen();
	int CAccept();
	int CRecv();
	int CSend();
	int CClose();


public:
	int       retVal ;
	char	  bufRecv[MAX_NUM_BUF];			//read buffer
	char	  bufSend[MAX_NUM_BUF];			//send buffer
    
    int       server_fd;              //server file discripter
    int       client_fd;              //client file discripter

    
    
	bool	  Conning;
};


#endif