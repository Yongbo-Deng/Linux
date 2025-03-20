#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


/* socket
 * send
 */

 #define SERVER_PORT  8888

int main(int argc, char **argv)
{
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;

    int iRet;
    unsigned char ucSendBuf[1000];
    int iSendLen;

    if (argc != 2)
    {
        printf("Usage: %s <server_ip>\n", argv[0]);
    }

    iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

    tSocketServerAddr.sin_port      = htons(SERVER_PORT);
    tSocketServerAddr.sin_family    = AF_INET; 
    // tSocketServerAddr.sin_addr.s_addr = ;
    if (inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0)
    {
        printf("Invalid server IP.\n");
        return -1;
    }

    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (iRet == -1)
    {
        perror("Connect error.");
        return -1;
    }
    while (1) {
        if (fgets(ucSendBuf, 999, stdin))
        {
            iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);
            if (iSendLen <= 0)
            {
                close(iSocketClient);
                return -1;
            }
        }
    }    
    
    return 0;
}