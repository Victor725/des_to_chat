#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include  "Des.h"

using namespace std;

#define SERVERPORT 9370
#define BUFFERSIZE 1024

ssize_t TotalRecv(int s, void *buf, size_t len, int flags)
{
	size_t nCurSize = 0;
	while(nCurSize <len)
	{
		ssize_t nRes = recv(s,((char*)buf)+nCurSize,len-nCurSize,flags);
		if(nRes<0||nRes+nCurSize>len)
		{
			return -1;
		}
		nCurSize+=nRes;
	}
	return nCurSize;
}


void SecretChat(int sock, char* ip_addr, char* key){
    CDesOperate cDes;
	if(strlen(key)!=8)
	{
		printf("Key length error");
		return ;
	}
	
	pid_t nPid;
	nPid = fork();
	if(nPid != 0)
	{
		char strSocketBuffer[BUFFERSIZE];
		char strDecryBuffer[BUFFERSIZE];
		pid_t wpid;
		int stat;
		while(1)
		{
			bzero(&strSocketBuffer, BUFFERSIZE);
			int nLength = 0;
			nLength = TotalRecv(sock, strSocketBuffer,BUFFERSIZE,0);
			if(nLength !=BUFFERSIZE)
			{
				break;
			}
			else
			{
				int nLen = BUFFERSIZE;
				cDes.Decry(strSocketBuffer,BUFFERSIZE,strDecryBuffer,nLen,key,8);
				strDecryBuffer[BUFFERSIZE-1]=0;
				if(strDecryBuffer[0]!=0&&strDecryBuffer[0]!='\n')
				{
					if(0==memcmp("quit",strDecryBuffer,4))
					{
						char strStdinBuffer[BUFFERSIZE]="quit";
						char strEncryBuffer[BUFFERSIZE];
						int nLen = BUFFERSIZE;
						cDes.Encry(strStdinBuffer,BUFFERSIZE,strEncryBuffer,nLen,key,8);
						if(send(sock, strEncryBuffer, BUFFERSIZE,0)!=BUFFERSIZE)
						{
							perror("send");
						}
						printf("Quit!\n");
						break;
					}
					printf("Receive message form <%s>: %s\n", ip_addr,strDecryBuffer);
				}
				/*if(strSocketBuffer[0]!=0&&strSocketBuffer[0]!='\n')
				{
					printf("Receive message form <%s>: %s\n", ip_addr,strSocketBuffer);
					if(0==memcmp("quit",strSocketBuffer,4))
					{
						printf("Quit!\n");
						break;
					}
				}*/
			}
		}
	}
	else
	{ 
		char strStdinBuffer[BUFFERSIZE];
		char strEncryBuffer[BUFFERSIZE];
		while(1)
		{
			bzero(&strStdinBuffer, BUFFERSIZE);
			while(strStdinBuffer[0]==0)
			{
				if (fgets(strStdinBuffer, BUFFERSIZE, stdin) == NULL) 
				{
					continue;
				}
			}      
			int nLen = BUFFERSIZE;
			cDes.Encry(strStdinBuffer,BUFFERSIZE,strEncryBuffer,nLen,key,8);
			if(send(sock, strEncryBuffer, BUFFERSIZE,0)!=BUFFERSIZE)
			{
				perror("send");
			}
			/*if(send(sock, strStdinBuffer, BUFFERSIZE,0)!=BUFFERSIZE)
			{
				perror("send");
			}*/
			else
			{
				if(0==memcmp("quit",strStdinBuffer,4))
				{
					//printf("Quit!\n");
					break;
				}
			}
		}
	}
}

int main(){
    char opt[2];
    cout<<"Client or Server?"<<endl;
    cin>>opt;
    if(opt[0]=='c'){
        cout<<"Please input the server address:"<<endl;
        char strIpAddr[16];
        cin>>strIpAddr;
        int nConnectSocket;
        struct sockaddr_in sDestAddr;
        if ((nConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
            perror("Socket");
            exit(errno);
        }
        sDestAddr.sin_family = AF_INET;
        sDestAddr.sin_port = htons(SERVERPORT);
        sDestAddr.sin_addr.s_addr = inet_addr(strIpAddr);
        if (connect(nConnectSocket, (struct sockaddr *) &sDestAddr, sizeof(sDestAddr)) != 0) 
        {
            perror("Connect ");
            exit(errno);
        }
        else
        {
            printf("Connect Success!  \nBegin to chat...\n");
            SecretChat(nConnectSocket,strIpAddr,"benbenmi");	
        }
        close(nConnectSocket);
    }
    else if(opt[0]=='s'){
        char strIpAddr[16]="127.0.0.1";
        int nListenSocket, nAcceptSocket;
        struct sockaddr_in sLocalAddr, sRemoteAddr;
        socklen_t nLength = sizeof(sockaddr_in);

        if ((nListenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
            perror("Socket");
            exit(errno);
        }

        int one=1;
        if(setsockopt(nListenSocket,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one))<0){
            perror("Socket opt set");
            exit(errno);
        }

        sLocalAddr.sin_family = AF_INET;
        sLocalAddr.sin_port = htons(SERVERPORT);
        sLocalAddr.sin_addr.s_addr = inet_addr(strIpAddr);



        if (bind(nListenSocket, (struct sockaddr *) &sLocalAddr, sizeof(struct sockaddr))== -1)
        {
            perror("bind");
            exit(1);
        }

        cout<<"Listening..."<<endl;
        if (listen(nListenSocket, 5) == -1) 
        {
            perror("listen");
            exit(1);
        }
        nAcceptSocket = accept(nListenSocket,(struct sockaddr *) &sRemoteAddr,&nLength);
        close(nListenSocket);printf("server: got connection from %s, port %d, socket %d\n",inet_ntoa(sRemoteAddr.sin_addr),ntohs(sRemoteAddr.sin_port), nAcceptSocket);
        SecretChat(nAcceptSocket,inet_ntoa(sRemoteAddr.sin_addr),"benbenmi");		
        close(nAcceptSocket);
    }	
	return 0;
}