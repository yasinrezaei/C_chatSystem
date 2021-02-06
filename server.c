#undef UNICODE

#define WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include<string.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h> 

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 20
#define DEFAULT_PORT "27015"


void *threadconnect(void *v) ;
void *threadchatroom(void *v) ;
int startclient=-1;
 WSADATA wsaData;
 char x[10];
 int iResult;

  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket[11] ;

    

  struct addrinfo *result = NULL;
  struct addrinfo hints;

  int iSendResult;
   char recvbuf[10][DEFAULT_BUFLEN];
   int recvbuflen = DEFAULT_BUFLEN;
   pthread_t threadchat[10],threadct[10];
   char name[10][20];
   char ip[10][20];
   int kiahastand[10];
int numberofclient=-1;

//int __cdecl main(void) 
int main() 
{
for (int i = 0; i <= 10; i++)
{
    kiahastand[i]=0;
    /* code */
}
   // char clientname;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);
        iResult = listen(ListenSocket, 10);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();

    }
    pthread_t thread1;
    pthread_create(&thread1, NULL, (&threadconnect), NULL); 
    pthread_join(thread1, NULL); 

    return 0;
}

void *threadconnect(void *v) 
{
    Sleep(1);
    numberofclient+=1;

    while (1)
    {
        if (numberofclient==10)
        {
            ClientSocket[startclient] = accept(ListenSocket, NULL, NULL);
            iResult = send( ClientSocket[startclient], "server is full try later", recvbuflen, 0 );
            closesocket(ClientSocket[startclient]);
            continue;

        }
        for (int i = 0; i < 10; i++)
        {
            if (kiahastand[i]==0)
            {
                kiahastand[i]=1;
                startclient=i;
                break;

            }

        }

    // Accept a client socket
    ClientSocket[startclient] = accept(ListenSocket, NULL, NULL);
    if (ClientSocket[startclient] == INVALID_SOCKET) {

        continue;
        printf("yes");
    }   
pthread_t t1,t2;

    pthread_create(&t1, NULL, (&threadchatroom), NULL); 
    pthread_create(&t2, NULL, (&threadconnect), NULL); 
    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 
    }
    

}
void *threadchatroom(void *v)
{
int k;
k=startclient;
int private;
int len;
int mn2;
iResult = recv(ClientSocket[k], name[k], recvbuflen, 0);
    if (iResult>0)
    {
        len=strlen(name[k]);
        name[k][len-1]='\0';

        
        for (int j = 0; j <= 9; j++)
        {
            if (j== k)
            {
                continue;

              
            }
            if (kiahastand[j]==0)
            {
                continue;
             
            }
            
        iResult = send( ClientSocket[j], name[k], recvbuflen, 0 );
        iResult = send( ClientSocket[j], " join to chatroom\n", recvbuflen, 0 );

            


        }
        

    }

        iResult = recv(ClientSocket[k], ip[k], recvbuflen, 0);
    if (iResult>0)
    {
        len=strlen(ip[k]);
        ip[k][len-1]='\0';

    }

    
for (int i = 0; i < 10; i++)
{
    
    if (kiahastand[i]==1)
    { 
        for (int j = 0; j < 10; j++)
        {
            if (kiahastand[j]==0)
            {
                continue;
            }
                iResult = send( ClientSocket[j], "#IP", recvbuflen, 0 );
                iResult = send( ClientSocket[j], ip[i], recvbuflen, 0 );
        }

        break;
  
    }
    
}
while (1)
{
    private=0;
    iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);
    if (iResult>0)
    {
        if (recvbuf[k][0]=='#')
        {
            if (recvbuf[k][1]=='P')
            {
                
                    iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);
                    if (iResult>0)
                        {

                             len=strlen(recvbuf[k]);
                            recvbuf[k][len-1]='\0';
                            for (int mn = 0; mn <= 9; mn++)
                            {

                                   if (kiahastand[mn]==0)
                                               {
                                                    continue;
                                                }
                                if (strcmp(name[mn],recvbuf[k])==0)
                                {
                                private=1;
                                mn2=mn;
                                iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);
                                
                               iResult = send( ClientSocket[mn2], "private from ", recvbuflen, 0 );
                               iResult = send( ClientSocket[mn2], name[k], recvbuflen, 0 );

                               iResult = send( ClientSocket[mn2], " :", recvbuflen, 0 );
                               iResult = send( ClientSocket[mn2], recvbuf[k], recvbuflen, 0 );
                                }

                            }

                        }
            }
            else if (recvbuf[k][1]=='T')
            {
                private=1;
                for(int ii=0;ii<=9;ii++)
                {
                                if (kiahastand[ii]==0)
                               {
                                    continue;

                               }
                    if (ii==k)
                    {
                        continue;
                      
                    }
                    iResult = send( ClientSocket[ii], "#T", recvbuflen, 0 );
                }
                iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);            
                for(int ii=0;ii<=9;ii++)
                {
                    if (ii==k)
                    {
                        continue;
                        
                    }
                     if ( kiahastand[ii]==0)
                    {
                        continue;
                       
                     
                    }

                     iResult = send( ClientSocket[ii], recvbuf[k], recvbuflen, 0 );
                     iResult = send( ClientSocket[ii], name[k], recvbuflen, 0 );
                }
                while (1)
                {
                  iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);     
      
                for(int ii=0;ii<=9;ii++)
                {
                    if (ii==k)
                    {
                        continue;
                        
                    }
                     if ( kiahastand[ii]==0)
                    {
                        continue;   
                    }


                         iResult = send( ClientSocket[ii], recvbuf[k], recvbuflen, 0 );     
                     }
                         if (strcmp("end",recvbuf[k])==0)
                         {
                             break;
                            
                         }           
                }
                }
  
            }

        
        if (private==1)
        {
            private=0;
            continue;

        }
        
        
        for (int j = 0; j <= startclient; j++)
        {
            if (j== k)
            {
                continue;

            }
            iResult = send( ClientSocket[j], "message from ", recvbuflen, 0 );
            iResult = send( ClientSocket[j], name[k], recvbuflen, 0 );
            iResult = send( ClientSocket[j], " : ", recvbuflen, 0 );
        iResult = send( ClientSocket[j], recvbuf[k], recvbuflen, 0 );

        }
        
    }
    
    else
    {
       
        kiahastand[k]=0;
            for(int ii=0;ii<=9;ii++)
                {
                    if (ii==k)
                    {
                        continue;
                
                    }
                     if (kiahastand[ii]==0)
                     {
                          continue;
                     }
                     
                    iResult=send( ClientSocket[ii], name[k], recvbuflen, 0 );
                    iResult=send( ClientSocket[ii], " leave chatroom\n", recvbuflen, 0 );
                    numberofclient=numberofclient-1;
                    kiahastand[k]=0;
                }
                

    for (int i = 0; i < 10; i++)
     {
      
    if (kiahastand[i]==1)
    {
      
        for (int j = 0; j < 10; j++)
        {
            if (kiahastand[j]==0)
            {
                continue;


            }
                iResult = send( ClientSocket[j], "#IP", recvbuflen, 0 );
                iResult = send( ClientSocket[j], ip[i], recvbuflen, 0 );

        }

        break;

    }
    
        }
                closesocket(ClientSocket[k]);
        
        break;
 
    }
    
}


}







