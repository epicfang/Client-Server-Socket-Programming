#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Error: Please provide IP address and port number as arguments.\n");
        return 1;
    }

    char ip[12];  // characters for argument
    strcpy(ip, argv[1]); // takes the argument data type into a character array
    int portNumber = atoi(argv[2]); // the default port of the server

    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    // Initialize the receive buffer
    memset(recvBuff, '0', sizeof(recvBuff));

    // Create a socket for communication
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\nError: Could not create socket\n");
        return 1;
    }

    // Initialize the server address structure
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNumber);

    // Print information about the client connecting to the server
    fprintf(stderr, "** The client is to connect to the server of IP# %s and Port# %i **\n", ip, portNumber);
    system("date; hostname; whoami; ps; ls -l");

    // Convert the IP address to binary form and set it in the server address structure
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        perror("\ninet_pton error occurred\n");
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("\nError: Connect Failed\n");
        return 1;
    }

    // Print a message indicating that the client will be terminated
    fprintf(stderr, "** The client will be terminated **\n");

    // Receive data from the server and print it to stdout
    while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
        recvBuff[n] = 0;
        if (fputs(recvBuff, stdout) == EOF) {
            perror("\nError: Fputs error\n");
        }
    }

    // Check for read errors
    if (n < 0) {
        perror("\nRead error\n");
        exit(1);
    }

    // Print a message indicating that the client has ended
    fprintf(stderr, "** The client ends **\n");
    // Display additional information
    system("date; hostname; whoami; ps; ls -l");
    return 0;
}
