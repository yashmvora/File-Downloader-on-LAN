#include<stdio.h>
#include<string.h>  
#include<fcntl.h>
#include<unistd.h>  
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX_FILE_LENGTH 1000

int main(int argc, char *argv[])
{
    int socketDesc;
    char ipAddress[20];
    int portNum;
    char *message;
    char serverReply[MAX_FILE_LENGTH];
    char filename[50];

    FILE *file = NULL;
    struct sockaddr_in server;
    
    //Initialising socket
    socketDesc = socket(AF_INET, SOCK_STREAM, 0);

    printf("IP address: ");
    scanf("%s",ipAddress);
    printf("Port No: ");
    scanf("%d",&portNum);

    server.sin_addr.s_addr = inet_addr(ipAddress);
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);

    //Connecting to the server
    printf("\nConnect Status: ");
    if (connect(socketDesc, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("fail");
        return 1;
    }
    printf("success");

    while(1)
    {
        printf("\nInput the file name to be requested from the server: ");
        scanf("%s",filename);

        if (strcmp(filename,"exit")==0)
            break;

        message = filename;
        
        //Sending request to server
        printf("\nSend Status: ");
        if (send(socketDesc, message, strlen(message), 0) < 0)
        {
            printf("fail");
            return 1;
        }
        printf("success"); 
 
        char *clientFile = filename;
        printf("\n\nOpen file status: ");
        file = fopen(clientFile, "w");

        if (file == NULL)
        {
            printf("fail");
            return 1;
        }
        printf("success\n");

        //Receive file contents from server
        printf("\nReceived Text:\n");
        int receivedLength = recv(socketDesc, serverReply, sizeof serverReply, 0);
        while (receivedLength > 0)
        {
            printf("%s", serverReply);
            
            //Save text to client file
            printf("\nSave text status: ");
            if (fwrite(serverReply, receivedLength, 1, file))
                printf("success\n");
            else
                printf("fail\n");

            break;
        }
    }
    fclose(file);

    //Close connection
    close(socketDesc);

    return 0;
}
