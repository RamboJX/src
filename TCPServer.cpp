#include "TCPServer.h"
#include <iostream>
#include <maya/MGlobal.h>

using namespace std;


Server::Server()
{
	memset(bufRecv, 0, MAX_NUM_BUF);
	memset(bufSend, 0, MAX_NUM_BUF);            

	Conning = FALSE;
    
    retVal = 0;
}



int	Server::CSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1)
	{
        perror("create socket error\n");
		exit(0);
	}
	return 1;
}	

int	Server::CBind()
{
	struct sockaddr_in addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVERPORT);
	addrServ.sin_addr.s_addr = INADDR_ANY;

	retVal = bind(server_fd , (struct sockaddr*)&addrServ, sizeof(addrServ));
	if(-1 == retVal)
	{
        perror("bind socket error\n");
        exit(0);
	}
	return 1;
}

int	Server::CListen()
{
	retVal = listen(server_fd, 1);
	if(-1 == retVal)
	{
		perror("listen socket error\n");
		exit(0);
	}
	else
	{
        printf("server succeed on listen:\n");
		MGlobal::displayInfo( MString( "Server succeeded! On listen:\n" ) );
	}
	return 1;
}

int	Server::CAccept()
{
	sockaddr addrClient;
	//socklen_t addrClientlen = sizeof(addrClient);
	client_fd = accept(server_fd,(struct sockaddr*)&addrClient, NULL);
	if(-1 == client_fd)
	{
        perror("accept socket error\n");
        exit(0);
	}
	else
	{
		Conning = TRUE;
		MGlobal::displayInfo( MString( "Client accepted" ) );
        printf("client accepted\n");
	}

    
	MGlobal::displayInfo( MString( "accept a client: ip:" ) + addrClient.sa_data );
	return 1;
}


int	Server::CRecv()
{
	bool	retVal = TRUE;
	bool	bLineEnd = FALSE;
	int		nReadLen = 0;
	int		nDataLen = 0;
	memset(bufRecv, 0, MAX_NUM_BUF);  
	while (!bLineEnd && Conning)
	{
		nReadLen = recv(client_fd, bufRecv + nDataLen, 1, 0);
		if (-1 == nReadLen)
		{
		/*	int nErrCode = WSAGetLastError();
			if (WSAENOTCONN == nErrCode)
			{
				MGlobal::displayInfo( MString( "The socket is not connected!" ) );
			}
			else if(WSAESHUTDOWN == nErrCode)
			{
				MGlobal::displayInfo( MString( "The socket has been shut down!" ) );

			}else if (WSAETIMEDOUT == nErrCode)
			{
				MGlobal::displayInfo( MString( "The connection has been dropped!" ) );
				
			}else if (WSAECONNRESET == nErrCode)
			{
				MGlobal::displayInfo( MString( "The virtual circuit was reset by the remote side!" ) );
				
			}else{}	*/
            perror("recv error!\n");
			retVal = FALSE;
			break;
		}

		if (0 == nReadLen)
		{
			retVal = FALSE;
			break ;
		}

		if ('\n' == *(bufRecv + nDataLen))
		{
			bLineEnd = TRUE;        //what is this used for?
		}
		else
		{
			nDataLen += nReadLen;
		}	
	}

	if (!retVal)
	{  
		MGlobal::displayInfo( MString( "recieve over!" ) );
		return	-1;
	}	

	return 1;
}

int	Server::CSend()
{

	strcpy(bufSend, "Hello,Client!\n");	
	retVal = send(server_fd, bufSend, strlen(bufSend), 0);//Ò»´Î·¢ËÍ
	if(-1==retVal)
	{
		perror("send failed");
        exit(0);
	}
	return 1;
}

int	Server::CClose()
{
	
	MGlobal::displayInfo( MString( "Server exiting..." ) );
	return 1;	
}

Server::~Server()
{
}
