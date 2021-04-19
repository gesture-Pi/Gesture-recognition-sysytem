#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <iostream> 
#include <string.h>
#include <cstring>
#define SERV_IP "192.168.1.8"
#define SERV_PORT 6666
using namespace std;

int main(void)
{
    int lfd, cfd; 
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    char clie_IP;
    char fuser[BUFSIZ],suser[BUFSIZ],word[40];
    int stf[2],fts[2];
    pipe(stf);
    pipe(fts);
    lfd = socket(AF_INET, SOCK_STREAM, 0);
 
    serv_addr.sin_family = AF_INET;                
    serv_addr.sin_port = htons(SERV_PORT);         
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    
 
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 
    listen(lfd, 128);
 
    clie_addr_len = sizeof(clie_addr); 
 
    pid_t pid;
    pid=fork();
    if ( pid==0)
    {
       
    cfd = accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
 
    printf("client IP:%s, client port:%d you are player 1\n",
		inet_ntop(AF_INET, (struct sockaddr *)&clie_addr.sin_addr,&clie_IP,sizeof(clie_IP)),
		ntohs(clie_addr.sin_port));
    
    
    while (1) {
        memset(fuser,0, sizeof(fuser));
        memset(suser,0, sizeof(suser));
        memset(word,0, sizeof(word));
       
        sleep(1);
        read(cfd, fuser, sizeof(fuser));
        cout<<"player 1 send "<<fuser<<endl;
        write(fts[1],fuser,sizeof(fuser));
        if( sizeof(stf[0])>3)
        {
            read(stf[0],suser,sizeof(suser));
        }
        if(strcmp(fuser,"r\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                cout<<"draw!"<<endl;
                strcpy(word, "you are player1\ndraw!\n");
                
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"r\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                
                strcpy(word, "you are player1\nplayer1 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"r\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                
                cout<<"player2 win!"<<endl;
                strcpy(word, "you are player1\nplayer2 win!\n");
                write(cfd, word, sizeof(word));
            }
            
            
            
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                
                cout<<"draw!"<<endl;
                strcpy(word, "you are player1\ndraw!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                
                cout<<"player2 win!"<<endl;
                strcpy(word, "you are player1\nplayer2 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                
                cout<<"player1 win!"<<endl;
                strcpy(word, "you are player1\nplayer1 win!\n");
                write(cfd, word, sizeof(word));
            }
            
            
            
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                strcpy(word, "you are player1\nplayer2 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                strcpy(word, "you are player1\nplayer1 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                strcpy(word, "you are player1\ndraw!\n");
                write(cfd, word, sizeof(word));
            }
            else
            {
                strcpy(word, "wait for other player\n");
                write(cfd, word, sizeof(word));
        
        
    }
 }
    close(lfd);
    close(cfd);
 }

 if ( pid!=0)
    {
       
    cfd = accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
 
    printf("client IP:%s, client port:%d you are player 2\n",
		inet_ntop(AF_INET, (struct sockaddr *)&clie_addr.sin_addr,&clie_IP,sizeof(clie_IP)),
		ntohs(clie_addr.sin_port));
    
    
    while (1) {
        memset(fuser,0, sizeof(fuser));
        memset(suser,0, sizeof(suser));
        memset(word,0, sizeof(word));
        read(cfd, suser, sizeof(suser));
        
        write(stf[1],suser,sizeof(suser));
        read(fts[0],fuser,sizeof(fuser));
        cout<<"player 2 send  "<<suser<<endl;
        if(strcmp(fuser,"r\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                cout<<"draw!"<<endl;
                strcpy(word, "you are player2\ndraw!\n");
                
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"r\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                
                strcpy(word, "you are player2\nplayer1 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"r\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                
                cout<<"player2 win!"<<endl;
                strcpy(word, "you are player2\nuser2 win!\n");
                write(cfd, word, sizeof(word));
            }
            
            
            
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                
                cout<<"draw!"<<endl;
                strcpy(word, "you are player2\ndraw!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                
                cout<<"player2 win!"<<endl;
                strcpy(word, "you are player2\nuser2 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"s\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                
                cout<<"player1 win!"<<endl;
                strcpy(word, "you are player2\nuser1 win!\n");
                write(cfd, word, sizeof(word));
            }
            
            
            
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"s\n") ==0)
            {
                strcpy(word, "you are player2\nuser2 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"r\n") ==0)
            {
                strcpy(word, "you are player2\nuser1 win!\n");
                write(cfd, word, sizeof(word));
            }
        else if( strcmp(fuser,"p\n") ==0 && strcmp(suser,"p\n") ==0)
            {
                strcpy(word, "you are player2\ndraw!\n");
                write(cfd, word, sizeof(word));
            }
        else
        {
            strcpy(word, "wait for other player\n");
                write(cfd, word, sizeof(word));
        
    }
 }
    close(lfd);
    close(cfd);
 
}
}
