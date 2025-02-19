#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

int alarm_duration; // Global variable to store the alarm duration

// Signal handler for the alarm signal
void alarm_handler(int signum) {
    fprintf(stderr, "** The server will be terminated after %d seconds of inactivity **\n", alarm_duration);
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Error: Please provide port number and time duration as arguments.\n");
        return 1;
    }

    // Extract port number and alarm duration from command-line arguments
    int portNumber = atoi(argv[1]);
    alarm_duration = atoi(argv[2]);

    // Register the alarm handler function for the SIGALRM signal
    signal(SIGALRM, alarm_handler);
    // Set an initial alarm for the specified duration
    alarm(alarm_duration);

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    // Print informational messages about server configuration
    fprintf(stderr, "** The server uses port# %d to listen with %d seconds for alarm **\n", portNumber, alarm_duration);
    fprintf(stderr, "** The server will be terminated after %d seconds of inactivity **\n", alarm_duration);
    // Execute system commands to display additional information
    system("date; hostname; whoami; ps; ls -l");

    // Create a socket for listening
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    // Configure server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portNumber);

    // Bind the socket to the specified port
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Start listening for incoming connections
    listen(listenfd, 10);

    // Main server loop to handle connections
    while (1) {
        // Accept a connection from a client
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        // Reset the alarm for inactivity
        alarm(alarm_duration);

        // Get the current time
        ticks = time(NULL);
        // Format the time as a string and write to the client
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));

        // Close the connection and introduce a short delay
        close(connfd);
        sleep(1);
    }

    // The server should never reach this point as it runs indefinitely
    return 0;
}
