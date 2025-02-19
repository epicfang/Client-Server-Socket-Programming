#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>     // Include string.h for strcpy and memset
#include <arpa/inet.h>
#include <netinet/in.h> // Include netinet/in.h for struct sockaddr_in
#include <sys/types.h>  // Include sys/types.h for socket programming

int main(int argc, char *argv[]) {
    char ip[12];  // characters for argument
    strcpy(ip, argv[1]); //takes the argument data type into a character array
    int portNumber = atoi(argv[2]); //the default port of the server

    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\nError: Could not create socket\n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNumber); 
    char call[512];

    // Use sprintf correctly by providing a buffer as the first argument
    sprintf(call, "netstat -aont | grep \" `hostname -i`:%i \"", portNumber);
    system("date; hostname; whoami");
    system(call);

    printf("\ntimeClient: connecting to %s Port#= %i\n", ip, portNumber);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("\ninet_pton error occurred\n");
        return 1;
    } 

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("\nError: Connect Failed\n");
        return 1;
    } 

    printf("\ntimeClient: connected to timeServer.\n");

    // Use sprintf to format the command and concatenate into a single string
    sprintf(call, "netstat -aont | grep \":%i \"", portNumber);
    system(call);
    printf("\n\n");

    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF) {
            perror("\nError: Fputs error\n");
        }
    } 

    if (n < 0) {
        perror("\nRead error\n");
        exit(1);
    } 

    printf("\ntimeClient: now terminated.\n");
    return 0;
}
