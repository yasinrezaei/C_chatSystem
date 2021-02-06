#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h> 
#include <time.h>

void *threadsent(void *v);
void *threadrecv(void *v);  
void *threadconnect(void *v) ;
void *threadchatroom(void *v) ;
void *threadsentserver(void *v);
void becomeserver();
void joinserver();
int numberofclient=-1;
int startclient=-1;
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
SOCKET ConnectSocket = INVALID_SOCKET;
int iResult;
int kiahastand[9];
  SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket[11] ;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
char xs[20],xs2[20];
char name[20],ip[20],ipserver[10];
   char name2[10][20];
   char ip2[10][20];
int startip=0;
    WSADATA wsaData;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";
    char recvbuf[10][DEFAULT_BUFLEN];
  
    int recvbuflen = DEFAULT_BUFLEN;

//int __cdecl main(int argc, char **argv) 
int main(int argc, char **argv) 
{

    
    // Validate the parameters

    // Initialize Winsock
        printf("write your id:");
    fgets(name, 20, stdin);
        printf("write your ip:");
    fgets(ip, 20, stdin);

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    iResult = send( ConnectSocket, name, 20, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();

    }

    iResult = send( ConnectSocket, ip, 20, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();

    }
    


    // Send an initial buffer
    pthread_t t1,t2;

    pthread_create(&t1, NULL, (&threadsent), NULL);
    pthread_create(&t2, NULL, (&threadrecv), NULL);

    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 
    
}
void *threadsent(void *v)
{
    while (1)
    {
        
    fgets(xs, 20, stdin);
    if (xs[0]=='#')
    {
        if (xs[1]=='T')
        {
            iResult = send( ConnectSocket, xs, 20, 0 );
            fgets(xs, 20, stdin);


            int len=strlen(xs);
            xs[len-1]='\0';
            iResult = send( ConnectSocket, xs, 20, 0 );  
            FILE *fp;
            fp=fopen(xs,"r");
            while (fgets(xs, 40, fp)!=NULL)
            {
                iResult = send( ConnectSocket, xs, 20, 0 );
    

            }

    fclose(fp) ; 
    strcpy(xs,"end");
 
        }
        
  
    }
    
    iResult = send( ConnectSocket, xs, 20, 0 );
    }
    


}
void *threadrecv(void *v)
{
   
    while (1)
    {
            iResult = recv(ConnectSocket, xs2, 20, 0);
    if (iResult>0)
    {
        if (strcmp("#T",xs2)==0)
         {
            iResult = recv(ConnectSocket, xs2, 20, 0);

            FILE *fp;
            fp=fopen(xs2,"w");
            iResult = recv(ConnectSocket, xs2, 20, 0);
            printf("You recieve File From:");
            printf(xs2);
            printf("\n");

            while (1)
            {
                iResult = recv(ConnectSocket, xs2, 20, 0);

                if (strcmp("end",xs2)==0)
                {
                    break;
                }
                fputs(xs2, fp);   
            }
            fclose(fp) ;

        }
          
         else if (strcmp("#IP",xs2)==0)
         {
             iResult = recv(ConnectSocket, xs2, 20, 0);
             strcpy(ipserver,xs2);
             
        }

        else
        {
                    printf(xs2);
        }

    }
    else
    {
        closesocket(ConnectSocket);
        printf("server lost\n");
        printf("wait to join to another server\n");

        break;
    }
    
    }
    int str;
    str=strlen(ip);
    ip[str-1]='\0';

    
    if (strcmp(ip,ipserver)==0)
    {
            closesocket(ConnectSocket);

       

     becomeserver();

    }
    else
    {
        closesocket(ConnectSocket);
        joinserver();
    }

}  
void becomeserver()
{


    for (int i = 0; i <= 9; i++)
    {
    kiahastand[i]=0;
    }
   // char clientname;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);  
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();

    }
    

    freeaddrinfo(result);
        iResult = listen(ListenSocket, 10);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
     //   return 1;
    }
    printf("you are server now\n");
    pthread_t thread1,t3;
    startclient=0;
    numberofclient=-1;

    pthread_create(&thread1, NULL, (&threadconnect), NULL);  
    pthread_join(thread1, NULL); 


}
void joinserver()
{
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);

    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ipserver, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr!= NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();

        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();

    }
    printf("welcome back to chatroom\n");



    printf("server find\n");
    iResult = send( ConnectSocket, name, 20, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();

    }

    iResult = send( ConnectSocket, ip, 20, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();

    }
    pthread_t t1,t2;

    pthread_create(&t1, NULL, (&threadsent), NULL);
    pthread_create(&t2, NULL, (&threadrecv), NULL);
    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 



}

