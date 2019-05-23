  /* Server code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
 
  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
    char msg[256];
 
    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(2025);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    int ConnectFD = accept(SocketFD, NULL, NULL);
    bzero(buffer,256);
    while(true){
      while(buffer[0]==0)
        read(ConnectFD,buffer,255);
      if(buffer[0]=='.'){
        printf("The chat finished\n");
        break;
      }
      printf("%s\n",buffer);
      bzero(buffer,256);
      scanf("%s",msg);
      write(ConnectFD,msg,sizeof(msg));
      if(msg[0]=='.')
        break;
      bzero(msg,256);
    }

    close(SocketFD);
    return 0;
  }