void *threadconnect(void *v) 
{
    
    //startclient+=1;
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

iResult = recv(ClientSocket[k], name2[k], recvbuflen, 0);
    if (iResult>0)
    {
        len=strlen(name2[k]);
        name2[k][len-1]='\0';

        
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
            
        iResult = send( ClientSocket[j], name2[k], recvbuflen, 0 );
        iResult = send( ClientSocket[j], " join to chatroom\n", recvbuflen, 0 );
        }
        printf(name2[k]);
        printf(" join to chatroom\n");

    }

        iResult = recv(ClientSocket[k], ip2[k], recvbuflen, 0);
    if (iResult>0)
    {
        len=strlen(ip2[k]);
        ip2[k][len-1]='\0';

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
                iResult = send( ClientSocket[j], ip2[i], recvbuflen, 0 );
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
 

                                if (strcmp(name2[mn],recvbuf[k])==0)
                                {
                                private=1;
                                

                                
                                mn2=mn;
                                iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);
                                
                               iResult = send( ClientSocket[mn2], "private from ", recvbuflen, 0 );
                               iResult = send( ClientSocket[mn2], name2[k], recvbuflen, 0 );

                               iResult = send( ClientSocket[mn2], " :", recvbuflen, 0 );
                               iResult = send( ClientSocket[mn2], recvbuf[k], recvbuflen, 0 );
                                }
                                if (strcmp(name,recvbuf[k])==0)
                                {
                                    iResult = recv(ClientSocket[k], recvbuf[k], recvbuflen, 0);
                                    printf("private from : ");
                                    printf(name2[k]);
                                    printf(recvbuf[k]);


                                    break;
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
                     if (kiahastand[ii]==0)
                    {
                        continue;

                    }

                     iResult = send( ClientSocket[ii], recvbuf[k], recvbuflen, 0 );
                     iResult = send( ClientSocket[ii], name2[k], recvbuflen, 0 );
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
            iResult = send( ClientSocket[j], name2[k], recvbuflen, 0 );
            iResult = send( ClientSocket[j], " : ", recvbuflen, 0 );
        iResult = send( ClientSocket[j], recvbuf[k], recvbuflen, 0 );
        }
        printf("message from ");
        printf(name2[k]);
        printf(": ");
        printf(recvbuf[k]);


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
                     
                    iResult=send( ClientSocket[ii], name2[k], recvbuflen, 0 );
                    iResult=send( ClientSocket[ii], " leave chatroom\n", recvbuflen, 0 );
                    numberofclient=numberofclient-1;
                    kiahastand[k]=0;

                }
        
        printf(name2[k]);
        printf(" leave chatroom\n");

                

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
                iResult = send( ClientSocket[j], ip2[i], recvbuflen, 0 );

        }
        
        break;

    }
    
        }

                 closesocket(ClientSocket[k]);
        
        break;
    }
    


}





}

void *threadsentserver(void *v)
{
    
    while (1)
    {

        
    fgets(xs, 20, stdin);
    printf("yes");

    if (xs[0]=='#')
    {
        if (xs[1]=='T')
        {
            for (int i = 0; i <=9; i++)
            {
                if (kiahastand[i]==0)
                {
                    continue;

                }
                iResult = send( ClientSocket[i], "#T", 20, 0 );

            }
            
            
            fgets(xs, 20, stdin);


            int len=strlen(xs);
            xs[len-1]='\0';
             for (int i = 0; i <=9; i++)
            {
                if (kiahastand[i]==0)
                {
                    continue;

                }
                iResult = send( ClientSocket[i], xs, 20, 0 );

            }
            FILE *fp;
            fp=fopen(xs,"r");
            while (fgets(xs, 40, fp)!=NULL)
            {
                for (int i = 0; i <=9; i++)
             {
                if (kiahastand[i]==0)
                {
                    continue;

                }
                iResult = send( ClientSocket[i], xs, 20, 0 );
             }
                
            }
    

    fclose(fp) ; 
    strcpy(xs,"end");
                for (int i = 0; i <=9; i++)
            {
                if (kiahastand[i]==0)
                {
                    continue;
  
                }

                iResult = send( ClientSocket[i], xs, 20, 0 );

            }

        }
        
    }
    else
    {
             
            for (int i = 0; i <=9; i++)
            {
                if (kiahastand[i]==0)
                {
                    continue;

                }

                iResult = send( ClientSocket[i], xs, 20, 0 );

            }

    }

    }
    

}








